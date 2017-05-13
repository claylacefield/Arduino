/*
Based on arduino SoftwareSerialExample

NOTE: 
This version is for Arduino UNO

 */
#include <SoftwareSerial.h>
int rotSerPinRx = 2;
int rotSerPinTx = 3;
SoftwareSerial rotSerial(rotSerPinRx, rotSerPinTx); // RX, TX

// Other pins
int lickPin = 4;
int spkrPin = 5;
int ledPin = 13;

int solPin1 = 5;
int solPin2 = 6;

// times
long startTime = 0;

//
int currPos = 0;
int prevPos = 0;
int dy = 0;
int vel = 0;

char inMess;
char outMess;

void setup()
{
  // set up pins
  pinMode(lickPin, INPUT);
  //pinMode(spkrPin, INPUT);
  pinMode(ledPin, INPUT);
  pinMode(solPin1, INPUT);
  pinMode(solPin2, INPUT);
  
  // Open serial communications
  Serial.begin(38400);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  Serial.println("Goodnight moon!");

  // set the data rate for the SoftwareSerial port
  rotSerial.begin(19200);
  //rotSerial.println("Hello, world?");
}

void loop() // run over and over
{
  if (rotSerial.available()) {
    //Serial.write(rotSerial.read());
//    inMess = rotSerial.read();
//
//    Serial.println(inMess);

    while (rotSerial.available() > 0) {
    char inMess = rotSerial.read();
    delayMicroseconds(100);
    Serial.write(inMess);
    //delayMicroseconds(100);
  }
  Serial.println();
    
//    yInd = inMess.indexOf("dy"); // index of "dy" in rotary encoder message
//    mInd = inMess.indexOf("millis");
//
//    dyStr = inMess.substring(yInd+3, mInd-4); 
//    dy = dyStr.toInt();
//    msStr = inMess.substring(mInd+8, inMess.length()-1); 
//    reTime = msStr.toInt();
//    dt = reTime - prevReTime;
//
//    vel = dy/dt;
//    currPos = prevPos + dy;
//
//    Serial.println("vel=" + vel);
//    Serial.println("currPos=" + currPos);
//
//    if (currPos >= rewBegPos && curPos <= rewEndPos) {
//      if (digitalRead(lickPin)) {
//        giveReward();
//      }
//    }
    
  }
//  if (Serial.available()) {
//    rotSerial.write(Serial.read());
//  }
}

