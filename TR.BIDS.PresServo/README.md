# Servo Motor Driver with BIDS
## About this
シリアル通信にてBIDS_Server等と共有した圧力情報をもとに、最大で2個のサーボモーターを制御します。  
サーボモーターの接続ピン、1kPaあたり何degree回転させるかなどを、簡単に変更することが可能になっています。

## Variables
「該当箇所」リンクは、[commit 156d46350e3a18401a1c53f1318c0f848a419e61](https://github.com/TetsuOtter/BIDS.ArduinoSketches/commit/156d46350e3a18401a1c53f1318c0f848a419e61)での変数定義位置を指します。

### DEBUG
[該当箇所](https://github.com/TetsuOtter/BIDS.ArduinoSketches/blob/156d46350e3a18401a1c53f1318c0f848a419e61/TR.BIDS.PresServo/TR.BIDS.PresServo.ino#L11)  
デバッグモードを有効化します。BIDS_Serverから受け取った情報やそれをもとに計算した圧力などを出力します。

無効化するには、該当箇所をコメントアウトしてください。

### USING_DNUM_1 / USING_DNUM_2
[該当箇所](https://github.com/TetsuOtter/BIDS.ArduinoSketches/blob/156d46350e3a18401a1c53f1318c0f848a419e61/TR.BIDS.PresServo/TR.BIDS.PresServo.ino#L13-L14)  
各サーボに割り当てる圧力値の種類を選択します。なお、末尾の数字はサーボ番号と一致します。  
圧力情報としては、以下の5つが利用できますが、これに限定されません。
- DNUM_ELAPD_BC_PRES
- DNUM_ELAPD_MR_PRES
- DNUM_ELAPD_ER_PRES
- DNUM_ELAPD_BP_PRES
- DNUM_ELAPD_SAP_PRES
[利用できるすべての情報は、こちらから確認できます。](https://github.com/TetsuOtter/TR.BIDS.ArduinoLibs/blob/master/src/TR.BIDS.defs.h#L21-L34)

### SERVO_PIN_1 / SERVO_PIN_2
以下準備中