//TR.BIDS.lib.cpp
//Under the MIT License
//Copyright 2020 Tetsu Otter

#define TIMEOUT_NUM 10000

#include "TR.BIDS.lib.h"
#include <Arduino.h>
#include <math.h>

BIDS::BIDS(Stream *ser)
{
    UsingSerial = ser;

    isEnable = true;
}

BIDS::~BIDS()
{
    isEnable = false;
}

bool BIDS::AddAutoSend(char type, int data_num, void *Action)
{
    if (Action = NULL)
        return false;
    if (Actions_count >= Actions_MAX)
        return false;
    if (CmdSenderInt(("TRA" + String(type) + String(data_num)).c_str()) != 0)
        return false;

    int Act_num = Actions_count++;
    ASActions[Act_num].type = Act_num;
    ASActions[Act_num].data_num = data_num;
    ASActions[Act_num].Action = Action;

    return true;
}

bool BIDS::RmvAutoSend(char type, int data_num)
{
    if (Actions_count <= 0)
        return false;
    bool IsFound = false;
    if (CmdSenderInt(("TRD" + String(type) + String(data_num)).c_str()) != 0)
        return false;
    for (int i = 0; i < Actions_count; i++)
    {
        if (ASActions[i].type == type && ASActions[i].data_num == data_num)
            IsFound = true;
        if (IsFound && (i + 1) < Actions_count)
        {
            ASActions[i].type = ASActions[i + 1].type;
            ASActions[i].data_num = ASActions[i + 1].data_num;
            ASActions[i].Action = ASActions[i + 1].Action;
        }
    }
    if (IsFound)
        Actions_count--;

    return IsFound;
}

int BIDS::CmdSender(const char *cmd, char *ret, int retlen)
{
    (*UsingSerial).println(cmd);
    int w = 0;
    while ((*UsingSerial).available() <= 0)
    {
        if (w >= TIMEOUT_NUM)
            return 0;
        w++;
    }
    int len = (*UsingSerial).available();
    if (len >= retlen)
        len = retlen;
    char *ret = new char[len];
    for (int i = 0; i < len; i++)
        ret[i] = (*UsingSerial).read();

    return len;
}
void BIDS::CmdSender(const char *cmd, int *ret)
{
    int len = 0;
    char charr[100];
    len = CmdSender(cmd, charr, 100);
    if (len <= 3 || charr[0] != 'T' || charr[1] != 'R' || charr[3] == 'E')
        return;

    *ret = 0;
    for (int i = 0; i < len; i++)
    {
        if (charr[i] == 'X')
        {
            bool IsMinus = false;
            i++;
            if (charr[i] == '-')
            {
                IsMinus = true;
                i++;
            }
            while (i < len)
            {
                if (charr[i] < '0' || '9' < charr[i])
                    break;
                *ret = (*ret) * 10 + (charr[i] - '0');
                i++;
            }
            if (IsMinus)
                *ret *= -1;
            return;
        }
    }
}
void BIDS::CmdSender(const char *cmd, float *ret)
{
    int len = 0;
    char charr[100];
    len = CmdSender(cmd, charr, 100);
    if (len <= 3 || charr[0] != 'T' || charr[1] != 'R' || charr[3] == 'E')
        return;

    *ret = 0;
    for (int i = 0; i < len; i++)
    {
        if (charr[i] == 'X')
        {
            bool IsMinus = false;
            bool IsDecArea = false;
            int d = 0;
            i++;
            if (charr[i] == '-')
            {
                IsMinus = true;
                i++;
            }
            while (i < len)
            {
                if (!IsDecArea && charr[i] == '.')
                {
                    IsDecArea = true;
                    d++;
                    continue;
                }
                if (charr[i] < '0' || '9' < charr[i])
                    break;
                int f = charr[i] - '0';
                if (IsDecArea)
                {
                    (*ret) += (float)f / pow(10, d);
                    d++;
                }
                else
                    *ret = (*ret) * 10 + f;
                i++;
            }
            if (IsMinus)
                *ret *= -1;
            return;
        }
    }
}
int BIDS::CmdSenderInt(const char *cmd)
{
    int ret;
    CmdSender(cmd, &ret);
    return ret;
}
float BIDS::CmdSenderFloat(const char *cmd)
{
    float ret;
    CmdSender(cmd, &ret);
    return ret;
}
bool BIDS::IsEnable() { return isEnable; }
