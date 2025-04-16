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
int extern IR[];
/*===========================import variable===========================*/

// TODO: add some function to control your car when encounter a node
// here are something you can try: left_turn, right_turn... etc.

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

void tracking_simple(){
    double error = 0;
    for(int i = 0; i < 5; i++) error += (i - 2) * digitalRead(IR[i]);
    int kp = 100;
    int vr = _Tp - kp * error;
    int vl = _Tp + kp * error;
    vl = min(vl, 255), vr = min(vr, 255);
    vl = max(vl, -255), vr = max(vr, -255);
    Writemotor(vl, vr);
}

void tracking(bool flag = false){ // PID Control
    static double kd = 0, lastError = 0, ki = 0, sumError = 0;
    if(flag) lastError = 0, sumError = 0;
    double error = 0;
    for(int i = 0; i < 5; i++) error += arr[i] * digitalRead(IR[i]);
    sumError += error; // I
    sumError = constrain(sumError, -20, 20); // max speed : [-200, 200]
    double dError = error - lastError; // D

    int kp = 35, tp = 150;
    int powerCorrection = kp * error + kd * dError + ki * sumError;
    lastError = error;
    int vr = (tp - powerCorrection) * (powerCorrection >= 0 ? 1 : 0.75);
    int vl = 0.95 * (tp + powerCorrection) * (powerCorrection <= 0 ? 1 : 0.75);
    int deduct = digitalRead(IR[0]) | digitalRead(IR[4]);
    if(deduct) vl *= 0.85, vr *= 0.85;
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

void TurnLeft(unsigned long wait_ms = 250){
    unsigned long start_time = millis();
    while(millis() - start_time < wait_ms){
        Writemotor(-125, 130);
    }
    while(!digitalRead(IR[0])){
        Writemotor(-70, 75);
    }
    start_time = millis();
    while(millis() - start_time < 50){
        Writemotor(100, 0);
        if(digitalRead(IR[2])) tracking();
    }
}


void TurnRight(unsigned long wait_ms = 240){
    unsigned long start_time = millis();
    start_time = millis();
    while(millis() - start_time < wait_ms){
        Writemotor(145, -180);
    }
    while(!digitalRead(IR[4])){
        Writemotor(55, -70);
    }
    start_time = millis();
    while(millis() - start_time < 50){
        Writemotor(0, 110);
        if(digitalRead(IR[2])) tracking();
    }
}

void UTurn(unsigned long wait_ms = 400){
    unsigned long start_time = millis();
    start_time = millis();
    while(millis()-start_time < wait_ms){
        Writemotor(80, -220);
    }
    while(!ck()){
        Writemotor(30, -80);
    }
    start_time = millis();
    stop(100);
}

void Forward(){
    unsigned long st = millis();
    while(millis() - st < 200 || ck() == 5){
        if(ck() == 5) Writemotor(255, 255);
        else tracking();
    }
}