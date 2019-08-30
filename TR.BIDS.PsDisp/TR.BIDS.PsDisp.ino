//TR.BIDS.PsDisp.ino ver001
//BIDS v202
//Copyright 2019 Tetsu Otter
//It's for ATS-Ps Display PCB by P-1
//Reference : https://garretlab.web.fc2.com/arduino/lab/7segment_led_with_shift_register/
//ATSPI : Integrated ATS Plugin (by @AskED757001@twitter.com)

//if you want to use LCD Keypad shield for debuging, set the number "1"
#define LCDMODE 0

#if LCDMODE == 1

#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#else
//ref : http://blog.davidecoppola.com/2013/08/using-a-shift-register-74hc595n-with-arduino/
#define DATA_PIN 11  //74HC595 : DS(14)
#define CLOCK_PIN 13 //74HC595 : SH_CP(11)
#define LATCH_PIN 10 //74HC595 : ST_CP(12)

#endif

#include "BIDS.h"
c_BIDS * BIDS;


#define LEDMOD_MAX 0b11111111
#define LEDMOD_HAF 0b01011010
#define PS_VOICE 0
#define WAIT_COUNT 50
#define LIGHT_HALF_SW A0

#define PS_PAT_OCCUR 0
#define PS_PAT_APPRO 1
#define PS_BR_BEHAVE 2
#define PS_BR_RELEAS 3
#define PS_BROKEN 4

#define PS_PAT_OCCUR_PNL 13
#define PS_PAT_APPRO_PNL 14
#define PS_BR_BEHAVE_PNL 15
#define PS_BR_RELEAS_PNL -1//undefined
#define PS_BROKEN_PNL -1//undefined
#define CURRENT_SPED_PNL 18
#define PS_PAT_SPEED_PNL 19

byte LEDMOD_print[5];
bool LEDMOD_TF[5];
byte LEDMOD_tf = 0;
bool led_isHalf = false;

int CurrentSpeedPNL = 0;
int PatternSpeedPNL = 0;

void setup() {
#if LCDMODE == 1
  lcd.begin(16, 2);
  //lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("ATS-Ps Disp");
#else
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
#endif

  BIDS = new c_BIDS(202, 115200);//BIDS Start
#if LCDMODE != 1
  #pragma region FuncTest
  Serial.print("FunctionTest Start\n");
  for (byte i = 0; i < 5 * 5; i++) {
    LEDControl(i << 3, false);
    SPIPrinter(LEDMOD_print, 5);
    delay(250);
    LEDControl(i << 3, true);
    SPIPrinter(LEDMOD_print, 5);
    delay(250);
  }
  LEDControl(0x00, false);
  SPIPrinter(LEDMOD_print, 5);
  Serial.print("FunctionTest Complete\n");
  #pragma endregion FuncTest
#endif
  //AutoSend Setting Start
  LEDMOD_TF[PS_PAT_OCCUR_PNL] = BIDS->DataGetInt("A", "P", PS_PAT_OCCUR_PNL) == 1;
  LEDMOD_TF[PS_PAT_APPRO_PNL] = BIDS->DataGetInt("A", "P", PS_PAT_APPRO_PNL) == 1;
  LEDMOD_TF[PS_BR_BEHAVE_PNL] = BIDS->DataGetInt("A", "P", PS_BR_BEHAVE_PNL) == 1;
  //BIDS->DataGet( "A", "P",PS_BR_RELEAS_PNL);
  //BIDS->DataGet( "A", "P",PS_BROKEN_PNL);
  CurrentSpeedPNL = BIDS->DataGetInt("A", "P", CURRENT_SPED_PNL);
  PatternSpeedPNL = BIDS->DataGetInt("A", "P", PS_PAT_SPEED_PNL);
  //AutoSend Setting End
}

