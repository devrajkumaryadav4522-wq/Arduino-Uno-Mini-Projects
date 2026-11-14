// Traffic Light Controller

int gL = 8; //gl=greenLED
int yL = 9; //yL=yellowLED
int rL = 12; //rL=redLED

void setup( ) {
  pinMode(gL, OUTPUT);
  pinMode(yL, OUTPUT);
  pinMode(rL, OUTPUT);
}

void loop( ) {
  //GREEN LIGHT ON
  digitalWrite(gL, HIGH);
  digitalWrite(yL,LOW);
  digitalWrite(rL, LOW);
  delay(5000);
  
    //YRLLOW LIGHT ON
  digitalWrite(gL, LOW);
  digitalWrite(yL, HIGH);
  digitalWrite(rL, LOW);
  delay(2000);
  
    //RED LIGHT ON
  digitalWrite(gL, LOW);
  digitalWrite(yL, LOW);
  digitalWrite(rL, HIGH);
  delay(5000);
  
}
