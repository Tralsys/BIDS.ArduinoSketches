//TR.BIDS.TimeOnLCD.ino
//Under the MIT License
//Copyright 2020 Tetsu Otter
//#define AUTOSEND

#include <Arduino.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void setup()
{
  lcd.begin(16, 2);
  lcd.flush();
  lcd.print("pls wait...");
  delay(500);
  lcd.setCursor(0, 0);
  lcd.print("initializing..");
  Serial.begin(19200);
  while (!Serial);
  Serial.setTimeout(1);
#ifdef AUTOSEND
  ASSet(10);//Hour
  ASSet(11);//Min
  ASSet(12);//Sec
#endif
  lcd.setCursor(0, 0);
  lcd.print("init complete.");
  delay(500);
  lcd.clear();
  lcd.print("  :  :  ");
}
int HourRec = -1;
int MinRec = -1;
int SecRec = -1;
void loop()
{
  int h = HourRec;
  int m = MinRec;
  int sec = SecRec;

#ifdef AUTOSEND
  while (Serial.available() <= 0);
  char c[32];
  ZeroFill(c, 32);
  if (Serial.readBytesUntil('\n', c, 32) < 4) ErrorLED(0x03);

  String s = String(c);


  if (s.startsWith("TRIE10X")) {
    s.remove(0, 7);
    h = s.toInt();
  }
  if (s.startsWith("TRIE11X")) {
    s.remove(0, 7);
    m = s.toInt();
  }
  if (s.startsWith("TRIE12X")) {
    s.remove(0, 7);
    sec = s.toInt();
  }

  lcd.setCursor(0, 1);
  lcd.print(">               ");
  lcd.setCursor(1, 1);
  lcd.print(s);

#else
  if (!BIDSReqInt("TRIE10\n", &h))ErrorLED(10);//Hour
  if (!BIDSReqInt("TRIE11\n", &m))ErrorLED(11);//Min
  if (!BIDSReqInt("TRIE12\n", &sec))ErrorLED(12);//Sec
#endif

  if (HourRec != h) {
    lcd.setCursor(0, 0);
    lcd.print((h / 10) % 10);
    lcd.setCursor(1, 0);
    lcd.print(h % 10);
  }
  if (MinRec != m) {
    lcd.setCursor(3, 0);
    lcd.print((m / 10) % 10);
    lcd.setCursor(4, 0);
    lcd.print(m % 10);
  }
  if (SecRec != sec) {
    lcd.setCursor(6, 0);
    lcd.print((sec / 10) % 10);
    lcd.setCursor(7, 0);
    lcd.print(sec % 10);
  }

}

void ASSet(int num) {
  char c[10];
  ZeroFill(c, 10);
  String s = "TRAE" + String(num) + "\n";

  lcd.setCursor(0, 1);
  lcd.print("set:");
  lcd.print(s.c_str());
  Serial.print(s.c_str());
  while (Serial.available() <= 0);
  if (Serial.readBytesUntil('\n', c, 10) < 4) ErrorLED(0x02);

  lcd.setCursor(0, 1);
  lcd.print("get:");
  s = String(c);
  lcd.print(s);
  delay(500);
  if (s != "TRAE" + String(num) + "X0") ErrorLED(0x01);
}

const int LED_DELAY_TIME = 100;
void ErrorLED(byte ec) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Error Occured");
  lcd.setCursor(0, 1);
  lcd.print("CODE: " + String(ec));
  bool Is1 = false;
  while (true) {
    for (int i = 0; i < 2; i++) {
      digitalWrite(13, HIGH);
      delay(LED_DELAY_TIME / 2);
      digitalWrite(13, LOW);
      delay(LED_DELAY_TIME / 2);
    }
    for (int i = 0; i < 10; i++) {
      if (i < 8) {
        digitalWrite(13, HIGH);
        delay(LED_DELAY_TIME);
        if ((ec >> i) > 0) delay(LED_DELAY_TIME);
      }
      digitalWrite(13, LOW);
      delay(LED_DELAY_TIME);
    }
  }
}

void ZeroFill(char *c, int len) {
  for (int i = 0; i < len; i++)
    c[i] = 0;
}


bool BIDSReqInt(char* cmd, int * val) {
  int i = 0;
  Serial.print(cmd);
  while (Serial.available() <= 0);
  char c[64];
  ZeroFill(c, 64);
  String s;
  if (Serial.readBytesUntil('\n', c, 64) < 4) ErrorLED(0x03);
  
  for (i = 0; i < 4; i++)
    if (cmd[i] != c[i])return false;
  i = 0;
  while (c[i] != 'X' && c[i] != '\n' && cmd[i] != '\0') i++;
  
  s = String(c);
  s.remove(0, i + 1);
  *val = s.toInt();
  return true;
}
