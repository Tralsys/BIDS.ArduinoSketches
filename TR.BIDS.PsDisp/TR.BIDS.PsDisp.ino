//TR.BIDS.PsDisp.ino ver001
//BIDS v202
//Copyright 2019 Tetsu Otter
//Updated : 20190808
//It's for ATS-Ps Display PCB by P-1
//Reference : https://garretlab.web.fc2.com/arduino/lab/7segment_led_with_shift_register/

#include "SPI.h"

#define LEDMOD_MAX 0b11111111
#define LEDMOD_HAF 0b01011010
#define PS_VOICE 0
#define WAIT_COUNT 80000

//#define PS_PAT_OCCUR 0
//#define PS_PAT_APPRO 1
//#define PS_BR_BEHAVE 2
//#define PS_BR_RELEAS 3
//#define PS_BROKEN 4

SPISettings spiSettings = SPISettings(8000000, MSBFIRST, SPI_MODE0);

byte LEDMOD_print[5];
byte LEDMOD_tf = 0;

bool led_isHalf = false;
void setup() {
  //BIDSSerial.Begin();
  SPI.begin();
  SPI.beginTransaction(spiSettings);
  Serial.begin(9600);
  #pragma region FunctionTest
  Serial.println("Function Test Start");
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
  Serial.println("Function Test Complete");
  #pragma endregion
}

void loop() {
  byte buf = LEDMOD_tf;
  //LEDMOD_tf = (byte)random(0, 256);
  int prN = Serial.readStringUntil('\n').toInt();

  LEDMOD_tf = 0;
  led_isHalf = prN & 1 == 1;
  for (int i = 0; i < 5; i++) {
    prN /= 10;
    LEDMOD_tf += prN & 1;
    LEDMOD_tf << 1;
  }
  LEDMOD_tf << 3;

  LEDControl(LEDMOD_tf, false);
  SPIPrinter(LEDMOD_print, 5);

#ifdef PS_VOICE 1
  for (int i = 0; i < 5; i++)
    if ((((buf & LEDMOD_tf) >> (7 - i)) & 1) == 0)
      PlayVoice(i, ((LEDMOD_tf >> (7 - i)) & 1) == 1);
#endif

  delay(WAIT_COUNT);
}

void LEDControl(byte Data, bool LED_isHalf) {
  for (int i = 0; i < 5; i++)
    LEDMOD_print[i] = LEDwriteNum(Data >> (7 - i), LED_isHalf);
}

void SPIPrinter(byte DataArr[], int Length) {
  digitalWrite(SS, LOW);
  for (int i = 0; i < 5; i++)
    SPI.transfer(SPIPrinter[i]);
  digitalWrite(SS, HIGH);

}

byte LEDwriteNum(byte Data, bool isHalf) {
  return (Data & 1) * (isHalf ? LEDMOD_MAX : LEDMOD_HAF);
}


void PlayVoice(byte Num, bool isLighting) {

}
