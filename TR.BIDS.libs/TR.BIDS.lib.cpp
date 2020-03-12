//TR.BIDS.lib.cpp
//Under the MIT License
//Copyright 2020 Tetsu Otter

#define TIMEOUT_NUM 10000

#include <Arduino.h>
#include <math.h>
#include <stdlib.h>
#include "TR.BIDS.lib.h"

void ZeroFill(char *c, int len)
{
    for (int i = 0; i < len; i++)
        c[i] = 0;
}

BIDS::BIDS(Stream *ser)
{
    UsingSerial = ser;

    isEnable = true;
}

BIDS::~BIDS()
{
    isEnable = false;
}

bool BIDS::AddAutoSend(char type, int data_num, void (*Action)(int valI, double valF))
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
bool BIDS::AddAutoSend(ASAction asa)
{
    return AddAutoSend(asa.type, asa.data_num, asa.Action);
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
bool BIDS::RmvAutoSend(ASAction asa)
{
    return RmvAutoSend(asa.type, asa.data_num);
}

bool BIDS::ASDataCheck(bool *NonASCMDGot)
{
    *NonASCMDGot = false;
    if ((*UsingSerial).available() <= 0)
        return false;
    ZeroFill(LastCMD, LastCMD_len);

    int len = (*UsingSerial).readBytesUntil('\n', LastCMD, LastCMD_len);
    if (len <= 0)
        return false;
    if (len <= 4 || LastCMD[0] != 'T' || LastCMD[1] != 'R' || LastCMD[2] == 'I')
    {
        *NonASCMDGot = true;
        return false;
    }

    int data_start_pos = 0;
    for (int i = 4; i < len; i++)
    {
        if (LastCMD[len] == 'X')
        {
            data_start_pos = i + 1;
            break;
        }
    }
    if (data_start_pos == 0)
    {
        *NonASCMDGot = true;
        return false;
    }

    int dnum = atoi(&LastCMD[4]);
    int valI = atoi(&LastCMD[data_start_pos]);
    double valF = atof(&LastCMD[data_start_pos]);
    bool Done = false;
    for (int i = 0; i < Actions_count; i++)
        if (LastCMD[3] == ASActions[i].type && dnum == ASActions[i].data_num)
        {
            ASActions[i].Action(valI, valF);
            Done = true;
        }

    return Done;
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
bool BIDS::CmdSender(const char *cmd, int *ret)
{
    int len = 0;
    char charr[100];
    len = CmdSender(cmd, charr, 100);
    if (len <= 3 || charr[0] != 'T' || charr[1] != 'R' || charr[3] == 'E')
        return false;

    *ret = 0;
    for (int i = 0; i < len; i++)
    {
        if (charr[i] == 'X')
        {
            *ret = atoi(&charr[i + 1]);
            return true;
        }
        else if (cmd[i] != charr[i])
            return false;
    }
}
bool BIDS::CmdSender(const char *cmd, double *ret)
{
    int len = 0;
    char charr[100];
    len = CmdSender(cmd, charr, 100);
    if (len <= 3 || charr[0] != 'T' || charr[1] != 'R' || charr[3] == 'E')
        return false;

    *ret = 0;
    for (int i = 0; i < len; i++)
    {
        if (charr[i] == 'X')
        {
            *ret = atof(&charr[i + 1]);
            return true;
        }
        else if (cmd[i] != charr[i])
            return false;
    }
}
int BIDS::CmdSenderI(const char *cmd)
{
    int ret;
    CmdSender(cmd, &ret);
    return ret;
}
double BIDS::CmdSenderF(const char *cmd)
{
    double ret;
    CmdSender(cmd, &ret);
    return ret;
}
bool BIDS::IsEnable() { return isEnable; }
