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
SoftwareSerial BT(10, 11);

int PWM[] = {12, 13}; // A, B
int A[] = {2, 3};
int B[] = {5, 6};
int IR[] = {30, 32, 34, 36, 38};
// RFID, 請按照自己車上的接線寫入腳位
#define RST_PIN 9                // 讀卡機的重置腳位
#define SS_PIN 53               // 晶片選擇腳位
MFRC522 mfrc522(SS_PIN, RST_PIN);  // 建立MFRC522物件
/*===========================define pin & create module object===========================*/

/*============setup============*/
void setup() {
    Serial.begin(9600);
    BT.begin(9600);
    SPI.begin();
    mfrc522.PCD_Init();
    for(int i = 0; i < 2; i++) 
        pinMode(PWM[i], OUTPUT), pinMode(A[i], OUTPUT), pinMode(B[i], OUTPUT);
    for(int i = 0; i < 5; i++)
        pinMode(IR[i], INPUT);

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
    Serial.println(F("**Card Detected:**"));
    // mfrc522->PICC_DumpDetailsToSerial(&(mfrc522->uid)); //讀出 UID
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        Serial.print(mfrc522.uid.uidByte[i], HEX);
        BT.print(mfrc522.uid.uidByte[i], HEX);
        // uidStr += String(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.println("");
    BT.println("");
    mfrc522.PICC_HaltA(); // 讓同一張卡片進入停止模式 (只顯示一次)
    mfrc522.PCD_StopCrypto1(); // 停止 Crypto1
    return true;
}

char BT_get(){
    if(BT.available()){
        char c = BT.read();
        Serial.print("get : ");
        Serial.println(c);
        return c;
    }
    return 'z';
}

int cnt = 1, id = 1, ini = 0;
char c = 'z';

void send(String s){
    for(int i = 0; i < s.length(); i++){
        byte b = s[i];
        if(b < 0x10) BT.print("0");
        BT.print(b, HEX);
    }
    BT.println("");
}

void loop(){
    while(true){
        tracking();
        mfrc522.PCD_Init();
        if(checkMFRC()){
            while(true){
                Writemotor(0, 0);
            }
        }
    }
    // if(c == 'b'){
    //     mfrc522.PCD_Init();
    //     checkMFRC();
    // }
    if(!ini){
        while(c == 'z') c = BT_get();
        BT.println("s");
    }
    ini = 1;
    if(ck() == 5){
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
                break;
        }
        unsigned long st = millis();
        while(millis() - st < 200){
            tracking();
        }
        c = 'z';
        while(c == 'z') c = BT_get();
        char a = char('a' + cnt);
        cnt++;
        String str = "get : " + String(c);
        send(str);
        BT.println("s");
    }
    tracking();
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
