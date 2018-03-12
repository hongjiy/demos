const int ledPin = 12;
int val;
void setup(){
  
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);

}

void loop(){
  
  if (Serial.available()) {
    //light(Serial.read() - '0');
    val = Serial.read();
    Serial.print(val);
}

  delay(500);

}

void light(int n){

  for (int i = 0; i < n; i++) {
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
    delay(100);
}

}


