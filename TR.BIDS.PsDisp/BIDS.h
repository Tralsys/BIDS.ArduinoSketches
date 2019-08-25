//BIDS.h
//Base : stop-pattern
//v202 Support : TetsuOtter


#ifndef _BIDS_
#define _BIDS_

class c_BIDS {
  public:
    c_BIDS();
    c_BIDS(int);
    c_BIDS(int, int);

    //int DataGet(byte*, byte*, int);
    float DataGet(String, String, int);
    float DataGet(String, int);
    String DataGet(float&, String);

    int Version;

    enum Reverser : int;    //for TRR
      enum key : int;       //for TRK
      enum Car : int;       //for TRIC
      enum E : int;         //for TRIE (tentative)
      enum Handle : int;    //for TRIH

    private:
      float SerialGet(String);
      int VersionCheck(int, int);
  };

#endif //_BIDS_
