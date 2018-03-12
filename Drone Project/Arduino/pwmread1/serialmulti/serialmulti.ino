
// create array
int incoming[] = {0,0,0};
const int ledPin = 5;

void setup(){

  pinMode(ledPin, OUTPUT);

  Serial.begin(9600);
  
}

void loop(){
  while(Serial.available() >= 3){
    // fill array
    for (int i = 0; i < 3; i++){
      incoming[i] = Serial.read();
    }
    // use the values
   
  }

  if (incoming[0] < 100 && incoming[1] < 100){
    digitalWrite(6, HIGH);
    }
  
  else{
    digitalWrite(6, LOW);
    }

    if (incoming[2] == 255){
      digitalWrite(ledPin, HIGH);
    }
  else{
    digitalWrite(ledPin, LOW);
   }
}
