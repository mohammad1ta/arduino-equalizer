# Arduino-Equalizer

<div>
  <!-- Version -->
  <a href="https://marktext.github.io/website">
    <img src="https://badge.fury.io/gh/mohammad1ta%2Farduino-equalizer.svg" alt="website">
  </a>
  <!-- Stars -->
  <a href="https://github.com/mohammad1ta/arduino-equalizer">
    <img src="https://img.shields.io/github/stars/mohammad1ta/arduino-equalizer.svg" alt="Stars">
  </a>
</div>
  
Processing of audio signals and showing in RGB LED Stripe as equalizer ( or spectrum )

## Video
[![Example of equalizer](https://img.youtube.com/vi/KQLnBpG5aBg/2.jpg)](https://youtu.be/KQLnBpG5aBg "Arduino equalizer example") 

## Requirements
- NodeMCU or another board ( like Arduino Uno and etc ).
- Audio input lines ( Ground + L or R )
- RGB LED Stripe ( or non RGB )

## Initialize board
1. Connect R line of your audio input to analog pin ( A0 ) and Ground line to ground of the board.
2. Connect Data line of your LED Stripe to D1 pin.
3. Upload equalizer.ino to your board.
