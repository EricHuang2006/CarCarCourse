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

int lst_vl = 0, lst_vr = 0;

void Writemotor(double a, double b){
    lst_vl = a, lst_vr = b; 
    analogWrite(PWM[0], abs(a));
    analogWrite(PWM[1], abs(b));
    digitalWrite(A[0], a <= 0);
    digitalWrite(A[1], a > 0);
    digitalWrite(B[0], b > 0);
    digitalWrite(B[1], b <= 0);
}


// void tracking(){
  //   static int ll = 0, lr = 0;
  //   double error = 0;
  //   for(int i = 0; i < 5; i++) error += arr[i] * digitalRead(RF[i]);
  //   int kp = 35, tp = 160;
  //   int vr = tp - kp * error;
  //   int vl = 0.95 * (tp + kp * error);
  //   int deduct = digitalRead(RF[0]) | digitalRead(RF[4]);
//   if(deduct) vl *= 0.8, vr *= 0.8;
//   // vl = (vl + 4 * ll) / 10, vr = (vr + 9 * lr) / 10;
//   vl = min(vl, 255), vr = min(vr, 255);
//   vl = max(vl, -255), vr = max(vr, -255);
//   ll = vl, lr = vr;
//   Writemotor(vl, vr);
// }

double arr[] = {-1.7, -1, 0, 1, 1.7};

void tracking(bool flag = false){ // PID Control
  static double kd = 10, lastError = 0, ki = 0, sumError = 0;
  if(flag) lastError = 0, sumError = 0;
  double error = 0;
  for(int i = 0; i < 5; i++) error += arr[i] * digitalRead(RF[i]);
  sumError += error; // I
  sumError = constrain(sumError, -20, 20); // max speed : [-200, 200]
  double dError = error - lastError; // D
  
  int kp = 33, tp = 150;
  // int kp = 22, tp = 90;
  int powerCorrection = kp * error + kd * dError + ki * sumError;
  lastError = error;
  int vr = (tp - powerCorrection) * (powerCorrection >= 0 ? 1 : 0.75);
  int vl = 0.95 * (tp + powerCorrection) * (powerCorrection <= 0 ? 1 : 0.75);
  // int vl = 0.95 * (tp + powerCorrection) * (powerCorrection <= 0 ? 1 : 0.75);
  // int vr = (tp - powerCorrection) * (powerCorrection >= 0 ? 1 : 0.75);
  int deduct = digitalRead(RF[0]) | digitalRead(RF[4]);
  if(deduct) vl *= 0.85, vr *= 0.85;
  vl = min(vl, 255), vr = min(vr, 255);
  vl = max(vl, -255), vr = max(vr, -255);
  Writemotor(vl, vr);
}

void TurnLeft(unsigned long wait_ms = 250){
  unsigned long st = millis();
  while(millis() - st < 120){
    Writemotor(40, 30);
  }
  st = millis();
  while(millis() - st < wait_ms){
    Writemotor(0, 130);
  }
  while(!digitalRead(RF[0])){
    Writemotor(-50, 80);
  }
  st = millis();
  while(millis() - st < 50){
    Writemotor(100, 0);
    if(digitalRead(RF[2])) tracking();
  }
}

void stop(double t = 100){
  unsigned long start_time = millis();
  start_time = millis();
  while(millis() - start_time < t){
    Writemotor(0, 0);
  }
}

void TurnRight(unsigned long wait_ms = 250){
  unsigned long st = millis();
  while(millis() - st < 150){
    Writemotor(30, 50);
  }
  st = millis();
  while(millis() - st < wait_ms){
    Writemotor(130, -80);
  }
  while(!digitalRead(RF[4])){
    Writemotor(55, -70);
  }
  st = millis();
  while(millis() - st < 50){
    Writemotor(0, 110);
    if(digitalRead(RF[2])) tracking();
  }
}

void UTurn(unsigned long wait_ms = 400){
  unsigned long start_time = millis();
  start_time = millis();
  while(millis()-start_time < wait_ms){
    Writemotor(80, -220);
  }
  while(!digitalRead(RF[4])){
    Writemotor(30, -80);
  }
  start_time = millis();
  while(millis()-start_time < 100){
    Writemotor(0, 0);
  }
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
    Serial.println(F("*Card Detected:*"));
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
            Serial.print("stop!\n");
        }
        return c;
    }
    return '0';
}

void Forward(){
  while(ck() == 5){
    Writemotor(100, 107);
  }
}
void test1(){
  if(ck() == 5){
    if(state == 0){ // 起點 -> 1，之後右轉
      while(ck() >= 3){
        Writemotor(153, 160);
      }
    }
    else{
      UTurn();
      state = -1;
    }
    state++;
  }
}
void loop() {
    // checkMFRC();
    // BT_get();
    tracking();
    if(state == 5){
      mfrc522.PCD_Init();
      if(checkMFRC()){
          while(true){
              Writemotor(0, 0);
          }
      }
    }
    // test1();
    // return;
    if(ck() == 5){
        if(state == 0){ // 起點 -> 1，之後右轉
          TurnRight();
        }
        else if (state == 1){ // 1 -> 2，之後迴轉
          // while(ck() >= 3){
          //   Writemotor(174, 180);
          // }
          UTurn();
        }
        else if(state == 2){ // 2 -> 1，之後直走
          // UTurn();
          // TurnLeft();
          // Writemotor(lst_vr * 0.97, lst_vl * 1.03);
          // unsigned long st = millis();
          // while(millis() - st < 150){
            
          // }
          // Writemotor(lst_vr * 0.96, lst_vl * 1.03);
          // st = millis();
          // while(millis() - st < 100){

          // }
          // stop(500);
          Forward();
        }
        else if(state == 3){ // 1 -> 4，之後迴轉
          // TurnLeft();
          UTurn();
          // state = -1;
        }
        else if(state == 4){ // 4 -> 1，之後左轉
          // UTurn();
          // state = -1;
          TurnLeft();
        }
        else{ // 1 -> 3，之後停止
          UTurn();
          state = 0;
          return;
            while(ck()){
                Writemotor(200, 200);
            }
        }
        state++;
    }
    if(state > 5){
      Writemotor(0,0);
      return;
    }
}