volatile int pwm_valueAuto = 0;
volatile int prev_timeAuto = 0;

volatile int pwm_valueRpi = 0;
volatile int prev_timeRpi = 0;

int ThrustIn = 2;
int ThrustOut = 3;
int PitchIn = 4;
int PitchOut = 5;
int RollIn = 6;
int RollOut = 7;
int YawIn = 8;
int YawOut = 9;
bool MuxOut = false;


void setup() {
  // put your setup code here, to run once:

   Serial.begin(115200);
   attachInterrupt(0,risingAuto,RISING);
   attachInterrupt(1,risingRpi,RISING);
  
}

void loop() {
  // put your main code here, to run repeatedly:

 if(pwm_value <1500 ){
 // manual mode   
  Muxout = false;
  digitalWrite(11,Muxout);
 }

 else{
 // auto mode
  Muxout = true;
  digitalWrite(11,Muxout);
  
  
 }
}

void risingAuto(){

  attachInterrupt(0,fallingAuto.FALLING);
  prev_timeAuto = micros();
  
}

void fallingAuto(){
  
  attachInterrupt(0,risingAuto,RISING);
  pwm_valueAuto = micros() - prev_timeAuto;
  Serial.println(pwm_valueAuto);  
  
}

void risingRpi(){

  attachInterrupt(1,fallingRpi.FALLING);
  prev_timeRpi = micros();
  
}

void fallingRpi(){
  
  attachInterrupt(1,risingRpi,RISING);
  pwm_valueRpi = micros() - prev_timeRpi;
  Serial.println(pwm_valueRpi);  
  
}

