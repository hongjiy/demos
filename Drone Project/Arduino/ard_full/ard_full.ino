#include <PID_v1.h>

volatile int pwm_valueAuto = 0;
volatile int prev_timeAuto = 0;


const int PWMreadpin = 2;
const int echoPin = 3;
const int trigPin = 4; 
const int MuxPin = 5;
bool MuxOut = false;

int incoming[2];

int rollOut;
int pitchOut;
int thrustOut;

int thrustSetpoint;

const double rollP = 0.5;
const double rollI = 0.5;
const double rollD = 0.5;
const double pitchP = 0.5;
const double pitchI = 0.5;
const double pitchD = 0.5;
double thrustP, thrustI, thrustD;

PID rollPID(&incoming[0], &rollOut, 100,rollP,rollI,rollD, DIRECT);
PID pitchPID(&incoming[1], &pitchOut, 100,pitchP,pitchI,pitchD, DIRECT);
PID thrustPID(&incoming[2], &thrustOut, &thrustSetpoint,thrustP,thrustI,thrustD, DIRECT);


void setup() {
  // put your setup code here, to run once:

   Serial.begin(9600);
   attachInterrupt(PWMreadpin,risingAuto,RISING);
   pinMode(MuxPin, OUTPUT);

   pinMode(11,OUTPUT); //OC1A
   pinMode(12,OUTPUT); //OC1B
   TCCR1A= _BV(COM1A1) | _BV(COM1B1) | _BV(WGM11) | _BV(WGM10); // fast PWM
   TCCR1B = _BV(CS12) | _BV(CS10) | _BV(CS11); //lowest freq value mulitplier
   OCR1A = 31; //max value: 2ms
   OCR1B = 15; //min value: 1ms
   pinMode(9,OUTPUT); //OC2B
   pinMode(10,OUTPUT); //OC2A
   TCCR2A= _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20); // fast PWM
   TCCR2B = _BV(CS22) | _BV(CS20) | _BV(CS21); //lowest freq value mulitplier
   OCR2A = 31; //max value: 2ms
   OCR2B = 15; //min value: 1ms
}

void loop() {
  // put your main code here, to run repeatedly:

   if(pwm_valueAuto <1500 ){
 // manual mode   
  MuxOut = false;
  digitalWrite(MuxPin,MuxOut);
 }

 else{
 // auto mode
  MuxOut = true;
  digitalWrite(MuxPin,MuxOut);
  
 }

 while(Serial.available() >= 3){

    for (int i = 0; i < 3; i++){
      incoming[i] = Serial.read();
    
    }
 }
  if(incoming[2] == 1){
    ReadPingSensor();
    //SET Thrust PID parameters for Ultrasonic
  }
  else{
    //SET Thrust PID for RPi Size (incoming[2])
  }

  

}


void risingAuto(){

  attachInterrupt(PWMreadpin,fallingAuto,FALLING);
  prev_timeAuto = micros();
  
}

void fallingAuto(){

  attachInterrupt(PWMreadpin,risingAuto,RISING);
  pwm_valueAuto = micros() - prev_timeAuto;
  
}

void ReadPingSensor(){

  long duration, distance;
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  

}


