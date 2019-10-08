#include "arduinoFFT.h"
#include <Adafruit_NeoPixel.h>

#define LED_COUNT 256
#define LED_PIN D1
#define analogPin A0
#define PinReset D6
#define PinStrobe D7
#define SAMPLES 32             //Must be a power of 2
#define SAMPLING_FREQUENCY 1000 //Hz, must be less than 10000 due to ADC

arduinoFFT FFT = arduinoFFT();

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

unsigned int sampling_period_us;
unsigned long microseconds;

double vReal[SAMPLES];
double vImag[SAMPLES];

float r, g, b, mapped;

int caseId, rollId = 0, ledCursor = 0, finalCursor = 0;

int ledFullMatrix[ 17 ][ 17 ];

void setup() {
  
  createMatrix();

  strip.begin();
  strip.setBrightness(255);
  strip.show();

  pinMode(analogPin, INPUT);
  pinMode(PinStrobe, OUTPUT);
  pinMode(PinReset, OUTPUT);

  digitalWrite(PinReset, LOW);
  digitalWrite(PinStrobe, HIGH);
  
}

void loop() {

  equalizer();

  strip.show();

}

void createMatrix() {

  for ( int i = 1; i <= 16; i++ ) {

    for ( int j = 1; j <= 16; j++ ) {

      if ( i % 2 == 0 )
        ledFullMatrix[ i ][ j ] = ( i * 16 - 16 ) + ( 16 - j );
      else
        ledFullMatrix[ i ][ j ] = ( i * 16 - 16 ) + j - 1;

    }
    
  }
  
}

void equalizer() {

  digitalWrite(PinReset, HIGH);
  digitalWrite(PinReset, LOW);

  for (int i = 0; i < 7; i++) {

    digitalWrite(PinStrobe, LOW);
    delayMicroseconds(40);

    bands[i] = analogRead(0);
    Serial.println( "Band " + (String) i + " ==> " + (String) bands[i] );

    digitalWrite(PinStrobe, HIGH);
    delayMicroseconds(40);

  }

  for (int i = 0; i < 7; i++) {

    mapped = map(round(bands[i]), 1000, 100, 0, 16);

    for ( int j = 1; j <= mapped; j++ )
      strip.setPixelColor( ledFullMatrix[j][i], 255, 255, 255, 0 );

    for ( int k = 16; k > mapped; k-- )
      strip.setPixelColor( ledFullMatrix[k][i], 0, 0, 0 );

  }

}
