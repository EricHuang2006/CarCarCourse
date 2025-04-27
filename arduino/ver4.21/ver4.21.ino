/***************************************************************************/
// File       [final_project.ino]
// Author     [Erik Kuo]
// Synopsis   [Code for managing main process]
// Functions  [setup, loop, Search_Mode, Hault_Mode, SetState]
// Modify     [2020/03/27 Erik Kuo]
/***************************************************************************/

#define DEBUG  // debug flag

// for RFID
#include <MFRC522.h>
#include <SPI.h>
#include <SoftwareSerial.h>
/*===========================define pin & create module object================================*/
// SoftwareSerial BT(10, 11);
#define BT Serial1
int PWM[] = {12, 13}; // A, B
int A[] = {2, 3};
int B[] = {5, 6};
int RF[] = {30, 32, 34, 36, 38, 28, 42};
// RFID, 請按照自己車上的接線寫入腳位
#define RST_PIN 9                // 讀卡機的重置腳位
#define SS_PIN 53               // 晶片選擇腳位
MFRC522 mfrc522(SS_PIN, RST_PIN);  // 建立MFRC522物件
/*===========================define pin & create module object===========================*/

/*============setup============*/
void setup() {
    // Serial.begin(9600);
    BT.begin(9600); 
    SPI.begin();
    mfrc522.PCD_Init();
    for(int i = 0; i < 2; i++) 
        pinMode(PWM[i], OUTPUT), pinMode(A[i], OUTPUT), pinMode(B[i], OUTPUT);
    for(int i = 0; i < 5; i++)
        pinMode(RF[i], INPUT);

#ifdef DEBUG
#endif
}
/*============setup============*/

/*=====Import header files=====*/
#include "RFID.h"
// #include "bluetooth.h"
// #include "node_slow.h"
#include "node.h"
// #include "track.h" functions are in node.h
/*=====Import header files=====*/

/*===========================initialize variables===========================*/
int _Tp = 100;                                // set your own value for motor power
//BT_CMD _cmd = NOTHING;  // enum for bluetooth message, reference in bluetooth.h line 2
/*===========================initialize variables===========================*/

/*===========================declare function prototypes===========================*/
void Search();    // search graph
void SetState();  // switch the state

int state = 0;

bool checkMFRC(){
    if(!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
        return false;
    } //PICC_IsNewCardPresent()：是否感應到新的卡片?
    // Serial.println(F("*Card Detected:*"));
    // mfrc522->PICC_DumpDetailsToSerial(&(mfrc522->uid)); //讀出 UID
    String s = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        // Serial.print(mfrc522.uid.uidByte[i], HEX);
        if (mfrc522.uid.uidByte[i] < 0x10) s += "0";
        s += String(mfrc522.uid.uidByte[i], HEX);
        // BT.print(mfrc522.uid.uidByte[i], HEX);
        // uidStr += String(mfrc522.uid.uidByte[i], HEX);
    }
    // Serial.println("");
    BT.println(s);
    mfrc522.PICC_HaltA(); // 讓同一張卡片進入停止模式 (只顯示一次)
    mfrc522.PCD_StopCrypto1(); // 停止 Crypto1
    return true;
}

String s = "fflfbfrrlrbllflbfbs";
int sid = 0;
char BT_get(){
    // if(sid == s.length()) return 'z';
    // return s[sid++];
    if(BT.available()){
        char c = BT.read();
        // Serial.print("get : ");
        // Serial.println(c);
        return c;
    }
    return 'z';
}

int cnt = 0, id = 1, ini = 0;
char c = 'z';

void send(String s){
    for(int i = 0; i < s.length(); i++){
        byte b = s[i];
        if(b < 0x10) BT.print("0");
        BT.print(b, HEX);
    }
    BT.println("");
}

String command = "fllbrrfb";

void test1(){
  tracking();
  if(ck() >= 5){
      switch(command[state]){
          case 'l':
              TurnLeft();
              break;
          case 'r':
              TurnRight();
              break;
          case 'b':
              UTurn();
              break;
          case 'f':
              Forward();
              break;
          case 's':
              while(true){
                  Writemotor(0, 0);
              }
              break;
          default:
              while(true){
                  Writemotor(0, 0);
              }
              break;
      }
      state = (state + 1)%8  ;
  }
}
char lst_command = 'z';
int turn = 0;
int got_uid = 0;
int ltime = 0;
int aa = 0;

void loop(){
    if(!ini){
        while(c == 'z') c = BT_get();
        BT.println(c);
    }
    ini = 1;
    if(ck() >= 5){
        switch(c){
            case 'l':
                TurnLeft();
                break;
            case 'r':
                TurnRight();
                break;
            case 'b':
                UTurn();
                break;
            case 'f':
                Forward();
                break;
            case 's':
                while(true){
                    Writemotor(0, 0);
                }
                break;
            default:
                while(true){
                    Writemotor(0, 0);
                }
                break;
        }
        lst_command = c;
        c = BT_get();
        while(c == 'z'){
          Writemotor(0, 0);
          c = BT_get();
        } 
        BT.println(c);
    }
    if(c == 'b' || lst_command == 'b'){
        tracking();
        // turn = (turn + 1) % 5;
        // if(!turn){
          // mfrc522.PCD_Reset();
        mfrc522.PCD_WriteRegister(MFRC522::TModeReg, 0x80);
        mfrc522.PCD_WriteRegister(MFRC522::TPrescalerReg, 0xA9);
        mfrc522.PCD_WriteRegister(MFRC522::TReloadRegH, 0x00);
        mfrc522.PCD_WriteRegister(MFRC522::TReloadRegL, 0x10);
        mfrc522.PCD_WriteRegister(MFRC522::TxASKReg, 0x40);
        mfrc522.PCD_WriteRegister(MFRC522::ModeReg, 0x3D);
        mfrc522.PCD_AntennaOn();
        // }
        if(checkMFRC()) got_uid = 1;
        // int newtime = millis(), cost = newtime - ltime;
        // BT.println("[ Time : " + String(cost));
        // ltime = newtime;
    }
    else tracking();
}


void SetState() {

    // TODO:
    // 1. Get command from bluetooth
    // 2. Change state if need
}

void Search() {
    // TODO: let your car search graph(maze) according to bluetooth command from computer(python
    // code)
}
/*===========================define function===========================*/