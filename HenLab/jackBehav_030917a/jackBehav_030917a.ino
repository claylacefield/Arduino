
#include <Tone.h>
Tone portTone1;
Tone portTone2;

int portBeam1 = 0; // analog input pins for beam break
int portBeam2 = 1;

int lickPin1 = 8;
int lickPin2 = 9;
int spkrPin1 = 10;
int spkrPin2 = 11;
int solPin1 = 4;
int solPin2 = 5;
int buttonPin = 6;

int analogThresh = 100;

int portVal1 = 0;
int portVal2 = 0;

int trialType = 0;

int stimFreq1 = 1000;
int stimFreq2 = 2000;
int stimDur = 1000;

int portNum = 0;

void setup() {
  pinMode(lickPin1, INPUT);
  pinMode(lickPin2, INPUT);
  pinMode(solPin1, OUTPUT);
  pinMode(solPin2, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  portTone1.begin(spkrPin1);
  portTone2.begin(spkrPin2);

  Serial.begin(9600);

  randomSeed(analogRead(3));

}

//////////////////////
void loop() {

  checkPorts();

  checkLicks();

}

//////////////////////
void checkLicks() {
  
}

//////////////////////
void checkPorts() {
  portVal1 = analogRead(portBeam1);
  portVal2 = analogRead(portBeam2);
  
  if (portVal1>analogThresh) {

    portNum = 1;
    
    if (random(0,100)>50) {
      trialType = 1;
      portTone1.play(stimFreq1, stimDur);
    }
    else {
      trialType = 2;
      portTone1.play(stimFreq2, stimDur);
    }
    
  }
  else if (portVal2>analogThresh) {

    portNum = 2;
    
    if (random(0,100)>50) {
      trialType = 1;
      portTone2.play(stimFreq1, stimDur);
    }
    else {
      trialType = 2;
      portTone2.play(stimFreq2, stimDur);
    }
    
  }
  
}

