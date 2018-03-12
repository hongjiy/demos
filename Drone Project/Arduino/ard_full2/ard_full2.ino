#include <PID_v1.h>

//pwm read interupt
volatile int pwm_value = 0;
volatile int pwm_temp_value = 0;
volatile int prev_time = 0;

//PIN definitions
const int PWMreadpin = 0; //Digital PIN 2
const int echoPin = 3;
const int trigPin = 4;
const int MuxPin = 5;
const int LedPin = 6;

//Output pin variable
bool MuxOut = false;

//PING sensor variables
long duration, distance;

//serial read variables
double incoming[] = {0,0,0};
double Ztarget = 0;

//PID output variables
double rollOut = 0;
double pitchOut = 0;
double thrustOut = 0;

//SET PID constants
double thrustSetpoint;

double rollP = 4;
double rollI = 1;
double rollD = 0.1;
double pitchP = 4;
double pitchI = 1;
double pitchD = 0.1;
double thrustP = 1 , thrustI = 1, thrustD = 0.1;
double Setpoint = 100;
double Setpoint70 = 70;

//SET PID output limits //124 = min 249 = max
//161 = 1300 211 = 1700
//186 = 1500 173 = 1400 148 = 1200
const int rolllow = 173;
const int rollhigh = 199;
const int pitchlow = 173;
const int pitchhigh = 199;
const int thrustlow = 148;
const int thrusthigh = 173;

//Serial Loss of Communication Detection Variables
double lastreceivetime = 0;
double timedifference = 0;

//consturct PID loops
PID rollPID(&incoming[0], &rollOut, &Setpoint, rollP, rollI, rollD, DIRECT);
PID pitchPID(&incoming[1], &pitchOut, &Setpoint70, pitchP, pitchI, pitchD, REVERSE);
PID thrustPID(&Ztarget, &thrustOut, &thrustSetpoint, thrustP, thrustI, thrustD, REVERSE);



void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  attachInterrupt(PWMreadpin, change, CHANGE); //PWMreadpin 0 = PIN 2
  pinMode(MuxPin, OUTPUT);
  rollPID.SetOutputLimits(rolllow, rollhigh);
  pitchPID.SetOutputLimits(pitchlow, pitchhigh);
  thrustPID.SetOutputLimits(thrustlow, thrusthigh);

  rollPID.SetMode(AUTOMATIC);
  pitchPID.SetMode(AUTOMATIC);
  thrustPID.SetMode(AUTOMATIC);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(LedPin, OUTPUT);


/*
   pinMode(11,OUTPUT); //OC4C???
   pinMode(12,OUTPUT); 
   TCCR0A= _BV(COM0A1) | _BV(COM0B1) | _BV(WGM01) | _BV(WGM00) ; // fast PWM
   TCCR0B = _BV(CS02) | _BV(CS00) ; //lowest freq value mulitplier
   OCR0A = 31; //max value: 2ms
   OCR0B = 15; //min value: 1ms
*/

 // Set PB1/2 as outputs.
 //I think these are on timer 1, and using phase correct PWM (second one)
  DDRB |= (1 << DDB5) | (1 << DDB6);

  TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM10); 
  TCCR1B =  (1 << CS11)| (1 << CS10); //480ish Hz
  

  OCR1B = 23; //pin 12 (YAW = 1500)
  //test
  OCR1A = 240;
  OCR1B = 120;


  
   pinMode(9,OUTPUT); //OC2B
   pinMode(10,OUTPUT); //OC2A
   //TCCR2A= _BV(COM2A1) | _BV(COM2B1) | _BV(WGM20); // phase correct PWM
   TCCR2A= _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20); // fast PWM
   TCCR2B = _BV(CS22) | _BV(CS20); //second lowest freq value mulitplier (480hz)
   //TCCR2B = _BV(CS22) | _BV(CS20) | _BV(CS21); //lowest freq value mulitplier (61 Hz)
   OCR2A = 120; //max value: 2ms
   OCR2B = 240; //min value: 1ms
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(pwm_value)

  if (pwm_value < 1500 ) {
    // manual mode
    MuxOut = false;
    digitalWrite(MuxPin, MuxOut);
  }

  else {
    // auto mode
    MuxOut = true;
    digitalWrite(MuxPin, MuxOut);

  }
 

  while (Serial.available() >= 3) {

    for (int i = 0; i < 3; i++) {
      incoming[i] = Serial.read();
      lastreceivetime = micros();
    }
  }
  Ztarget = incoming[2];

 timedifference = micros() - lastreceivetime;
 

  if (incoming[2] == 255 || timedifference > 100000 ) {//[ || timedifference > 1000000
   
    //digitalWrite(MuxPin, LOW);
    ReadPingSensor();
    Ztarget = distance;
    //SET Thrust PID parameters for Ultrasonic
    thrustPID.SetControllerDirection(DIRECT);
    thrustSetpoint = 100;
    thrustP = .5;
    thrustI = .5;
    thrustD = 0.05;


  }
  else {
   // digitalWrite(MuxPin, HIGH);
  //SET Thrust PID for RPi Size (incoming[2])
    thrustPID.SetControllerDirection(REVERSE);
    thrustSetpoint = 30;
    thrustP = 1;
    thrustI = 1;
    thrustD = 0.1;
    
    rollPID.Compute();
    pitchPID.Compute();
    thrustPID.Compute();

  }
 
 // rollPID.Compute();
 // pitchPID.Compute();
 // thrustPID.Compute();

   if (incoming[2] == 255 || timedifference > 1000000) { //|| timedifference > 1000000
    //rollOut = 186; //=1497
    //pitchOut = 186;

    rollOut = 186; //low high test
    pitchOut = 186;

    digitalWrite(LedPin, LOW);
    
  }
  else{
    digitalWrite(LedPin, HIGH);
  }


  // pin 9            0-255, want to be 15 -31
  OCR2A = rollOut;
  // pin 10
  OCR2B = pitchOut;
  // pin 11
  OCR1A = thrustOut;//thrustOut;



}

//interrupt servie routine
void change() {

pwm_temp_value = micros()-prev_time;
prev_time = micros();
if(pwm_temp_value < 2500){
  pwm_value = pwm_temp_value;
}
}

//measure sonar distance
void ReadPingSensor() {

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;
 



}


