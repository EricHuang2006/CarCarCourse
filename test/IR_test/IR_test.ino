#include <SPI.h>
#include <MFRC522.h>
// #include <SoftwareSerial.h>
#include <SoftwareSerial.h>
SoftwareSerial BT(10, 11);
// 引入 SPI 程式庫 與 MFRC522 程式庫
#define RST_PIN 9
#define SS_PIN 53
// 設定重設腳位 與 SPI 介面裝置選擇腳位
MFRC522 *mfrc522;
int PWM[] = {12, 13}; // A, B
int A[] = {2, 3}; // 1, 2
int B[] = {5, 6}; // 1, 2
int RF[] = {30, 32, 34, 36, 38};
// 宣告MFRC522指標

void setup() {
    Serial.begin(9600);
    SPI.begin();
    mfrc522 = new MFRC522(SS_PIN, RST_PIN);
    mfrc522->PCD_Init();
    BT.begin(9600);
    for(int i = 0; i < 2; i++){
        pinMode(PWM[i], OUTPUT);
        pinMode(A[i], OUTPUT);
        pinMode(B[i], OUTPUT);
    }
    // Serial.println(F("Read UID on a MIFARE PICC:"));
}

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

void loop() { 
    for(int i = 0; i < 5; i++){
        Serial.print(digitalRead(RF[i]));
        Serial.print(" ");
    }
    Serial.println("");
}
