# Servo Motor Driver with BIDS
## About this
シリアル通信にてBIDS_Server等と共有した圧力情報をもとに、最大で2個のサーボモーターを制御します。  
サーボモーターの接続ピン、1kPaあたり何degree回転させるかなどを、簡単に変更することが可能になっています。

## How to Use
予め[TR.BIDS.ArduinoLibs](https://github.com/TetsuOtter/TR.BIDS.ArduinoLibs)をArduino IDEにインストールしたうえで、本スケッチをArduino IDEで開き、ボードへの書き込みを行ってください。  
Arduino IDE 1.8.10にて正常に書き込みができることを確認しております。

## Variables
「該当箇所」リンクは、変数定義位置へのリンクです。

### DEBUG_SERVO
[該当箇所](TR.BIDS.PresServo.h#L7)  
サーボモーターの動作試験モードを有効化します。  
このモードでは、シリアル通信の開通状況に関係なく、0kPa~1000kPaを順番に繰り返し表示させます。Servo1とServo2は交互に動作します。  
なお、起動直後のみ、各サーボは90°に移動した後0°に移動する動作をします。

DEBUGなど他に定義されたオプションも使用され、例えばDEBUGを設定するとデバッグ用情報がシリアルに出力されます。

無効化するには、該当箇所をコメントアウトしてください。

### DEBUG_SERVO_STEP
[該当箇所](TR.BIDS.PresServo.h#L8)  
値の増える具合を設定します。例えば、デフォルトの「5」では、0kPaから5kPaごとに増圧していき、1000kPaになったら逆に995kPaから順に0kPaまで5kPaずつ減圧していく表示になります。

### DEBUG_SERVO_INTERVAL
[該当箇所](TR.BIDS.PresServo.h#L9)  
値の増減の指示を出した後に休止する時間の設定です。単位はミリ秒で、デフォルトは100ミリ秒です。

### DEBUG
[該当箇所](TR.BIDS.PresServo.h#L11)  
デバッグモードを有効化します。BIDS_Serverから受け取った情報やそれをもとに計算した圧力などを出力します。

無効化するには、該当箇所をコメントアウトしてください。

### USING_DNUM_1 / USING_DNUM_2
[該当箇所](TR.BIDS.PresServo.h#L13-L14)  
各サーボに割り当てる圧力値の種類を選択します。なお、末尾の数字はサーボ番号と一致します。  
圧力情報としては、以下の5つが利用できますが、これに限定されません。
- DNUM_ELAPD_BC_PRES
- DNUM_ELAPD_MR_PRES
- DNUM_ELAPD_ER_PRES
- DNUM_ELAPD_BP_PRES
- DNUM_ELAPD_SAP_PRES
[利用できるすべての情報は、こちらから確認できます。](https://github.com/TetsuOtter/TR.BIDS.ArduinoLibs/blob/master/src/TR.BIDS.defs.h#L21-L34)

### SERVO_PIN_1 / SERVO_PIN_2
[該当箇所](TR.BIDS.PresServo.h#L16-L17)  
サーボモーターを接続したピンを設定します。

サーボモーターを一つしか使用しない場合は、[L45](TR.BIDS.PresServo.h#L45)および[L48](TR.BIDS.PresServo.h#L48)のコメントアウトで対応してください。  
逆にサーボモーターを3つ以上接続したい場合は、ご自身で本スケッチを修正するか、issueよりご要望をお送りください。

### Deg_Per_kPa1 / Deg_Per_kPa2
[該当箇所](TR.BIDS.PresServo.h#L19-L20)  
BIDSより受信した値が'1'増減するごとに、サーボモーターで何degree動かすかを設定します。

BIDSからの入力値が過大になった結果、本スケッチではリミッターを実装していないため、サーボモーターへの出力値が180をオーバーする可能性があります。

### ENABLE_THRESHOLD1 / ENABLE_THRESHOLD2
[該当箇所1](TR.BIDS.PresServo.h#L22)  
[該当箇所2](TR.BIDS.PresServo.h#L25)  
閾値によるサーボモーターの動作制限を行うかどうかを設定します。有効にすると、Threshold1_Deg / Threshold2_Degで設定した角度以上の動作のみ行います。

### Threshold1_Deg / Threshold2_Deg
[該当箇所1](TR.BIDS.PresServo.h#L23)  
[該当箇所2](TR.BIDS.PresServo.h#L26)  
サーボモーターの動作制限の閾値を設定します。ENABLE_THRESHOLDを設定しない場合、ここで設定した値は使用されません。