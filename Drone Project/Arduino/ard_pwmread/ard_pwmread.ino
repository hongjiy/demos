volatile int pwm_value = 0;
volatile int pwm_temp_value = 0;
volatile int prev_time = 0;


bool muxout = false;


void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);
   attachInterrupt(digitalPinToInterrupt(2),change,CHANGE); //PIN 2



   pinMode(6,OUTPUT); //OC4A
   pinMode(7,OUTPUT); //OC4B
   pinMode(8,OUTPUT); //OC4C???
   TCCR4A= _BV(COM4A1) | _BV(COM4B1) | _BV(COM4C1) | _BV(WGM41) | _BV(WGM40); // fast PWM
   TCCR4B = _BV(CS42) | _BV(CS40) | _BV(CS41); //lowest freq value mulitplier
   OCR4A = 31; //max value: 2ms
   OCR4B = 15; //min value: 1ms
   OCR4C = 19;
   pinMode(9,OUTPUT); //OC2B
   pinMode(10,OUTPUT); //OC2A
   TCCR2A= _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20); // fast PWM
   TCCR2B = _BV(CS22) | _BV(CS20) | _BV(CS21); //lowest freq value mulitplier
   OCR2A = 31; //max value: 2ms
   OCR2B = 15; //min value: 1ms


}

void loop() {
  // put your main code here, to run repeatedly:

  
  
   if(pwm_value <1500 ){
 // manual mode   
  muxout = false;
  digitalWrite(8,muxout);
  Serial.print("pwm_valueAuto: ");
  Serial.println(pwm_value);
 }

 else{
 // auto mode
  muxout = true;
  digitalWrite(8,muxout);
  Serial.println(pwm_value); 
  
  
 }

}


void change() {

pwm_temp_value = micros()-prev_time;
prev_time = micros();
if(pwm_temp_value < 2500){
  pwm_value = pwm_temp_value;
}
}

