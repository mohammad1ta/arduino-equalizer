#include "arduinoFFT.h"
#include <Adafruit_NeoPixel.h>

#define LedCount 256
#define PIN_LED D1
#define PIN_ANALOG A0
#define PIN_RESET D6
#define PIN_STROBE D7
#define SAMPLES 32             //Must be a power of 2
#define SAMPLING_FREQUENCY 1000 //Hz, must be less than 10000 due to ADC
#define numVertical 16
#define numHorizontal 16

unsigned int sampling_period_us;
unsigned long microseconds;

double vReal[SAMPLES];
double vImag[SAMPLES];

float r, g, b, mapped;

int ledFullMatrix[ numVertical + 1 ][ numVertical + 1 ];
int caseId, rollId = 0, ledCursor = 0, finalCursor = 0;

arduinoFFT FFT = arduinoFFT();

Adafruit_NeoPixel strip(LedCount, PIN_LED, NEO_GRB + NEO_KHZ800);

void setup() {
  
  createMatrix();

  strip.begin();
  strip.setBrightness(255);
  strip.show();

  pinMode(PIN_ANALOG, INPUT);
  pinMode(PIN_STROBE, OUTPUT);
  pinMode(PIN_RESET, OUTPUT);

  digitalWrite(PIN_RESET, LOW);
  digitalWrite(PIN_STROBE, HIGH);
  
}

void loop() {

  equalizer();

  strip.show();

}

void createMatrix() {

  for ( int i = 1; i <= numVertical; i++ ) {

    for ( int j = 1; j <= numHorizontal; j++ ) {

      if ( i % 2 == 0 )
        ledFullMatrix[ i ][ j ] = ( i * numVertical - numVertical ) + ( numVertical - j );
      else
        ledFullMatrix[ i ][ j ] = ( i * numHorizontal - numHorizontal ) + j - 1;

    }

  }
  
}

void equalizer() {

  digitalWrite(PIN_RESET, HIGH);
  digitalWrite(PIN_RESET, LOW);

  for (int i = 0; i < 7; i++) {

    digitalWrite(PIN_STROBE, LOW);
    delayMicroseconds(40);

    bands[i] = analogRead(0);
    Serial.println( "Band " + (String) i + " ==> " + (String) bands[i] );

    digitalWrite(PIN_STROBE, HIGH);
    delayMicroseconds(40);

  }

  for (int i = 0; i < 7; i++) {

    mapped = map( round( bands[i] ), 1000, 100, 0, 16);

    for ( int j = 1; j <= mapped; j++ )
      strip.setPixelColor( ledFullMatrix[j][i], 255, 255, 255, 0 );

    for ( int k = 16; k > mapped; k-- )
      strip.setPixelColor( ledFullMatrix[k][i], 0, 0, 0 );

  }

}
