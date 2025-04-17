/***************************************************************************/
// File			  [node.h]
// Author		  [Erik Kuo, Joshua Lin]
// Synopsis		[Code for managing car movement when encounter a node]
// Functions  [/* add on your own! */]
// Modify		  [2020/03/027 Erik Kuo]
/***************************************************************************/

/*===========================import variable===========================*/
int extern _Tp;
int extern PWM[];
int extern A[];
int extern B[];
int extern RF[];
/*===========================import variable===========================*/

// TODO: add some function to control your car when encounter a node
// here are something you can try: left_turn, right_turn... etc.

int ck(){
    int c = 0;
    for(int i = 0; i < 5; i++) c += digitalRead(RF[i]);
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

double arr[] = {-40, -20, 0, 20, 40};

void tracking(bool flag = false){ // PID Control
  static double kd = 0, lastError = 0, ki = 0, sumError = 0;
  if(flag) lastError = 0, sumError = 0;
  double error = 0;
  for(int i = 0; i < 5; i++) error += arr[i] * digitalRead(RF[i]);
  sumError += error; // I
  sumError = constrain(sumError, -20, 20); // max speed : [-200, 200]
  double dError = error - lastError; // D
  
  int kp = 1, tp = 150; // old : 37 // new kp : 170
  // int kp = 22, tp = 90;
  int powerCorrection = kp * error + kd * dError + ki * sumError;
  lastError = error;
  int vr = (tp - powerCorrection) * (powerCorrection >= 0 ? 1 : 1);
  int vl = 0.95 * (tp + powerCorrection) * (powerCorrection <= 0 ? 1 : 1);
  // int vl = 0.95 * (tp + powerCorrection) * (powerCorrection <= 0 ? 1 : 0.75);
  // int vr = (tp - powerCorrection) * (powerCorrection >= 0 ? 1 : 0.75);
  // int deduct = digitalRead(RF[0]) | digitalRead(RF[4]);
  // if(deduct) vl *= 0.85, vr *= 0.85;
  vl = min(vl, 255), vr = min(vr, 255);
  vl = max(vl, -255), vr = max(vr, -255);
  Writemotor(vl, vr);
}

double slow_arr[] = {-20, -10, 0, 10, 20};
void slow_tracking(){
  double powerCorrection = 0;
  for(int i = 0; i < 5; i++) powerCorrection += arr[i] * digitalRead(RF[i]);
  int tp = 80;
  int vr = (tp - powerCorrection);
  int vl = 0.95 * (tp + powerCorrection);
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
void TurnLeft(unsigned long wait_ms = 260){
  unsigned long st = millis();
  while(millis() - st < 70){
    Writemotor(40, 45);
  }
  st = millis();
  while(millis() - st < wait_ms){
    Writemotor(-55, 100);
  }
  while(!digitalRead(RF[0])){
    Writemotor(-30, 50);
  }
  stop(200);
  st = millis();
  while(millis() - st < 300){
    slow_tracking();
  }
}


void TurnRight(unsigned long wait_ms = 260){
  unsigned long st = millis();
  while(millis() - st < 70){
    Writemotor(40, 40);
  }
  st = millis();
  while(millis() - st < wait_ms){
    Writemotor(110, -60);
  }
  while(!digitalRead(RF[4])){
    Writemotor(48, -45);
  }
  stop(200);
  st = millis();
  while(millis() - st < 300){
    slow_tracking();
  }
}

void UTurn(unsigned long wait_ms = 440){
  unsigned long st = millis();
  st = millis();
  while(millis()-st < wait_ms){
    Writemotor(80, -200);
  }
  while(!digitalRead(RF[4])){
    Writemotor(28, -70);
  }
  stop(200);
  st = millis();
  while(millis() - st < 300){
    slow_tracking();
  }
}

void Forward(){
  while(ck() == 5){
    Writemotor(130, 138);
  }
}