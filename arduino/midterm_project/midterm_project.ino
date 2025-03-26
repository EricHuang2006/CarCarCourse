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

/*===========================define pin & create module object================================*/
// BlueTooth
// BT connect to Serial1 (Hardware Serial)
// Mega               HC05
// Pin  (Function)    Pin
// 18    TX       ->  RX
// 19    RX       <-  TX
// TB6612, 請按照自己車上的接線寫入腳位(左右不一定要跟註解寫的一樣)
// TODO: 請將腳位寫入下方
#define MotorR_I1 0     // 定義 A1 接腳（右）
#define MotorR_I2 0     // 定義 A2 接腳（右）
#define MotorR_PWMR 0  // 定義 ENA (PWM調速) 接腳
#define MotorL_I3 0     // 定義 B1 接腳（左）
#define MotorL_I4 0     // 定義 B2 接腳（左）
#define MotorL_PWML 0  // 定義 ENB (PWM調速) 接腳

// 循線模組, 請按照自己車上的接線寫入腳位
int PWM[] = {12, 13}; // A, B
int A[] = {2, 3};
int B[] = {5, 6};
int IR[] = {30, 32, 34, 36, 38};
// RFID, 請按照自己車上的接線寫入腳位
#define RST_PIN 19                // 讀卡機的重置腳位
#define SS_PIN 18                // 晶片選擇腳位
MFRC522 mfrc522(SS_PIN, RST_PIN);  // 建立MFRC522物件
/*===========================define pin & create module object===========================*/

/*============setup============*/
void setup() {
    // bluetooth initialization
    Serial1.begin(9600);
    // Serial window
    Serial.begin(9600);
    // RFID initial
    SPI.begin();
    mfrc522.PCD_Init();
    // TB6612 pin
    for(int i = 0; i < 2; i++) 
        pinMode(PWM[i], OUTPUT), pinMode(A[i], OUTPUT), pinMode(B[i], OUTPUT);
    // tracking pin
    for(int i = 0; i < 5; i++)
        pinMode(IR[i], INPUT);

#ifdef DEBUG
    Serial.println("Start!");
#endif
}
/*============setup============*/

/*=====Import header files=====*/
#include "RFID.h"
#include "bluetooth.h"
#include "node.h"
// #include "track.h" functions are in node.h
/*=====Import header files=====*/

/*===========================initialize variables===========================*/
int l2 = 0, l1 = 0, m0 = 0, r1 = 0, r2 = 0;  // 紅外線模組的讀值(0->white,1->black)
int _Tp = 100;                                // set your own value for motor power
bool state = false;     // set state to false to halt the car, set state to true to activate the car
BT_CMD _cmd = NOTHING;  // enum for bluetooth message, reference in bluetooth.h line 2
/*===========================initialize variables===========================*/

/*===========================declare function prototypes===========================*/
void Search();    // search graph
void SetState();  // switch the state
/*===========================declare function prototypes===========================*/

/*===========================define function===========================*/

void Writemotor(double a, double b){
    analogWrite(PWM[0], abs(a));
    analogWrite(PWM[1], abs(b));
    digitalWrite(A[0], a <= 0);
    digitalWrite(A[1], a > 0);
    digitalWrite(B[0], b > 0);
    digitalWrite(B[1], b <= 0);
}

void tracking(){
    double error = 0;
    for(int i = 0; i < 5; i++) error += (i - 2) * digitalRead(RF[i]);
    int kp = 100, tp = 150;
    int vr = tp - kp * error;
    int vl = tp + kp * error;
    vl = min(vl, 255), vr = min(vr, 255);
    vl = max(vl, -255), vr = max(vr, -255);
    Writemotor(vl, vr);
}

int ck(){
    int c = 0;
    for(int i = 0; i < 5; i++) c += digitalRead(RF[i]);
    return c;
}

void checkRF(){
    for(int i = 0; i < 5; i++){
        int a = digitalRead(RF[i]);
        Serial.print(a); // 將數值印出來int sensorValue = digitalRead(analogPin);
        Serial.print(" "); // 將數值印出來
    }
    delay(200);
    Serial.println(" "); // 將數值印出來
}


void 














// void loop() {
//     if (!state)
//         MotorWriting(0, 0);
//     else
//         Search();
//     SetState();
// }

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
