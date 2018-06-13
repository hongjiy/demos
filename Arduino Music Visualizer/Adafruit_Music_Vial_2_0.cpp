/*********************************************************************************************
 * Arduino Music Vissulizer
 * Author: Hongji Yang
 *
 * This program is licensed under a GPLv3 License
 **********************************************************************************************/

#include <Adafruit_NeoPixel.h>
#include "WS2812_Definitions.h"
#include <avr/pgmspace.h>
#include <math.h>
#include <Wire.h>


#define OUT_PIN 4
#define IN_PIN 1
#define LED_COUNT 47
#define FLOW_RATE 10  // how fast rainbow flow
#define SPARK_LEVEL 0 // intensity of spark
#define AUX_LOW 55    // sensitivity lower bound
#define AUX_HIGH 110 // sensitivity upper bound
#define DYNAMIC       // make LEDs random shut off due to counter overflow 
#define ADC_CHANNEL 2

int flow = 0;
int rainbowScale = 192 / LED_COUNT * 2;
int vol = 0;

//initialze globle LED object
Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_COUNT, OUT_PIN, NEO_RGB + NEO_KHZ800);
void setup()
{
  Serial.begin(9600); 
  leds.begin();
  leds.show();
  leds.setBrightness(64);
}

void loop()
{
  int total = 0;
  int counter = 0;
  vol = 0;
  //integrate voltage to get power
  for(int i = 0; i < 100; i++)
  {
      vol += analogRead(IN_PIN);
  }
  vol = vol/100;  //Normalize summation
  //vol = map(vol, AUX_LOW, AUX_HIGH, 15, 120);   //140 LEDs
  vol = map(vol, AUX_LOW, AUX_HIGH, 5, 20);   //45 LEDs
  //20,95 for 95 LEDs
  //15,140 f0r 150 LEDs
  //print_vol(analogRead(IN_PIN));  //use this for tuning
  rainbow();  //update LED configuration
}


void print_vol(int vol)
{
/*
//use this to print in volume graphically
 for(int i = 0; i < vol; i++)
 {
 Serial.print("-");
 }
*/
  Serial.print(vol);
  Serial.print("\n");
}
// Turn off all LEDs with this function
void clearLEDs()
{
  for (int i=0; i<LED_COUNT; i++)
  {
    leds.setPixelColor(i, 0);
  }
}
// configure LED color in a rainbow spectrum order
void rainbow() 
{
  int spark = 0;
  int i = 0;
  static int counter = 0;
  if(flow > LED_COUNT)  flow = 0;
  //set left side half LEDs color
  for (i = 0; i<vol; i++)
  {
    leds.setPixelColor(LED_COUNT/2 - i, rainbowOrder((rainbowScale * (i + flow)) % 192));
  }
  for (i; i<LED_COUNT/2 + 1; i++)
  {
#ifdef DYNAMIC
    leds.setPixelColor(LED_COUNT/2 - i, BLACK);
#else
    leds.setPixelColor(LED_COUNT/2 - i, rainbowOrder((rainbowScale * (i + flow)) % 192));
#endif 
  }
  //set right side half LEDs color
  for (i = 0; i < vol; i++)
  {
    leds.setPixelColor( LED_COUNT/2 + i, rainbowOrder((rainbowScale * (i + flow)) % 192));
  }
  for (i; i<LED_COUNT/2 + 1; i++)
  {
#ifdef DYNAMIC
    leds.setPixelColor(LED_COUNT/2 + i, BLACK);
#else
    leds.setPixelColor(LED_COUNT/2 + i, rainbowOrder((rainbowScale * (i + flow)) % 192));
#endif 
  }
  // determine where to spark
   for(i = 0; i < SPARK_LEVEL; i++)
   {
   spark = random(LED_COUNT);
   leds.setPixelColor(spark, WHITE);
   }
  //flow++;
  counter++;
  if( (counter%2) == 0)  flow++;
  //Serial.print((counter));
  //Serial.print("\n");
  leds.show();
  //delay(FLOW_RATE);
  }

uint32_t rainbowOrder(byte position) 
{

  if (position < 31)  // Red -> Yellow (Red = FF, blue = 0, green goes 00-FF)
  {
    return leds.Color(0xFF, position * 8, 0);
  }
  else if (position < 63)  // Yellow -> Green (Green = FF, blue = 0, red goes FF->00)
  {
    position -= 31;
    return leds.Color(0xFF - position * 8, 0xFF, 0);
  }
  else if (position < 95)  // Green->Aqua (Green = FF, red = 0, blue goes 00->FF)
  {
    position -= 63;
    return leds.Color(0, 0xFF, position * 8);
  }
  else if (position < 127)  // Aqua->Blue (Blue = FF, red = 0, green goes FF->00)
  {
    position -= 95;
    return leds.Color(0, 0xFF - position * 8, 0xFF);
  }
  else if (position < 159)  // Blue->Fuchsia (Blue = FF, green = 0, red goes 00->FF)
  {
    position -= 127;
    return leds.Color(position * 8, 0, 0xFF);
  }
  else  //160 <position< 191   Fuchsia->Red (Red = FF, green = 0, blue goes FF->00)
  {
    position -= 159;
    return leds.Color(0xFF, 0x00, 0xFF - position * 8);
  }
}


