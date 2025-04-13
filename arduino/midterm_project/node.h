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

void tracking(){
    double error = 0;
    for(int i = 0; i < 5; i++) error += (i - 2) * digitalRead(IR[i]);
    int kp = 100;
    int vr = _Tp - kp * error;
    int vl = _Tp + kp * error;
    vl = min(vl, 255), vr = min(vr, 255);
    vl = max(vl, -255), vr = max(vr, -255);
    Writemotor(vl, vr);
}


void L_Turn(unsigned long wait_ms = 650){
    unsigned long start_time = millis();
    while(millis() - start_time < wait_ms){
        Writemotor(85,255);
    }
    Writemotor(255,255);
}

void R_Turn(unsigned long wait_ms = 650){
    unsigned long start_time = millis();
    while(millis() - start_time < wait_ms){
        Writemotor(255,85);
    }
    Writemotor(255,255);
}

void U_Turn(unsigned long wait_ms = 575){
    while(ck() == 5){
        Writemotor(255, 255);
    }
    unsigned long start_time = millis();
    while(millis() - start_time < wait_ms){
        //Writemotor(175,-175); 700
        Writemotor(225,-225);
    }
    Writemotor(255,255);
}

void Forward(){
    unsigned long st = millis();
    while(millis() - st < 200 || ck() == 5){
        tracking();
    }
}