void loop() {
  int CurrentSpeedPNLrec = CurrentSpeedPNL;
  int PatternSpeedPNLrec = PatternSpeedPNL;;
  bool isChanged = false;

  bool led_isHalf_rec = led_isHalf;
#if LCDMODE == 1
  int a0ar = analogRead(A0);
  if (500 < a0ar && a0ar < 750) {
    led_isHalf = !led_isHalf_rec;
    do {
      a0ar = analogRead(A0);
    } while (500 < a0ar && a0ar < 750);
    lcd.setCursor(12, 1);
    lcd.print(led_isHalf ? "HALF" : "FULL");
  }
#endif
  isChanged = led_isHalf != led_isHalf_rec;

  if (Serial.available() > 0) {
    while (Serial.available() > 0) {
      int i = 0;
      String s = BIDS->DataGet(i, "TRIP");
      if (s.length() >= 6) {
        s.replace("TRIP", "");
        s.remove(s.indexOf('X'));
        switch (s.toInt()) {
          case PS_PAT_OCCUR_PNL:
            LEDMOD_TF[PS_PAT_OCCUR] = i == 1;
            break;
          case PS_PAT_APPRO_PNL:
            LEDMOD_TF[PS_PAT_APPRO] = i == 1;
            break;
          case PS_BR_BEHAVE_PNL:
            LEDMOD_TF[PS_BR_BEHAVE] = i == 1;
            break;
          case CURRENT_SPED_PNL:
            CurrentSpeedPNL = i;
            break;
          case PS_PAT_SPEED_PNL:
            PatternSpeedPNL = i;
            break;
        }
      }
    }
  }

  //isChanged Judge
  for (int i = 0; i < 5; i++) {
    if (LEDMOD_TF[i] && (LEDMOD_print[i] == 0)) {
      LEDMOD_print[i] = 1;
      isChanged = true;
    } else if (!LEDMOD_TF[i] && (LEDMOD_print != 0)) {
      LEDMOD_print[i] = 0;
      isChanged = true;
    }
  }
  if (CurrentSpeedPNL != CurrentSpeedPNLrec) isChanged = true;
  if (PatternSpeedPNL != PatternSpeedPNLrec) isChanged = true;

  if (isChanged) {
    for (int i = 0; i < 5; i++)
      if (LEDMOD_print[i] > 0)
        LEDMOD_print[i] = led_isHalf ? LEDMOD_HAF : LEDMOD_MAX;

    SPIPrinter(LEDMOD_print, 5);
    LEDBarWriter(CurrentSpeedPNL, PatternSpeedPNL);

#if PS_VOICE == 1

#endif
  }
  delay(WAIT_COUNT);
}


void SPIPrinter(byte DataArr[], int Length) {
#if LCDMODE == 1
  lcd.setCursor(0, 1);
  char c = 'F';
  for (int i = 0; i < Length; i++) {
    switch (DataArr[i]) {
      case 0:
        c = 'F';
        break;
      case LEDMOD_HAF:
        c = 'H';
        break;
      case LEDMOD_MAX:
        c = 'T';
        break;
      default:
        c = '?';
        break;
    }
    lcd.print(c);
  }
  lcd.print("       ");
#else
  digitalWrite(LATCH_PIN, LOW);
  for (int i = 0; i < Length; i++) {
    byte buf = DataArr[i];
    Serial.print("writing [" + String(i) + "] : " + String(buf) + "\n");
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, buf);
  }
  digitalWrite(LATCH_PIN, HIGH);
#endif
}


void LEDControl(byte Data, bool LED_isHalf) {
  for (int i = 0; i < 5; i++) {
    byte buf = Data;
    LEDMOD_print[i] = LEDwriteNum(buf >> (7 - i), LED_isHalf);
  }
}

byte LEDwriteNum(byte Data, bool isHalf) {
  return (Data & 1) * (isHalf ? LEDMOD_MAX : LEDMOD_HAF);
}

void LEDBarWriter(int CurrentSPD, int PatternSPD) {
#if LCDMODE == 1
  String s = String(CurrentSPD);
  while (s.length() < 3)
    s = " " + s;
  lcd.setCursor(0, 0);
  lcd.print(s);
  lcd.print(" / ");
  s = String(PatternSPD);
  while (s.length() < 3)
    s = " " + s;
  lcd.print(s);
  lcd.print("        ");
#endif
}
/*void PlayVoice(byte Num, bool isLighting) {

  }*/
