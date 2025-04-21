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
int d[7], lst_sum = 0;
int ck(){
    int c = 0;
    for(int i = 0; i < 7; i++) c += (d[i] = digitalRead(RF[i])) * (i < 5);
    lst_sum = c;
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

void stop(double t = 100){
  unsigned long start_time = millis();
  start_time = millis();
  while(millis() - start_time < t){
    Writemotor(0, 0);
  }
}
double arr[] = {-40, -20, 0, 20, 40};

void slow_tracking();
int lst_slow = -1000;
void tracking(bool flag = false){ // PID Control
  if(millis() - lst_slow < 200){
    slow_tracking();
  }
  for(int i = 0; i < 5; i++) d[i] = digitalRead(RF[i]);
  // if(d[5]){ // 最左
  //   Writemotor(-30, 80);
  // }
  // else if(d[6]){ // 最右邊
  //   Writemotor(76, -30);
  // }
  // if(d[0] || d[4]){
  //   stop(10000);
  // }
  if(!(d[0] + d[1] + d[2] + d[3] + d[4])){  
    // stop(10000);
    if(d[5]){
      lst_slow = millis();
      slow_tracking();
      // Writemotor(40, 150);
    }
    else if(d[6]){
      lst_slow = millis();
      slow_tracking();
      // Writemotor(145, 45);
    }
    else Writemotor(194, 200);
  }
  else if(d[2] == 1 && !d[1] && !d[3]){ Writemotor(194, 200); } // (172, 180)
  else if(d[2] == 1 && d[3] == 1){ Writemotor(155, 135); } 
  else if(d[2] == 1 && d[1] == 1){  Writemotor(138, 165); }
  else if(d[4] && !d[3]){ Writemotor(155, 75); }
  else if(d[4] && d[3]){ Writemotor(155, 85); }
  else if(d[0] && !d[1]){ Writemotor(76, 165); }
  else if(d[0] && d[1]){ Writemotor(85, 165); }
  else if(d[3]){ Writemotor(155, 120); }
  else if(d[1]){ Writemotor(120, 165); }
  // Writemotor(vl, vr);
}


/*
void slow_tracking(){
  for(int i = 0; i < 5; i++) d[i] = digitalRead(RF[i]);
  if(!(d[0] + d[1] + d[2] + d[3] + d[4])){  
    Writemotor(124, 130);
  }
  else if(d[2] == 1 && !d[1] && !d[3]){ Writemotor(124, 130); } // (172, 180)
  else if(d[2] == 1 && d[3] == 1){ Writemotor(110, 97); }
  else if(d[2] == 1 && d[1] == 1){  Writemotor(95, 110); }
  else if(d[4] && !d[3]){ Writemotor(80, 40); }
  else if(d[4] && d[3]){ Writemotor(80, 45); }
  else if(d[0] && !d[1]){ Writemotor(38, 85); }
  else if(d[0] && d[1]){ Writemotor(42, 85); }
  else if(d[3]){ Writemotor(100, 78); }
  else if(d[1]){ Writemotor(70, 100); }
}
*/
void slow_tracking(){
  for(int i = 0; i < 5; i++) d[i] = digitalRead(RF[i]);
  if(!(d[0] + d[1] + d[2] + d[3] + d[4])){  
    Writemotor(116, 120);
  }
  else if(d[2] == 1 && !d[1] && !d[3]){ Writemotor(116, 120); } // (172, 180)
  else if(d[2] == 1 && d[3] == 1){ Writemotor(110, 100); }
  else if(d[2] == 1 && d[1] == 1){ Writemotor(94, 110); }
  else if(d[4] && !d[3]){ Writemotor(80, 50); }
  else if(d[4] && d[3]){ Writemotor(80, 55); }
  else if(d[0] && !d[1]){ Writemotor(46, 80); }
  else if(d[0] && d[1]){ Writemotor(51, 80); }
  else if(d[3]){ Writemotor(100, 80); }
  else if(d[1]){ Writemotor(76, 100); }
}

double slow_arr[] = {-20, -10, 0, 10, 20};

void transition(){
  double powerCorrection = 0;
  for(int i = 0; i < 5; i++) powerCorrection += slow_arr[i] * digitalRead(RF[i]);
  int tp = 70;
  int vr = (tp - powerCorrection);
  int vl = 0.95 * (tp + powerCorrection);
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
void TurnLeft(unsigned long wait_ms = 240){
  unsigned long st = millis();
  st = millis();
  while(millis() - st < 60){
    Writemotor(20, 50);
  }
  stop(50);
  st = millis();
  while(millis() - st < wait_ms){
    Writemotor(-90, 100);
  }
  while(!digitalRead(RF[5])){
    Writemotor(-60, 70);
  }
  // for(int i = 0; i < 10; i++){
  //   Writemotor(-45, 70);
  // }
  stop(100);
  st = millis();
  while(millis() - st < 100){
    slow_tracking();
  }
}


void TurnRight(unsigned long wait_ms = 240){
  unsigned long st = millis();
  st = millis();
  while(millis() - st < 70){
    Writemotor(50, 20);
  }
  stop(50);
  st = millis();
  while(millis() - st < wait_ms){
    Writemotor(102, -100);
  }
  while(!digitalRead(RF[6])){
    Writemotor(75, -65);
  }
  for(int i = 0; i < 10; i++){
      Writemotor(75, -65);
  }
  stop(100);
  st = millis();
  while(millis() - st < 100){
    slow_tracking();
  }
}

void UTurn(unsigned long wait_ms = 400){
  unsigned long st = millis();
  stop(100);
  st = millis();
  while(millis()-st < wait_ms){
    Writemotor(-200, 70);
  }
  while(!digitalRead(RF[0])){
    Writemotor(-60, 25);
  }
  stop(300);
  // st = millis();
  // while(millis() - st < 250){
  //   slow_tracking();
  //   transition();
  // }
  
  // st = millis();
  // while(millis() - st < 300){
  //   slow_tracking();
  // }
}

void Forward(){
  while(ck() == 5){
    Writemotor(184, 190);
  }
  if(!ck()){
    stop(30);
    unsigned long st = millis();
    BT.println("read 0, rotate...");
    while(!d[4] && millis() - st < 400){
      Writemotor(50, -50);
      ck();
    }
    if(!lst_sum){
      while(!ck()){
        Writemotor(-50, 50);
      }
    }
  }
}