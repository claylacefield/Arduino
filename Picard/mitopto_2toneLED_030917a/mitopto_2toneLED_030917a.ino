
// set variables
int stimFreq1 = 1;
int stimFreq2 = 40;
int stimDur = 1000;

// setup tones
#include <Tone.h>
Tone toneX;
Tone oroTone1;
Tone oroTone2;

// pins
int buttonPin = 12;
int ledPin1 = 8;
int ledPin2 = 10;
int xPin = 2;

// state vars
int buttonState = 0;
int buttonVal = 0;
int numReads = 0;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);

  toneX.begin(xPin);
  oroTone1.begin(ledPin1);
  oroTone2.begin(ledPin2);

  //Serial.begin(9600);
}

void loop() {
  buttonState = digitalRead(buttonPin);
  numReads = numReads+1;
  buttonVal = buttonVal + buttonState;

  if (numReads==5) {
    if (buttonVal==0) {
      startLedStim();
    }
    numReads = 0;
    buttonVal = 0;
  }
  
}

///////////////////
void startLedStim() {
  oroTone1.play(stimFreq1, stimDur);
  oroTone2.play(stimFreq2, stimDur);
}

