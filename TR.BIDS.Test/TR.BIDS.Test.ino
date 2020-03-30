//TR.BIDS.Test.ino
//Under the MIT License
//Copyright 2020 Tetsu Otter
//****************************************
//This sketch needs Arduino Keypad Shield.
//****************************************

#include <TR.BIDS.libs.h>
#include <TR.BIDS.defs.h>
#include <Arduino.h>
#include <stdio.h>
#include <LiquidCrystal.h>
#include "TR.BIDS.Test.defs.h"
#include "UFunc.h"

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
bool IsASMode = true;
bool IsASModeRec = false;
int OptNum = 0;
int OptRec = 0;
int OptNum_MAX = 256;
BIDS bids = BIDS(&Serial);
KeypadS_Keys KeyStateRec = KeypadS_Keys::No;
KeypadS_Keys CurKeyState = KeypadS_Keys::No;
BIDSEls ModeNum = BSMD_IS_ENABLE;
BIDSEls ModeRec = None;
void setup()
{
  //variable init
  IsASMode = true;
  IsASModeRec = false;
  OptNum = 0;
  OptRec = 0;
  OptNum_MAX = 256;
  KeyStateRec = KeypadS_Keys::No;
  CurKeyState = KeypadS_Keys::No;
  ModeNum = BSMD_IS_ENABLE;
  ModeRec = None;

  //lcd init
  lcd.begin(16, 2);
  lcd.clear();

  lcdPrinter("Test.ino");
  lcdPrinter(0, 1, "init...");
  Serial.begin(BIDS_BAUDRATE);
  while (!Serial)
    delay(1);
  lcdPrinter(0, 1, "serial began");
  delay(500);
  lcd.clear();
}

void loop()
{
  CheckBTN();
  ModeDisp();
  BIDSCtrlAS(); //AutoSend
  BIDSCtrlCR(); //Call-Response

  ModeRec = ModeNum;
  OptRec = OptNum;
  IsASModeRec = IsASMode;
}
int BtnChkValCount = 0;
long BtnVal = 0;
void CheckBTN()
{
  BtnVal += analogRead(A0);
  BtnChkValCount++;
  if (BtnChkValCount < BTN_CHECK_TIME)
    return;

  CurKeyState = CheckBtn((double)BtnVal / BTN_CHECK_TIME);

  if (KeyStateRec != CurKeyState && CurKeyState == KeypadS_Keys::No) //OnKeyReleased
  {
    switch (KeyStateRec)
    {
      case KeypadS_Keys::Sel:
        IsASMode = !IsASMode;
        break;
      case KeypadS_Keys::L: //DataType Ctrl
        if (ModeNum > BIDSEls::BSMD_IS_ENABLE)
          ModeNum = (BIDSEls)((int)ModeNum - 1);
        break;
      case KeypadS_Keys::D: //OptNum Ctrl
        if (OptNum > 0)
          OptNum--;
        break;
      case KeypadS_Keys::U: //OptNum Ctrl
        if (OptNum < OptNum_MAX)
          OptNum++;
        break;
      case KeypadS_Keys::R: //DataType Ctrl
        if (ModeNum < BIDSEls::CTRL_BTN)
          ModeNum = (BIDSEls)((int)ModeNum + 1);
        break;
    }
  }
  KeyStateRec = CurKeyState;
  BtnChkValCount = 0;
  BtnVal = 0;
}

//Check which button is pushed
KeypadS_Keys CheckBtn(double val)
{
  if (val < Val_Thr_U_R)
    return KeypadS_Keys::R;
  if (val < Val_Thr_D_U)
    return KeypadS_Keys::U;
  if (val < Val_Thr_L_D)
    return KeypadS_Keys::D;
  if (val < Val_Thr_Sel_L)
    return KeypadS_Keys::L;
  if (val < Val_Thr_No_Sel)
    return KeypadS_Keys::Sel;
  return KeypadS_Keys::No;
}

//AutoSend
void BIDSCtrlAS()
{
  switch (ModeNum) //Can use AS Mode Check
  {
    case BSMD_IS_ENABLE:
    case BSMD_VERSION_NUM:
    case CTRL_HAND_R:
    case CTRL_HAND_P:
    case CTRL_HAND_B:
    case CTRL_HAND_S:
    case CTRL_BTN:
      if (IsASModeRec) RmvASSetting(ModeRec);
      return;
  }

  if (IsASMode != IsASModeRec) //Turn to Other Mode
  {
    if (IsASMode) //Turn to AS Mode from CR Mode
      AddASSetting(ModeNum);
    else //Prepare for Turning to CR Mode
    {

      RmvASSetting(ModeNum);
      return;
    }
  }

  //Data IsChanged Check
  if (ModeNum != ModeRec)
  {
    RmvASSetting(ModeRec, OptRec);
    AddASSetting(ModeNum, OptNum);
  }

  bool NoASCMD;
  if (IsASMode)
    bids.ASDataCheck(&NoASCMD);
}

