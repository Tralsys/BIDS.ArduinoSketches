//TR.BIDS.lib.h
//Under the MIT License
//Copyright 2020 Tetsu Otter
#ifndef TR_BIDS_LIB_H
#define TR_BIDS_LIB_H

#ifndef AS_MAX
#define AS_MAX 100
#endif
typedef struct ASAction
{
    char type;
    int data_num;
    void *Action;
} ASAction;

class BIDS
{
private:
    bool isEnable = false;
    ASAction ASActions[AS_MAX];
    int Actions_MAX = AS_MAX;
    int Actions_count = 0;
    Stream *UsingSerial;

public:
    BIDS(Stream *ser);
    ~BIDS();
    bool AddAutoSend(char type, int data_num, void *Action);
    bool RmvAutoSend(char type, int data_num);

    int CmdSender(const char *cmd, char *ret, int retlen);
    void CmdSender(const char *cmd, int *ret);
    void CmdSender(const char *cmd, float *ret);
    int CmdSenderInt(const char *cmd);
    float CmdSenderFloat(const char *cmd);

    bool IsEnable();
};

#endif