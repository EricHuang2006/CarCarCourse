int PWM[] = {12, 13}; // A, B
int A[] = {2, 3}; // 1, 2
int B[] = {5, 6}; // 1, 2
int RF[] = {30, 32, 34, 36, 38};

void setup() {
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
    double error = 0;
    for(int i = 0; i < 5; i++) error += (i - 2) * digitalRead(RF[i]);
    int kp = 100, tp = 150;
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

int state = 0;

void loop() {
    if(ck() == 5){
        if(state == 0){ // 起點 -> 1，之後右轉
            while(ck() == 5){
                Writemotor(150, 150);
            }
            Writemotor(125, -125);
            delay(300);
            while(digitalRead(RF[2]) != 1){
                Writemotor(125, -125);
            }
        }
        else if (state == 1){ // 1 -> 2，之後迴轉
            while(ck()){
                Writemotor(150, 150);
            }
            while(digitalRead(RF[2]) != 1){
                Writemotor(125, -125);
            }
        }
        else if(state == 2){ // 2 -> 1，之後直走
            while(ck() == 5){
                Writemotor(150, 150);
            }
        }
        else if(state == 3){ // 1 -> 4，之後迴轉
            while(ck()){
                Writemotor(150, 150);
            }
            while(digitalRead(RF[2]) != 1){
                Writemotor(125, -125);
            }
        }
        else if(state == 4){ // 4 -> 1，之後左轉
            while(ck() == 5){
                Writemotor(150, 150);
            }
            Writemotor(-125, 125);
            delay(300);
            while(digitalRead(RF[2]) != 1){
                Writemotor(-125, 125);
            }
        }
        else{ // 1 -> 3，之後停止
            while(ck()){
                Writemotor(150, 150);
            }
            while(true){
                Writemotor(0, 0);
            }
        }
        state++;
    }
    tracking();
}