//Call-Response
void BIDSCtrlCR()
{
  if (IsASMode)
    return;

  const int ca_len = 32;
  char ca[ca_len];
  switch (ModeNum)
  {
    case BSMD_IS_ENABLE: //Unimplemented in this sketch
      break;
    case BSMD_VERSION_NUM: //Unimplemented in this sketch
      break;
    case CTRL_HAND_R: //Unimplemented in this sketch
      break;
    case CTRL_HAND_P: //Unimplemented in this sketch
      break;
    case CTRL_HAND_B: //Unimplemented in this sketch
      break;
    case CTRL_HAND_S: //Unimplemented in this sketch
      break;
    case CTRL_BTN: //Unimplemented in this sketch
      break;
    default:
      char c;
      int num;
      bool IsIntD;
      if (!GetTypeAndDNum(ModeNum, &c, &num, &IsIntD, OptNum))
        ErrorLED(TR_BIDS_TEST_ERRORS::BIDSCtrlCR_Mode_Unknown);

      if (IsIntD) {
        int val = 0;
        if (bids.CmdSender(ca, &val))
          ValuePrinterI(val, 0);
      }
      else {
        double val = 0;
        if (bids.CmdSender(ca, &val))
          ValuePrinterI(0, val);
      }
      break;
  }
}

void AddASSetting(BIDSEls Els, int OptNumArg = 0)
{
  char DTyp = 0;
  int DNum = 0;
  bool IsIntD = false;
  if (!GetTypeAndDNum(Els, &DTyp, &DNum, &IsIntD, OptNumArg)) {
    ErrorLED(TR_BIDS_TEST_ERRORS::AddASSetting_DTypDNum_404);
  }

  if (!bids.AddAutoSend(DTyp, DNum, IsIntD ? ValuePrinterI : ValuePrinterF))
    ErrorLED(TR_BIDS_TEST_ERRORS::AddASSetting_AddAS_Failed);
}
void RmvASSetting(BIDSEls Els, int OptNumArg = 0)
{
  switch (ModeRec) //LastMode is in ASList Check
  {
    case BSMD_IS_ENABLE:
    case BSMD_VERSION_NUM:
    case CTRL_HAND_R:
    case CTRL_HAND_P:
    case CTRL_HAND_B:
    case CTRL_HAND_S:
    case CTRL_BTN:
    case None:
      return; //Not in ASList
  }
  char DTyp = 0;
  int DNum = 0;
  bool IsIntD = false;
  if (!GetTypeAndDNum(Els, &DTyp, &DNum, &IsIntD, OptNumArg))
    ErrorLED(TR_BIDS_TEST_ERRORS::AddASSetting_DTypDNum_404);

  if (!bids.RmvAutoSend(DTyp, DNum, IsIntD ? ValuePrinterI : ValuePrinterF))
    ErrorLED(TR_BIDS_TEST_ERRORS::RmvASSetting_RmvAS_Failed);
}

bool GetTypeAndDNum(BIDSEls bels, char *c, int *num, bool *IsINTData, int OptNumArg = 0)
{
  if (bels == BIDSEls::None || bels > BIDSEls::CTRL_BTN)
    return false;

  if (bels == BIDSEls::BSMD_IS_ENABLE || bels == BIDSEls::BSMD_VERSION_NUM)
    return false; //It is not InfoData

  if (BSMD_SPEC_B <= bels && bels <= BSMD_SPEC_C) //Spec Data
  {
    *c = DTYPE_CONSTD;
    *num = (int)(bels - BIDSEls::BSMD_SPEC_BIAS);
    *IsINTData = true;
    return true;
  }

  if (BSMD_STATE_Z <= bels && bels <= BSMD_STATE_Milliseconds) //State Data
  {
    *c = DTYPE_ELAPD;
    *num = (int)(bels - BIDSEls::BSMD_STATE_BIAS);
    *IsINTData = false;
    if (bels == BSMD_STATE_T || (BSMD_STATE_Hours <= bels && bels <= BSMD_STATE_Milliseconds))
      *IsINTData = true;
    return true;
  }

  if (BSMD_HAND_B <= bels && bels <= BSMD_HAND_C) //Handle Position Data
  {
    *c = DTYPE_HANDPOS;
    *num = (int)(bels - BIDSEls::BSMD_HAND_BIAS);
    *IsINTData = true;
    return true;
  }

  if (bels == BSMD_PANEL || bels == BSMD_SOUND) //Panel or Sound Data
  {
    *c = bels == BSMD_PANEL ? 'P' : 'S';
    *num = OptNumArg;
    *IsINTData = true;
    return true;
  }
  if (bels == BSMD_DOOR) //Door State Data
  {
    *c = DTYPE_DOOR;
    *num = 0;
    *IsINTData = true;
    return true;
  }

  return false;
}

