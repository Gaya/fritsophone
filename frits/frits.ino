#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include "Viola_samples.h"

const int WAVETABLES = 7;
const int MIXERS = WAVETABLES + 1;
const float RELIEF = 0.1;
float initialLdr[7];
float pins[7] = { A0, A2, A3, A10, A11, A13, A12 };

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

  Serial.print("Setup LDR");
  // setup ldr
  for (unsigned int i = 0; i < sizeof(pins) / sizeof(int); i++) {
    pinMode(pins[i], INPUT);
    initialLdr[i] = analogRead(pins[i]);  
  }

  Serial.println("Setup volume pot");
  pinMode(A1, INPUT);

  Serial.println("Setup audio");
  AudioMemory(120);
  sgtl5000_1.enable();
  sgtl5000_1.volume(1);

  Serial.println("Seting mixer gains");
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

  Serial.println("Initialize wavetables");
  // C E G B D F A
  float notes[7] = { 261.63, 329.63, 392.00, 493.88, 587.33, 698.46, 880.00 };

  for (int i = 0; i < WAVETABLES; i++) {
    wavetable[i].setInstrument(Viola);
    wavetable[i].playFrequency(notes[i]);
    wavetable[i].amplitude(0);
  }

  Serial.println("Setup done");
}

void loop() {
  int volumeReading = analogRead(A1);
  float volume = map(volumeReading, 0, 1024, 0, 100);

  // set overall volume
  sgtl5000_1.volume(volume / 100);

  // determine amplitudes of wavetables
  for (unsigned int i = 0; i < sizeof(pins) / sizeof(int); i++) {
    int reading = analogRead(pins[i]);
    float ampl = map(reading, 0, initialLdr[i] - (initialLdr[i] * RELIEF), 100, 0);
    wavetable[i].amplitude(ampl / 100);
  }
}
