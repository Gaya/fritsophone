# Fritsophone

This is the arduino code for the Fritsophone, a musical instument controlled by light resistors.

## Requirements

- Teensy
- Teensy Audio Shield
- 7 ldr + 7 10k resistors
- 1 10k potentiometer
- a lot of wires

## How to use

1. Download the [latest version of the code](https://github.com/Gaya/fritsophone/archive/refs/heads/main.zip).
2. Unzip the folder and keep the files inside the `frits` folder.
3. Install [Teensyduino](https://www.pjrc.com/teensy/td_download.html).
4. Open `frits/frits.ino` in Teensyduino.
5. Adjust code and upload to the Teensy.

## Wiring

1. Each ldr gets one connection to Teensy's 5v on one leg.
2. The other leg goes to an analog (wire) out, then to a 10k resistor, then to Teensy's ground.
3. The analog outs go to analog pins on the Teensy. Adjust code to match pins.
4. Wire 10k pot to the volume wheel connections on the audio shield.