//print the current mode name to LCD
void ModeDisp()
{
  if (ModeNum == ModeRec) //If it is not changed, ModeName Change is unneeded.
    return;

  switch (ModeNum)
  {
    case BSMD_IS_ENABLE:
      return lcdPrinter("IsEnabled ");
    case BSMD_VERSION_NUM:
      return lcdPrinter("Version   ");
    case BSMD_SPEC_B:
      return lcdPrinter("Spec.B    ");
    case BSMD_SPEC_P:
      return lcdPrinter("Spec.P    ");
    case BSMD_SPEC_A:
      return lcdPrinter("Spec.A    ");
    case BSMD_SPEC_J:
      return lcdPrinter("Spec.J    ");
    case BSMD_SPEC_C:
      return lcdPrinter("Spec.C    ");
    case BSMD_STATE_Z:
      return lcdPrinter("State.Z   ");
    case BSMD_STATE_V:
      return lcdPrinter("State.V   ");
    case BSMD_STATE_T:
      return lcdPrinter("State.T   ");
    case BSMD_STATE_BC:
      return lcdPrinter("State.BC  ");
    case BSMD_STATE_MR:
      return lcdPrinter("State.MR  ");
    case BSMD_STATE_ER:
      return lcdPrinter("State.ER  ");
    case BSMD_STATE_BP:
      return lcdPrinter("State.BP  ");
    case BSMD_STATE_SAP:
      return lcdPrinter("State.SAP ");
    case BSMD_STATE_I:
      return lcdPrinter("State.I   ");
    case BSMD_STATE_Volt:
      return lcdPrinter("State.Vlt ");
    case BSMD_STATE_Hours:
      return lcdPrinter("State.Hour");
    case BSMD_STATE_Minutes:
      return lcdPrinter("State.Min ");
    case BSMD_STATE_Seconds:
      return lcdPrinter("State.Sec ");
    case BSMD_STATE_Milliseconds:
      return lcdPrinter("State.MSec");
    case BSMD_HAND_B:
      return lcdPrinter("Current B ");
    case BSMD_HAND_P:
      return lcdPrinter("Current P ");
    case BSMD_HAND_R:
      return lcdPrinter("Current R");
    case BSMD_HAND_C:
      return lcdPrinter("Const SPD");
    case BSMD_PANEL:
      return lcdPrinter("Panel    ");
    case BSMD_SOUND:
      return lcdPrinter("Sound    ");
    case BSMD_DOOR:
      return lcdPrinter("Door     ");
    case CTRL_HAND_R:
      return lcdPrinter("Ctrl R   ");
    case CTRL_HAND_P:
      return lcdPrinter("Ctrl P   ");
    case CTRL_HAND_B:
      return lcdPrinter("Ctrl B   ");
    case CTRL_HAND_S:
      return lcdPrinter("Ctrl S   ");
    case CTRL_BTN:
      return lcdPrinter("Ctrl Btn");
  }
}

//Integer Number print to LCD
void ValuePrinterI(int v1, double v2)
{
  char c[6];

  snprintf(c, 6, "%6d", v1);

  lcdPrinter(10, 0, c);
}
//Floating Number print to LCD
void ValuePrinterF(int v1, double v2)
{
  char c[6];

  LD2CA(c, 0, 6, 1, v2);

  lcdPrinter(10, 0, c);
}

//Print ERROR Msg
void ErrorLED(unsigned char ec)
{
  lcd.clear();
  lcdPrinter(0, 0, "Error Occured");
  lcdPrinter(0, 1, "CODE: " + String(ec));

  bool Is1 = false;
  while (true)
  {
    for (int i = 0; i < 2; i++)
    {
      digitalWrite(13, HIGH);
      delay(LED_DELAY_TIME / 2);
      digitalWrite(13, LOW);
      delay(LED_DELAY_TIME / 2);
    }
    for (int i = 0; i < 10; i++)
    {
      if (i < 8)
      {
        digitalWrite(13, HIGH);
        delay(LED_DELAY_TIME);
        if ((ec >> i) > 0)
          delay(LED_DELAY_TIME);
      }
      digitalWrite(13, LOW);
      delay(LED_DELAY_TIME);
    }
  }
}
//print Error msg
void ErrorLED(TR_BIDS_TEST_ERRORS ec)
{
  ErrorLED((unsigned char)ec);
}
void lcdPrinter(int c, int r, const char *s)
{
  lcd.setCursor(c, r);
  lcd.print(s);
}
void lcdPrinter(const char *s)
{
  lcdPrinter(0, 0, s);
}
void lcdPrinter(int c, int r, String s)
{
  lcd.setCursor(c, r);
  lcd.print(s);
}
