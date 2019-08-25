//TR.BIDS.PsDisp.ino ver001
//BIDS v202
//Copyright 2019 Tetsu Otter
//It's for ATS-Ps Display PCB by P-1
//Reference : https://garretlab.web.fc2.com/arduino/lab/7segment_led_with_shift_register/
//ATSPI : Integrated ATS Plugin (by @AskED757001@twitter.com)

#include "SPI.h"

#include "BIDS.h"
c_BIDS * BIDS;


#define LEDMOD_MAX 0b11111111
#define LEDMOD_HAF 0b01011010
#define PS_VOICE 0
#define WAIT_COUNT 10
#define LIGHT_HALF_SW 2

#define PS_PAT_OCCUR 0
#define PS_PAT_APPRO 1
#define PS_BR_BEHAVE 2
#define PS_BR_RELEAS 3
#define PS_BROKEN 4

#define PS_PAT_OCCUR_PNL 12
#define PS_PAT_APPRO_PNL 13
#define PS_BR_BEHAVE_PNL 14
#define PS_BR_RELEAS_PNL 15
#define PS_BROKEN_PNL -1

SPISettings spiSettings = SPISettings(8000000, MSBFIRST, SPI_MODE0);

byte LEDMOD_print[5];
byte LEDMOD_tf = 0;

bool led_isHalf = false;
void setup() {
  BIDS = new c_BIDS();
  SPI.begin();
  SPI.beginTransaction(spiSettings);
  pinMode(LIGHT_HALF_SW, INPUT_PULLUP);
  BIDS->DataGet("A", "P", PS_PAT_OCCUR_PNL);
  BIDS->DataGet("A", "P", PS_PAT_APPRO_PNL);
  BIDS->DataGet("A", "P", PS_BR_BEHAVE_PNL);
  BIDS->DataGet("A", "P", PS_BR_RELEAS_PNL);
  //BIDS->DataGet("A","P",PS_BROKEN_PNL);
}

void loop() {
  bool isChanged = false;
  bool led_isHalf_rec = (digitalRead(LIGHT_HALF_SW) == LOW);
  isChanged = led_isHalf != led_isHalf_rec;
  if (Serial.available() > 0) {
    float data = 0;
    String s = BIDS->DataGet(data, "TRIP");
    if (s != "") {
      if (s.startsWith("TRIP" + String(PS_PAT_OCCUR_PNL))) {
        LEDMOD_print[PS_PAT_OCCUR] = (byte)data;
        isChanged = true;
      }
      if (s.startsWith("TRIP" + String(PS_PAT_APPRO_PNL))) {
        LEDMOD_print[PS_PAT_APPRO] = (byte)data;
        isChanged = true;
      }
      if (s.startsWith("TRIP" + String(PS_BR_BEHAVE_PNL))) {
        LEDMOD_print[PS_BR_BEHAVE] = (byte)data;
        isChanged = true;
      }
      if (s.startsWith("TRIP" + String(PS_BR_RELEAS_PNL))) {
        LEDMOD_print[PS_BR_RELEAS] = (byte)data;
        isChanged = true;
      }
      if (s.startsWith("TRIP" + String(PS_BROKEN_PNL))) {
        LEDMOD_print[PS_BROKEN] = (byte)data;
        isChanged = true;
      }
    }
  }
  if (isChanged) {
    for (int i = 0; i < 5; i++)
      if (LEDMOD_print[i] > 0)
        LEDMOD_print[i] = led_isHalf ? LEDMOD_HAF : LEDMOD_MAX;

    SPIPrinter(LEDMOD_print, 5);

#ifdef PS_VOICE 1

#endif
  }
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
