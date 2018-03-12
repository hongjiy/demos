#include <PID_v1.h>


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

    if (val = 400400){
      digitalWrite(ledPin, HIGH);
    }
    else{
      digitalWrite(ledPin, LOW);
    }


}



