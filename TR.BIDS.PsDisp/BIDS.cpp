//BIDS.h
//Base : stop-pattern
//v202 Support : TetsuOtter
#define DELAY_TIME 10

#include <arduino.h>
#include "BIDS.h"
#define INITIAL_BAUDRATE 19200
#define TARGET_VERSION 202

c_BIDS::c_BIDS() {
  this->Version = VersionCheck(TARGET_VERSION, INITIAL_BAUDRATE);
}
c_BIDS::c_BIDS(int v = TARGET_VERSION) {
  this->Version = VersionCheck(v, INITIAL_BAUDRATE);
}
c_BIDS::c_BIDS(int v = TARGET_VERSION, long baudRate = INITIAL_BAUDRATE) {
  this->Version = VersionCheck(v, baudRate);
}

float c_BIDS::SerialGet(String Command) {
  SerialSender(Command);

  float retFlo = 0;
  return (DataGet(retFlo, Command + "X") != "") ? retFlo : 0;
}

void c_BIDS::SerialSender(String Command) {
  unsigned long sendtime;
  int cnt = 0;
  Serial.print(Command);
  Serial.print("\n");
  Serial.flush();
  sendtime = millis();
  while (Serial.available() <= 0) {
    //failed to get data (after 1 minute)
    if (cnt >= 6) return 0;

    //retransmission (every 10 seconds)
    if (millis() - sendtime > 10000) {
      Serial.print(Command);
      Serial.print("\n");
      Serial.flush();
      sendtime = millis();
      cnt++;
    }
  }
}
float c_BIDS::DataGet(String Identifier, String Symbol, int Num) {
  return SerialGet("TR" + Identifier + Symbol + String(Num));
}
float c_BIDS::DataGet(String Identifier, int Num) {
  return DataGet(Identifier, "", Num);
}

int c_BIDS::DataGetInt(String Identifier, String Symbol, int Num) {
  int i = 0;
  String Command = "TR" + Identifier + Symbol + String(Num);
  SerialSender(Command);
  DataGet(i, Command);
  return i;

}
int c_BIDS::DataGetInt(String Identifier, int Num) {
  return DataGetInt(Identifier, "", Num);
}

String c_BIDS::DataGet(int& Data, String Header = "TR") {
  String s, data;
  s = DataGet(data, Header);
  Data = atoi(data.c_str());
  /*Serial.print("intGet : |");
  Serial.print(Data);
  Serial.print("|, char : |");
  Serial.print(data.c_str());
  Serial.print("|\n");
  Serial.flush();*/
  //delay(DELAY_TIME);
  return s;

}
String c_BIDS::DataGet(float& Data, String Header = "TR") {
  String s, data;
  s = DataGet(data, Header);
  Data = atof(data.c_str());
  /*Serial.print("floatGet : ");
  Serial.print(Data);
  Serial.print('\n');
  Serial.flush();
  delay(DELAY_TIME);*/
  return s;
}

String c_BIDS::DataGet(String& Data, String Header = "TR") {
  if (Serial.available() <= 0)return "";
  delay(DELAY_TIME);
  String GotData = Serial.readStringUntil('\n');
  Serial.print("GotData : |" + GotData + "|\n");
  delay(DELAY_TIME);

  GotData.replace("\n", "");
  GotData.replace("\r", "");
  Serial.flush();
  delay(DELAY_TIME);
  if (GotData.startsWith(Header)) {
    Data = GotData.substring(GotData.indexOf("X") + 1);
    //Serial.print("GDat : " + GotData + "\n");
    return GotData;
  }
  else {
    Data = "";
    return "";
  }

}

int c_BIDS::VersionCheck(int VersionNum, long BaudRate) {
  //Serial.begin(INITIAL_BAUDRATE);
  Serial.begin(BaudRate);
  int ret, vnum;
  /*if (VersionNum >= 202 && BaudRate != INITIAL_BAUDRATE) {
    vnum = SerialGet("TRV" + String(VersionNum) + "BR" + String(BaudRate));
    if (vnum >= 202) {
      Serial.end();
      delay(100);
      Serial.begin(BaudRate);
    }
    } else*/ vnum = DataGetInt("V", VersionNum);

  ret = vnum < VersionNum ? VersionNum : vnum;
  return ret;
}


enum c_BIDS::Reverser : int {
  Rear = -1,    //後
  Neutral = 0,  //切
  Front = 1,    //前
};
enum c_BIDS::key : int {
  Horn_1,               //default:enter
  Horn_2,               //default:+
  MusicHorn,            //default:-
  ConstantSpeedControl, //default:BackSpace
  S,                    //default:Space
  A_1,                  //default:Insert
  A_2,                  //default:Delete
  B_1,                  //default:Home
  B_2,                  //default:End
  C_1,                  //default:PageUp
  C_2,                  //default:Next
  D,                    //default:2
  E,                    //default:3
  F,                    //default:4
  G,                    //default:5
  H,                    //default:6
  I,                    //default:7
  J,                    //default:8
  K,                    //default:9
  L,                    //default:0
};
enum c_BIDS::Car : int {
  BrakeNotchCount,  //Number of Brake Notches
  PowerNotchCount,  //Number of Power Notches
  ATSNotchCount,    //ATS Cancel Notch
  B67NotchCount,    //80% Brake (67 degree)
  CarNumber,        //Number of Cars
};
enum c_BIDS::E : int {
  Location,     //Train Position (Z-axis) [m]
  Speed,        //Train Speed [km/h]
  Time,         //Time [ms]
  BcPressure,   //Pressure of Brake Cylinder [Pa]
  MrPressure,   //Pressure of MR [Pa]
  ErPressure,   //Pressure of ER [Pa]
  BpPressure,   //Pressure of BP [Pa]
  SapPressure,  //Pressure of SAP [Pa]
  Current,      //Current [A]
};
enum c_BIDS::Handle : int {
  BrakeNotch,     //Brake Notch
  PowerNotch,     //Power Notch
  Reverser,       //Reverser Position
  ConstantSpeed,  //Constant Speed Control
};
