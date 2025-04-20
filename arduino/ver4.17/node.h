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
int rd[5] = {};

void tracking(bool flag = false){
  double error = 0;
  for(int i = 0; i < 5; i++) if((rd[i] = digitalRead(RF[i]))) error += arr[i];
  // int m0 = rd[2], l2 = rd[0], l1 = rd[1], r1 = rd[3], r2 = rd[4];
  // if(m0 and l2+l1+r1+r2==0){
  //     Writemotor(248,255);
  //   }else if(m0+r1==2 and r2+l1+l2==0){
  //     Writemotor(215,180);
  //   }else if(m0+l1==2 and l2+r1+r2==0){
  //     Writemotor(176,220);
  //   }else if(r1 and r2+m0+l1+l2==0){
  //     Writemotor(195,150);
  //   }else if(l1 and l2+m0+r1+r2==0){
  //     Writemotor(147,200);
  //   }else if(r1+r2==2 and m0+l1+l2==0){
  //     Writemotor(195,120);
  //   }else if(l1+l2==2 and m0+r1+r2==0){
  //     Writemotor(118,200);
  //   }else if(r2==1 and r1+m0+l1+l2==0){
  //     Writemotor(195,80);
  //   }else if(l2==1 and l1+m0+r1+r2==0){
  //     Writemotor(79,200);
  //   }else if(l2+l1+m0+r1+r2==0){
  //     // Writemotor(-195,-200);
  //     Writemotor(248,255);
  //   }else{
  //     Writemotor(0,0);
  //   }
  
  // return;
  // if(rd[0]){
  //   Writemotor(80, 150);
  //   return;
  // }
  // if(rd[4]){
  //   Writemotor(145, 85);
  //   return;
  // }
  
  int kp = 1, tp = 150; // old : 37 // new kp : 170
  // int kp = 22, tp = 90;
  //int powerCorrection = kp * error + kd * dError + ki * sumError;
  //lastError = error;
  int vr = (tp - error);
  int vl = 0.95 * (tp + error);
  
  // int vl = 0.95 * (tp + powerCorrection) * (powerCorrection <= 0 ? 1 : 0.75);
  // int vr = (tp - powerCorrection) * (powerCorrection >= 0 ? 1 : 0.75);
  // int deduct = digitalRead(RF[0]) | digitalRead(RF[4]);
  // if(deduct) vl *= 0.85, vr *= 0.85;
  //vl = min(vl, 255), vr = min(vr, 255);
  //vl = max(vl, -255), vr = max(vr, -255);
  Writemotor(vl, vr);
}

double slow_arr[] = {-20, -10, 0, 10, 20};
void slow_tracking(){
  double powerCorrection = 0;
  for(int i = 0; i < 5; i++) powerCorrection += slow_arr[i] * digitalRead(RF[i]);
  int tp = 90;
  int vr = (tp - powerCorrection);
  int vl = 0.95 * (tp + powerCorrection);
  vl = min(vl, 255), vr = min(vr, 255);
  vl = max(vl, -255), vr = max(vr, -255);
  Writemotor(vl, vr);
}

double mid_arr[] = {-30, -15, 0, 15, 30};
void mid_tracking(){
  double powerCorrection = 0;
  for(int i = 0; i < 5; i++) powerCorrection += mid_arr[i] * digitalRead(RF[i]);
  int tp = 130;
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
    Writemotor(55, -45);
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