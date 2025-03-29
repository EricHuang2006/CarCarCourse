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

void U_Turn(){
    // temporary, will change
    while(ck()){
        Writemotor(150, 150);
    }
    while(digitalRead(IR[2]) != 1){
        Writemotor(125, -125);
    }
}

void L_Turn(){
    while(ck() >= 3){
        Writemotor(150, 150);
    }
    // Writemotor(150, 150);
    // delay(700);
    Writemotor(-125, 125);
    delay(550);
    Writemotor(150, 150);
    delay(300);
    // while(digitalRead(IR[2]) != 1){
    //     Writemotor(-125, 125);
    // }
}

void R_Turn(){
    while(ck() == 5){
        Writemotor(150, 150);
    }
    Writemotor(125, -125);
    delay(300);
    while(digitalRead(IR[2]) != 1){
        Writemotor(125, -125);
    }
}