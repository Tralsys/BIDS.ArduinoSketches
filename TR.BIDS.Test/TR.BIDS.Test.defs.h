//TR.BIDS.Test.defs.h
//Under the MIT License
//Copyright 2020 Tetsu Otter

enum BIDSEls
{
  None,
  BSMD_BASE_BIAS,
  BSMD_IS_ENABLE = BSMD_BASE_BIAS,
  BSMD_VERSION_NUM,

  BSMD_SPEC_BIAS,
  BSMD_SPEC_B = BSMD_SPEC_BIAS,
  BSMD_SPEC_P,
  BSMD_SPEC_A,
  BSMD_SPEC_J,
  BSMD_SPEC_C,

  BSMD_STATE_BIAS,
  BSMD_STATE_Z = BSMD_STATE_BIAS,
  BSMD_STATE_V,
  BSMD_STATE_T,
  BSMD_STATE_BC,
  BSMD_STATE_MR,
  BSMD_STATE_ER,
  BSMD_STATE_BP,
  BSMD_STATE_SAP,
  BSMD_STATE_I,
  BSMD_STATE_Volt,
  BSMD_STATE_Hours,
  BSMD_STATE_Minutes,
  BSMD_STATE_Seconds,
  BSMD_STATE_Milliseconds,

  BSMD_HAND_BIAS,
  BSMD_HAND_B = BSMD_HAND_BIAS,
  BSMD_HAND_P,
  BSMD_HAND_R,
  BSMD_HAND_C,
  BSMD_PANEL,
  BSMD_SOUND,
  BSMD_DOOR,

  CTRL_HAND_R,
  CTRL_HAND_P,
  CTRL_HAND_B,
  CTRL_HAND_S,
  CTRL_BTN
};

enum KeypadS_Keys
{
  No,
  Sel,
  L,
  D,
  U,
  R
};

#define BIDS_BAUDRATE 115200
#define BTN_CHECK_TIME 100
const int Threshold_Zero = 10;

const int Val_AVE_No = 1023;
const int Val_AVE_Sel = 732;
const int Val_AVE_L = 514;
const int Val_AVE_D = 345;
const int Val_AVE_U = 144;
const int Val_AVE_R = 0;

const int Val_Thr_No_Sel = (Val_AVE_No + Val_AVE_Sel) / 2;
const int Val_Thr_Sel_L = (Val_AVE_Sel + Val_AVE_L) / 2;
const int Val_Thr_L_D = (Val_AVE_L + Val_AVE_D) / 2;
const int Val_Thr_D_U = (Val_AVE_D + Val_AVE_U) / 2;
const int Val_Thr_U_R = (Val_AVE_U + Val_AVE_R) / 2;

const int LED_DELAY_TIME = 100;

enum TR_BIDS_TEST_ERRORS
{
  UNKNOWN_ERROR,
  AddASSetting_DTypDNum_404,
  AddASSetting_AddAS_Failed,
  RmvASSetting_DTypDNum_404,
  RmvASSetting_RmvAS_Failed,
  BIDSCtrlAS_DChangeChk_RmvErr,
  BIDSCtrlAS_DChangeChk_AddErr,
  BIDSCtrlCR_Mode_Unknown,

};


//function declare
void CheckBTN();

//Check which button is pushed
KeypadS_Keys CheckBtn(double val);

//AutoSend
void BIDSCtrlAS();

//Call-Response
void BIDSCtrlCR();

void AddASSetting(BIDSEls Els, int OptNumArg = 0);
void RmvASSetting(BIDSEls Els, int OptNumArg = 0);

bool GetTypeAndDNum(BIDSEls bels, char *c, int *num, bool *IsINTData, int OptNumArg = 0);

//print the current mode name to LCD
void ModeDisp();

//Integer Number print to LCD
void ValuePrinterI(int v1, double v2);
//Floating Number print to LCD
void ValuePrinterF(int v1, double v2);

//Print ERROR Msg
void ErrorLED(unsigned char ec);
//print Error msg
void ErrorLED(TR_BIDS_TEST_ERRORS ec);
void lcdPrinter(int c, int r, const char *s);
void lcdPrinter(const char *s);
void lcdPrinter(int c, int r, String s);
