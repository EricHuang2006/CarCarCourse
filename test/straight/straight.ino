int PWM[] = {12, 13}; // A, B
int A[] = {2, 3}; // 1, 2
int B[] = {5, 6}; // 1, 2
int RF[] = {30, 32, 34, 36, 38};
#include <SoftwareSerial.h>
#include <MFRC522.h>
#include <SPI.h>
// SoftwareSerial BT(10, 11); 
#define BT Serial1
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

void Writemotor(double a, double b){
    analogWrite(PWM[0], abs(a));
    analogWrite(PWM[1], abs(b));
    digitalWrite(A[0], a <= 0);
    digitalWrite(A[1], a > 0);
    digitalWrite(B[0], b > 0);
    digitalWrite(B[1], b <= 0);
}

void tracking(){
    static int ll = 0, lr = 0;
    double error = 0;
    for(int i = 0; i < 5; i++) error += (i - 2) * digitalRead(RF[i]);
    int kp = 20, tp = 80;
    int vr = tp - kp * error;
    int vl = 0.9 * (tp + kp * error);
    vl = (vl + 9 * ll) / 10, vr = (vr + 9 * lr) / 10;
    vl = min(vl, 255), vr = min(vr, 255);
    vl = max(vl, -255), vr = max(vr, -255);
    ll = vl, lr = vr;
    Writemotor(vl, vr);
}

void TurnLeft(unsigned long wait_ms = 300){
  unsigned long start_time = millis();
  while(millis()-start_time < wait_ms){
    Writemotor(-140, 150);
  }
  while(!digitalRead(RF[2])){
    Writemotor(130, -140);
  }
  Writemotor(180,200);
}

void TurnRight(unsigned long wait_ms = 300){
  unsigned long start_time = millis();
  while(millis() - start_time < wait_ms){
    Writemotor(130, -140);
  }
  while(!digitalRead(RF[4])){
    Writemotor(130, -140);
  }
  start_time = millis();
  while(millis() - start_time < 50){
    Writemotor(-100, 140);
  }
  Writemotor(180,200);
}

void UTurn(unsigned long wait_ms = 300){
  while(ck() == 5){
    Writemotor(200, 200);
  }
  unsigned long start_time = millis();
  while(millis()-start_time < wait_ms){
    //Writemotor(175,-175); 700
    Writemotor(225,-225);
  }
  Writemotor(180,200);
}

int ck(){
    int c = 0;
    for(int i = 0; i < 5; i++) c += digitalRead(RF[i]);
    return c;
}

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
        if(c == 's'){
            Serial.print("same!\n");
            Writemotor(150, -150);
            delay(500);
            while(true){
                Writemotor(0, 0);
            }
            Serial.print("OK!\n");
        }
        return c;
    }
    return '0';
}

void loop() {
    tracking();
    if(checkMFRC()){
        while(true){
            Writemotor(0, 0);
        }
    }
}