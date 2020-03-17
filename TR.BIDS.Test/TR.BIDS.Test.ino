//TR.BIDS.Test.ino
//Under the MIT License
//Copyright 2020 Tetsu Otter
//****************************************
//This sketch needs Arduino Keypad Shield.
//****************************************

#include "TR.BIDS.libs.h"
#include <Arduino.h>
#include <stdio.h>
#include <LiquidCrystal.h>
#include "TR.BIDS.Test.defs.h"

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
bool IsASMode = true;
bool IsASModeRec = false;
HardwareSerial hs;
short BtnVals[9];
bool Pushed = false;
BIDS bids = BIDS(&hs);
BIDSEls ModeNum = BSMD_IS_ENABLE;
BIDSEls ModeRec = None;
void setup()
{
  lcd.begin(16, 2);
  lcd.clear();
  lcdPrinter(__FILE__);
  lcdPrinter(0, 1, "init...");
  hs.begin(230400);
  while (!hs)
    ;
  lcdPrinter(0, 1, "serial began");
  delay(500);
  lcd.clear();
}

void loop()
{
  CheckBTN();
  ModeDisp();
  BIDSCtrlAS();
  BIDSCtrlCR();
  ModeRec = ModeNum;
  IsASModeRec = IsASMode;
}
const int Threshold_Zero = 10;
const int Val_AVE_Sel = 0;
const int Val_AVE_L = 0;
const int Val_AVE_D = 0;
const int Val_AVE_U = 0;
const int Val_AVE_R = 0;
void CheckBTN()
{
  int cval = analogRead(A0);
  if (cval < Threshold_Zero)
  {
    if (!Pushed)
      return;
    switch (CheckBtn(0))
    {
    case KeypadS_Keys::Sel:
      break;
    case KeypadS_Keys::L:
      break;
    case KeypadS_Keys::D:
      break;
    case KeypadS_Keys::U:
      break;
    case KeypadS_Keys::R:
      break;
    }
  }
}
KeypadS_Keys CheckBtn(int val)
{
  if (val > Val_AVE_R)
    return KeypadS_Keys::R;
  if (val > Val_AVE_U)
    return KeypadS_Keys::U;
  if (val > Val_AVE_D)
    return KeypadS_Keys::D;
  if (val > Val_AVE_L)
    return KeypadS_Keys::L;
  if (val > Val_AVE_Sel)
    return KeypadS_Keys::Sel;
  return KeypadS_Keys::No;
}
void BIDSCtrlAS()
{
  if (IsASMode != IsASModeRec && !IsASMode)
  {
    RmvASSetting(ModeNum);
    return;
  }
}
void BIDSCtrlCR()
{
  if (IsASMode)
    return;
}
void RmvASSetting(BIDSEls RmvEls)
{
}
void ModeDisp()
{
  if (ModeNum == ModeRec)
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

void ValuePrinterI(int v1, double v2)
{
  char c[6];

  snprintf(c, 6, "%d", v1);

  lcdPrinter(10, 0, c);
}
void ValuePrinterF(int v1, double v2)
{
  char c[6];

  snprintf(c, 6, "%ld", v2);

  lcdPrinter(10, 0, c);
}

const int LED_DELAY_TIME = 100;
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
void ZeroFill(char *c, int len)
{
  for (int i = 0; i < len; i++)
    c[i] = 0;
}
