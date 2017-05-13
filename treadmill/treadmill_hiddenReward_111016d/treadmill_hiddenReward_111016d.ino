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

int rewBegPos = 500;
int rewEndPos = 600;

char inMess;
char outMess;

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

int yInd = 0;
int mInd = 0;
String dyStr = "";
String msStr = "";
int dt = 0;
long reTime = 0;
long prevReTime = 0;
//int currPos = 0;

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
  
  //rotSerial.println("Hello, world?");// reserve 200 bytes for the inputString:
  inputString.reserve(200);

  
}

void loop() // run over and over
{
  serialEvent(); //call the function
  // print the string when a newline arrives:
  if (stringComplete) {
    Serial.println(inputString);

    checkZone();
    
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
    

    
  //}
//  if (Serial.available()) {
//    rotSerial.write(Serial.read());
//  }
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (rotSerial.available()) {
    // get the new byte:
    char inChar = (char)rotSerial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

void checkZone() {

    prevPos = currPos;
    
    yInd = inputString.indexOf("dy"); // index of "dy" in rotary encoder message
    mInd = inputString.indexOf("millis");

    dyStr = inputString.substring(yInd+4, mInd-4); 
    Serial.println(dyStr);
    dy = dyStr.toInt();
    msStr = inputString.substring(mInd+8, inputString.length()-1); 
    reTime = msStr.toInt();
    dt = reTime - prevReTime;

    vel = dy/dt;
    currPos = prevPos + dy;

    Serial.println("vel=" + String(vel));
    Serial.println("currPos=" + String(currPos));

    if (currPos >= rewBegPos && currPos <= rewEndPos) {
//      if (digitalRead(lickPin)) {
//        giveReward();
//      }
      digitalWrite(ledPin, HIGH);
//      delay(1000);
//      digitalWrite(ledPin, LOW);
    }
    else if (currPos >= 2000) {
      currPos = 0;

    }
    else {
      digitalWrite(ledPin, LOW);
    }
    
}

