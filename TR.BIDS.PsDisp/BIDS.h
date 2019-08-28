//BIDS.h
//Base : stop-pattern
//v202 Support : TetsuOtter


#ifndef _BIDS_
#define _BIDS_

class c_BIDS {
  public:
    c_BIDS();
    c_BIDS(int);
    c_BIDS(int, long);

    //int DataGet(byte*, byte*, int);
    float DataGet(String, String, int);
    float DataGet(String, int);
    int DataGetInt(String, int);
    int DataGetInt(String, String, int);
    String DataGet(int&, String);
    String DataGet(float&, String);
    String DataGet(String&, String);

    int Version;

    enum Reverser : int;    //for TRR
      enum key : int;       //for TRK
      enum Car : int;       //for TRIC
      enum E : int;         //for TRIE (tentative)
      enum Handle : int;    //for TRIH

    private:
      float SerialGet(String);
      void SerialSender(String);
      int VersionCheck(int, long);
  };

#endif //_BIDS_
