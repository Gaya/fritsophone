#include <Bounce.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include "Viola_samples.h"

const int WAVETABLES = 7;
const int MIXERS = WAVETABLES + 1;

AudioSynthWavetable wavetable[WAVETABLES];
AudioOutputI2S i2s1;
AudioMixer4 mixer[3];
AudioConnection patchCord[] = {
  { wavetable[0], 0, mixer[0], 0 },
  { wavetable[1], 0, mixer[0], 1 },
  { wavetable[2], 0, mixer[0], 2 },
  { wavetable[3], 0, mixer[0], 3 },
  { wavetable[4], 0, mixer[1], 0 },
  { wavetable[5], 0, mixer[1], 1 },
  { wavetable[6], 0, mixer[1], 2 },
  { mixer[0], 0, mixer[2], 0 },
  { mixer[1], 0, mixer[2], 1 },
  { mixer[2], 0, i2s1, 0 },
  { mixer[2], 0, i2s1, 1 }
};
AudioControlSGTL5000 sgtl5000_1;

void setup() {
  // start serial output
  Serial.begin(9600); 
  
  // setup ldr
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(A6, INPUT);

  // setup pot
  pinMode(A9, INPUT);

  // setup audio
  AudioMemory(20);
  sgtl5000_1.enable();
  sgtl5000_1.volume(1);

  // set correct levels for mixers
  mixer[0].gain(0, 0.25);
  mixer[0].gain(1, 0.25);  
  mixer[0].gain(2, 0.25);
  mixer[0].gain(3, 0.25);  
  mixer[1].gain(0, 0.25);
  mixer[1].gain(1, 0.25);  
  mixer[1].gain(2, 0.25);
  mixer[1].gain(3, 0.25);
  mixer[2].gain(0, 0.5);  
  mixer[2].gain(1, 0.5);

  // C E G B D F A
  float notes[7] = { 261.63, 329.63, 392.00, 493.88, 587.33, 698.46, 880.00 };

  for (int i = 0; i < WAVETABLES; i++) {
    wavetable[i].setInstrument(Viola);
    wavetable[i].playFrequency(notes[i]);
    wavetable[i].amplitude(0);
  }
}

void loop() {
  // read inputs
  int potReadings[1] = { analogRead(A9) };
  int ldrReadings[7] = { analogRead(A6), analogRead(A5), analogRead(A4), analogRead(A3), analogRead(A2), analogRead(A1), analogRead(A0) };

  // set overall volume
  sgtl5000_1.volume(potReadings[0] / 1024);

  // determine amplitudes of wavetables
  float amplitude1 = map(ldrReadings[0], 0, 1024, 100, 0);
  float amplitude2 = map(ldrReadings[1], 0, 1024, 100, 0);
  float amplitude3 = map(ldrReadings[2], 0, 1024, 100, 0);
  float amplitude4 = map(ldrReadings[3], 0, 1024, 100, 0);
  float amplitude5 = map(ldrReadings[4], 0, 1024, 100, 0);
  float amplitude6 = map(ldrReadings[5], 0, 1024, 100, 0);
  float amplitude7 = map(ldrReadings[6], 0, 1024, 100, 0);

  // set individual volumes
  wavetable[0].amplitude(amplitude1 / 100);
  wavetable[1].amplitude(amplitude2 / 100);
  wavetable[2].amplitude(amplitude3 / 100);
  wavetable[3].amplitude(amplitude4 / 100);
  wavetable[4].amplitude(amplitude5 / 100);
  wavetable[5].amplitude(amplitude6 / 100);
  wavetable[6].amplitude(amplitude7 / 100);
}
