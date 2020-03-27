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
const int Threshold_Zero = 10;
const int Val_AVE_Sel = 0;
const int Val_AVE_L = 0;
const int Val_AVE_D = 0;
const int Val_AVE_U = 0;
const int Val_AVE_R = 0;
const int LED_DELAY_TIME = 100;

enum TR_BIDS_TEST_ERRORS
{
  UNKNOWN_ERROR,
  AddASSetting_DTypDNum_404,
  AddASSetting_AddAS_Failed,
  RmvASSetting_DTypDNum_404,
  RmvASSetting_RmvAS_Failed,
}