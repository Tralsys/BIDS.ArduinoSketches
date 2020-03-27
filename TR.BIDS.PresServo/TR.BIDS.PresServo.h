//TR.BIDS.PresServo.h
//Copyright 2020 Tetsu Otter
//Under the MIT License
#define TRUE 1
#define FALSE 0

#define DEBUG_SERVO
#define DEBUG_SERVO_STEP 5
#define DEBUG_SERVO_INTERVAL 100

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

/*Function Declare*/
#ifdef SERVO_PIN_1
void DriveServo1(int iv, double dv);
#endif
#ifdef SERVO_PIN_2
void DriveServo2(int iv, double dv);
#endif

#ifdef DEBUG
void DSDBG(int ServoNum, int iv, double dv, double SvDeg, double DegRec, double Threshold, bool ThresholdEnabled);
#endif

#if THRESHOLD_ENABLED == TRUE
bool NeedToDrive(double CurrentDeg, double DegRec, double Threshold_Deg);
#endif

#ifdef DEBUG_SERVO
void DebugServo();
#endif