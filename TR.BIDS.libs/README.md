# BIDS Serial Library for Arduino
## functions
### BIDS(*Stream)
BIDSクラスを初期化します。引数には、予め通信が開始された状態のシリアルインターフェースへのポインタを渡します。  
通信が開始されていない状態での使用は想定されていません。

### AddAutoSend(char, int, AS_OnDataGot)
ライブラリ内部のAutoSend管理リストに、引数で指定したデータタイプ/番号および受信時動作を設定します。

|Header|Command Type|Data Type|Data Number|Footer|
|-|-|-|-|-|
|TR|A|E|10|¥n|

上記Data Typeは必ず1文字の英字となり、これは第一引数で指定します。
上記Data Numberは正の整数であり、これは第二引数で指定します。

AutoSendでBIDS_Server等からデータが送られてきたときに、実行する処理を第三引数に指定します。  
ここで指定する処理はint型の第一引数およびdouble型の第二引数を受けるようにする必要があります。

例として、速度情報をAutoSendにより受信するプログラムを、以下に示します。なお、各種初期化処理は省略しています。

~~~
BIDS bids = BIDS(&Serial);
void setup()
{  
  bids.AddAutoSend('E', 1, func);
}
void func(int i, double d)
{
  //i : Got-data in int
  //d : Got-data in double
  //(any process)
}
~~~


以下準備中
