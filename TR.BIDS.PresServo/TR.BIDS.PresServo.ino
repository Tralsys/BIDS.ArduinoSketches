//TR.BIDS.PresServo.ino
//Copyright 2020 Tetsu Otter
//Under the MIT License
#include <TR.BIDS.libs.h>
#include <TR.BIDS.defs.h>
#include <Servo.h>
#include "TR.BIDS.PresServo.h"

BIDS bids = BIDS(&Serial);
#ifdef SERVO_PIN_1
Servo servo1;
#endif
#ifdef SERVO_PIN_2
Servo servo2;
#endif

void setup()
{
#ifdef DEBUG_SERVO
  DebugServo();
#endif

  Serial.begin(115200); //BaudRate 115200
  while (!Serial)
    ; //for leonardo etc.

#ifdef DEBUG
  Serial.println("TR.BIDS.PresServo.ino DEBUG_MODE");
  delay(1000);
#endif

#ifdef SERVO_PIN_1
  servo1.attach(SERVO_PIN_1);
  if (bids.AddAutoSend(DTYPE_ELAPD, USING_DNUM_1, DriveServo1))
    Serial.println("Servo1 AddAS Error");
#endif
#ifdef SERVO_PIN_2
  servo2.attach(SERVO_PIN_2);
  if (bids.AddAutoSend(DTYPE_ELAPD, USING_DNUM_2, DriveServo2))
    Serial.println("Servo2 AddAS Error");
#endif
}
void loop()
{
  bool NoASDataGot = false;
  bids.ASDataCheck(&NoASDataGot);
  if (NoASDataGot)
  {
    Serial.print("NonASDataGot >>> ");
    Serial.println(bids.LastCMD);
    if (bids.LastCMD[0] == 'T' && bids.LastCMD[1] == 'R' && bids.LastCMD[2] == CMD_ERROR)
      delay(1000);
  }
}

#ifdef SERVO_PIN_1
double Servo1DegRec = -10;
void DriveServo1(int iv, double dv)
{
  double SvDeg = dv * Deg_Per_kPa1; //Degree Calc

#ifdef DEBUG
  DSDBG(1, iv, dv, SvDeg, Servo1DegRec, Threshold1_Deg, ENABLE_THRESHOLD1);
#endif

#if ENABLE_THRESHOLD1 == TRUE
  if (!NeedToDrive(SvDeg, Servo1DegRec, Threshold1_Deg))
    return;
#endif

  servo1.write(SvDeg);  //Drive Servo
  Servo1DegRec = SvDeg; //Record Current Position
}
#endif

#ifdef SERVO_PIN_2
double Servo2DegRec = -10;
void DriveServo2(int iv, double dv)
{
  double SvDeg = dv * Deg_Per_kPa2; //Degree Calc

#ifdef DEBUG
  DSDBG(2, iv, dv, SvDeg, Servo2DegRec, Threshold2_Deg, ENABLE_THRESHOLD1);
#endif

#if ENABLE_THRESHOLD2 == TRUE
  if (!NeedToDrive(SvDeg, Servo2DegRec, Threshold2_Deg))
    return;
#endif

  servo2.write(SvDeg);  //Drive Servo
  Servo2DegRec = SvDeg; //Record Current Position
}
#endif

#ifdef DEBUG
void DSDBG(int ServoNum, int iv, double dv, double SvDeg, double DegRec, double Threshold, bool ThresholdEnabled)
{
  Serial.print("DriveServo");
  Serial.print(ServoNum);
  Serial.print("  >>> iv : ");
  Serial.print(iv);
  Serial.print("\tdv:");
  Serial.print(dv);
  Serial.print("\tSvDeg:");
  Serial.print(SvDeg);
  Serial.print("\tDegRec:");
  Serial.print(DegRec);

  if (ThresholdEnabled)
  {
#if THRESHOLD_ENABLED == TRUE
    Serial.print("\tThreshold:");
    Serial.print(Threshold);
    Serial.print("\tNeedToDrive:");
    Serial.print(NeedToDrive(SvDeg, DegRec, Threshold));
#endif
  }
  else
  {
    Serial.print("ThresholdEnabled:false");
  }

  Serial.println();
}
#endif

#if THRESHOLD_ENABLED == TRUE
bool NeedToDrive(double CurrentDeg, double DegRec, double Threshold_Deg)
{
  return (CurrentDeg < (DegRec - Threshold_Deg) || (DegRec + Threshold_Deg) < CurrentDeg);
}
#endif

#ifdef DEBUG_SERVO
void DebugServo()
{

#ifdef SERVO_PIN_1
  servo1.attach(SERVO_PIN_1);
  servo1.write(90);
  delay(1000);
  servo1.write(0);
#endif
#ifdef SERVO_PIN_2
  servo2.attach(SERVO_PIN_2);
  servo2.write(90);
  delay(1000);
  servo2.write(0);
#endif

#ifdef DEBUG
  Serial.begin(115200);
  while (!Serial)
    ;
#endif
  delay(1000);
  while (true)
  {
#ifdef SERVO_PIN_1
    for (int i = 0; i <= 1000; i += DEBUG_SERVO_STEP)
    {
      DriveServo1(i, (double)i);
      delay(DEBUG_SERVO_INTERVAL);
    }
    for (int i = 1000; i >= 0; i -= DEBUG_SERVO_STEP)
    {
      DriveServo1(i, (double)i);
      delay(DEBUG_SERVO_INTERVAL);
    }
#endif
#ifdef SERVO_PIN_2
    for (int i = 0; i <= 1000; i += DEBUG_SERVO_STEP)
    {
      DriveServo2(i, (double)i);
      delay(DEBUG_SERVO_INTERVAL);
    }
    for (int i = 1000; i >= 0; i -= DEBUG_SERVO_STEP)
    {
      DriveServo2(i, (double)i);
      delay(DEBUG_SERVO_INTERVAL);
    }
#endif
  }
}
#endif
