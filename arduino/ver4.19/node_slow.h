int extern PWM[];
int extern A[];
int extern B[];
int extern IR[];

int ck(){
    int c = 0;
    for(int i = 0; i < 5; i++) c += digitalRead(IR[i]);
    return c;
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

void slow_tracking(){
    double error = 0;
    for(int i = 0; i < 5; i++) error += arr[i] * digitalRead(IR[i]);
    int kp = 16, tp = 45;
    int vr = tp - kp * error;
    int vl = tp + kp * error;
    vl = min(vl, 255), vr = min(vr, 255);
    vl = max(vl, -255), vr = max(vr, -255);
    Writemotor(vl, vr);
}
void tracking(){
    double error = 0;
    for(int i = 0; i < 5; i++) error += arr[i] * digitalRead(IR[i]);
    int kp = 32, tp = 100;
    int vr = tp - kp * error;
    int vl = tp + kp * error;
    vl = min(vl, 255), vr = min(vr, 255);
    vl = max(vl, -255), vr = max(vr, -255);
    Writemotor(vl, vr);
}
void stop(double t = 100){
    unsigned long start_time = millis();
    start_time = millis();
    while(millis() - start_time < t){
        Writemotor(0, 0);
    }
}
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
  while(digitalRead(IR[4]) != 1){
      Writemotor(65, -80);
  }
  stop(100);
}
void Forward(){
  while(ck() == 5){
    Writemotor(85, 90);
  }
}