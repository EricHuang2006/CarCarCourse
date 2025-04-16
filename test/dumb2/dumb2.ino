int PWM[] = {12, 13}; // A, B
int A[] = {2, 3}; // 1, 2
int B[] = {5, 6}; // 1, 2
int RF[] = {30, 32, 34, 36, 38};
#include <SoftwareSerial.h>
#include <MFRC522.h>
#include <SPI.h>
SoftwareSerial BT(10, 11); 
#define RST_PIN 9                // 讀卡機的重置腳位
#define SS_PIN 53               // 晶片選擇腳位
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
    SPI.begin();
    mfrc522.PCD_Init();
    Serial.begin(9600);
    BT.begin(9600);
    for(int i = 0; i < 2; i++){
        pinMode(PWM[i], OUTPUT);
        pinMode(A[i], OUTPUT);
        pinMode(B[i], OUTPUT);
    }
}

bool checkMFRC(){
    if(!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
        return false;
    } //PICC_IsNewCardPresent()：是否感應到新的卡片?
    Serial.println(F("*Card Detected:*"));
    // mfrc522->PICC_DumpDetailsToSerial(&(mfrc522->uid)); //讀出 UID
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        Serial.print(mfrc522.uid.uidByte[i], HEX);
        BT.print(mfrc522.uid.uidByte[i], HEX);
        // uidStr += String(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.println("");
    BT.println("");
    while(true){
        Writemotor(0, 0);
    }
    mfrc522.PICC_HaltA(); // 讓同一張卡片進入停止模式 (只顯示一次)
    mfrc522.PCD_StopCrypto1(); // 停止 Crypto1
    return true;
}

void Writemotor(double a, double b){
    analogWrite(PWM[0], abs(a));
    analogWrite(PWM[1], abs(b));
    digitalWrite(A[0], a <= 0);
    digitalWrite(A[1], a > 0);
    digitalWrite(B[0], b > 0);
    digitalWrite(B[1], b <= 0);
}

double arr[] = {-1.6, -1, 0, 1, 1.6};

void tracking(){
    double error = 0;
    for(int i = 0; i < 5; i++) error += arr[i] * digitalRead(RF[i]);
    int kp = 32, tp = 100;
    int vr = tp - kp * error;
    int vl = tp + kp * error;
    vl = min(vl, 255), vr = min(vr, 255);
    vl = max(vl, -255), vr = max(vr, -255);
    Writemotor(vl, vr);
}
void slow_tracking(){
    double error = 0;
    for(int i = 0; i < 5; i++) error += arr[i] * digitalRead(RF[i]);
    int kp = 16, tp = 45;
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

void stop(double t = 100){
    unsigned long start_time = millis();
    start_time = millis();
    while(millis() - start_time < t){
        Writemotor(0, 0);
    }
}

int state = 0;

void TurnRight(){
    unsigned long st = millis();
    while(millis() - st < 80){
        Writemotor(20, 80);
    }
    st = millis();
    while(millis() - st < 500){ // 500 + stop(80)
        Writemotor(130, 20);
    }
    while(!ck()){
        Writemotor(55, -47); //(85, -70) also work
    }
    stop(80);
}
void TurnLeft(){
    unsigned long st = millis();
    while(millis() - st < 480){
        Writemotor(20, 130);
    }
    while(!ck()){
        Writemotor(-36, 55);
    }
    stop(80);
}
void UTurn(){
  while(ck()){
    Writemotor(85, 95);
  }
  unsigned long st = millis();
  while(millis() - st < 250){
    Writemotor(80, -120);
  }
  while(digitalRead(RF[4]) != 1){
      Writemotor(65, -80);
  }
  stop(100);
  st = millis();
  while(millis() - st < 200){
    tracking();
  }
}
void Forward(){
  while(ck() == 5){
    Writemotor(85, 90);
  }
}

void loop() {
    checkMFRC();
    if(ck() == 5){
        if(state == 0){ // 起點 -> 1，之後右轉
          TurnRight();
          unsigned long st = millis();
          while(millis() - st < 250){
            slow_tracking();
          }
          stop(1000);
        }
        else if (state == 1){ // 1 -> 2，之後迴轉
            UTurn();
            stop(2000);
        }
        else if(state == 2){ // 2 -> 1，之後直走
            Forward();
        }
        else if(state == 3){ // 1 -> 4，之後迴轉
            UTurn();
        }
        else if(state == 4){ // 4 -> 1，之後左轉
            TurnLeft();
        }
        else{ // 1 -> 3，之後停止
            UTurn();
            state = 0;
            return;
            while(ck()){
                Writemotor(85, 90);
            }
            while(true){
                Writemotor(0, 0);
            }
        }
        state++;
    }
    if(state != 1 && state != 2) tracking();
    else slow_tracking();
    // tracking();
}