//TR.BIDS.lib.h
//Under the MIT License
//Copyright 2020 Tetsu Otter
#ifndef TR_BIDS_LIB_H
#define TR_BIDS_LIB_H

#ifndef AS_MAX
#define AS_MAX 100
#endif

#define LAST_CMD_LEN 64

typedef struct ASAction
{
    char type;
    int data_num;
    void (*Action)(int valI, double valF);
} ASAction;

class BIDS
{
private:
    bool isEnable = false;
    ASAction ASActions[AS_MAX];
    int Actions_MAX = AS_MAX;
    int Actions_count = 0;
    Stream *UsingSerial;
    const int LastCMD_len = LAST_CMD_LEN;

public:
    char LastCMD[LAST_CMD_LEN];

    BIDS(Stream *ser);
    ~BIDS();
    bool AddAutoSend(char type, int data_num, void (*Action)(int valI, double valF));
    bool AddAutoSend(ASAction asa);
    bool RmvAutoSend(char type, int data_num);
    bool RmvAutoSend(ASAction asa);
    bool ASDataCheck(bool *NonASCMDGot);

    int CmdSender(const char *cmd, char *ret, int retlen);
    bool CmdSender(const char *cmd, int *ret);
    bool CmdSender(const char *cmd, double *ret);
    int CmdSenderI(const char *cmd);
    double CmdSenderF(const char *cmd);

    bool IsEnable();
};

#endif