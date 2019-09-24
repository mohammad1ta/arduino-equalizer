#include "arduinoFFT.h"
#include <Adafruit_NeoPixel.h>

#define LED_COUNT 300
#define LED_PIN D1

#define SAMPLES 32 // Must be a power of 2
#define SAMPLING_FREQUENCY 1000 // Hz, must be less than 10000 due to ADC

arduinoFFT FFT = arduinoFFT();

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

unsigned int sampling_period_us;
unsigned long microseconds;

int r, g, b, mapped;

double vReal[SAMPLES];
double vImag[SAMPLES];

// Define address of ordered LEDs
int ledMatrix[15][10] = {
  { 284, 254, 223, 192, 162, 131, 100, 69, 38, 7 },
  { 285, 255, 224, 193, 163, 132, 101, 70, 39, 8 },
  { 286, 256, 225, 194, 164, 133, 102, 71, 40, 9 },
  { 287, 257, 226, 195, 165, 134, 103, 72, 41, 10 },
  { 288, 258, 227, 196, 166, 135, 104, 73, 42, 11 },
  { 289, 259, 228, 197, 167, 136, 105, 74, 43, 12 },
  { 290, 260, 229, 198, 168, 137, 106, 75, 44, 13 },
  { 291, 261, 230, 199, 169, 138, 107, 76, 45, 14 },
  { 292, 262, 231, 200, 170, 139, 108, 77, 46, 15 },
  { 293, 263, 232, 201, 171, 140, 109, 78, 47, 16 },
  { 294, 264, 233, 202, 172, 141, 110, 79, 48, 17 },
  { 295, 265, 234, 203, 173, 142, 111, 80, 49, 18 },
  { 296, 266, 235, 204, 174, 143, 112, 81, 50, 19 },
  { 297, 267, 236, 205, 175, 144, 113, 82, 51, 20 },
  { 298, 268, 237, 206, 176, 145, 114, 83, 52, 21 }
};

void setup() {

  sampling_period_us = round( 1000000 * ( 1.0 / SAMPLING_FREQUENCY ) );

  strip.begin();
  strip.setBrightness(255);
  strip.show();
  
}

void loop() {

  equalizer();

  strip.show();

}

void equalizer() {

  for (int i = 0; i < SAMPLES; i++) {
  
    microseconds = micros(); // Overflows after around 70 minutes!

    vReal[i] = analogRead(0);
    vImag[i] = 0;
    
    while (micros() - microseconds < sampling_period_us) {
      //empty loop
    }
    
    microseconds += sampling_period_us;
    
  }

  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
  double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);

  for (int i = 0; i < SAMPLES / 2; i++) {

    // Because below of 2, have noise
    if ( i >= 2 ) {

      // Map value of bands to 10 LED ( Height of equalizer )
      mapped = map( round( vReal[i] ) - 1, 0, 60, 0, 10 );

      if ( mapped > 10 ) 
        mapped = 10;

      for ( int j = 0; j < mapped; j++ ) {

        r = j * 28;
        g = 255 - r;
        b = 0;
        
        strip.setPixelColor( ledMatrix[i - 2][j], r, g, b, 0 );

      }

      for ( int k = 10; k > mapped; k-- )
        strip.setPixelColor( ledMatrix[i - 2][k], 0, 0, 0 );

    }

  }

}
