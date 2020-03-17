//TR.BIDS.libs.ino
//Under the MIT License
//Copyright 2020 Tetsu Otter

#include "TR.BIDS.libs.h"
#include <LiquidCrystal.h>
#include <stdlib.h>
#include "UFunc.h"

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

BIDS bids = BIDS(&Serial);

void setup()
{
  lcd.begin(16, 2);
  lcd.clear();
  lcdPrinter("TR.BIDS.libs.ino");
  delay(1000);
  lcdPrinter(0, 1, "init...");
  Serial.begin(230400);
  while (!Serial);
  if (!bids.AddAutoSend('E', 1, SpeedChanged))ErrorLED(0x01);
  if (!bids.AddAutoSend('E', 3, BCChanged))ErrorLED(0x02);
  if (!bids.AddAutoSend('E', 4, MRChanged))ErrorLED(0x03);
  lcdPrinter(0, 1, "init done!");
  delay(1000);
  lcd.clear();
  lcdPrinter("SPD 000.0 BC|MR");
  lcdPrinter(0, 1, "000.00 | 000.00");
}
bool isNonAS = false;
void loop()
{
  isNonAS = false;
  if (!bids.ASDataCheck(&isNonAS) && isNonAS) {
    lcd.clear();
    lcd.print(bids.LastCMD);
    while (true);
  }

}

void SpeedChanged(int valI, double valF) {
  char str[7];
  ZeroFill(str, 7);
  LD2CA(str, 0, 3, 1, valF);
  lcdPrinter(4, 0, str);
}
void BCChanged(int valI, double valF) {
  char str[7];
  ZeroFill(str, 7);
  LD2CA(str, 0, 3, 2, valF);
  lcdPrinter(0, 1, str);
}
void MRChanged(int valI, double valF) {
  char str[7];
  ZeroFill(str, 7);
  LD2CA(str, 0, 3, 2, valF);
  lcdPrinter(9, 1, str);
}

const int LED_DELAY_TIME = 200;
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
