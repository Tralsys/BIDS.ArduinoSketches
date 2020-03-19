//TR.BIDS.PresServo.ino
//Copyright 2020 Tetsu Otter
//Under the MIT License
#include "TR.BIDS.libs.h"
#include "TR.BIDS.defs.h"
#include <Servo.h>

#define TRUE 1
#define FALSE 0

#define DEBUG

#define USING_DNUM_1 DNUM_ELAPD_BC_PRES
#define USING_DNUM_2 DNUM_ELAPD_MR_PRES

#define SERVO_PIN_1 8
#define SERVO_PIN_2 9

const double Deg_Per_kPa1 = 1 / 6.4;
const double Deg_Per_kPa2 = Deg_Per_kPa1;

#define ENABLE_THRESHOLD1 TRUE
const double Threshold1_Deg = 0.5;

#define ENABLE_THRESHOLD2 TRUE
const double Threshold2_Deg = 0.5;

#define THRESHOLD_ENABLED ((ENABLE_THRESHOLD1 == TRUE) || (ENABLE_THRESHOLD2 == TRUE))

BIDS bids = BIDS(&Serial);
Servo servo1;
Servo servo2;

void setup()
{
  Serial.begin(115200); //BaudRate 115200
  while (!Serial)
    ; //for leonardo etc.

#ifdef DEBUG
  Serial.println("TR.BIDS.PresServo.ino DEBUG_MODE");
  delay(1000);
#endif
  servo1.attach(SERVO_PIN_1);
  servo2.attach(SERVO_PIN_2);

  bids.AddAutoSend(DTYPE_ELAPD, USING_DNUM_1, DriveServo1);
  bids.AddAutoSend(DTYPE_ELAPD, USING_DNUM_2, DriveServo2);
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

double Servo1DegRec = -10;
double Servo2DegRec = -10;
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

  servo1.write(SvDeg); //Drive Servo
  Servo1DegRec = SvDeg;       //Record Current Position
}

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

  servo2.write(SvDeg); //Drive Servo
  Servo1DegRec = SvDeg;       //Record Current Position
}

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
