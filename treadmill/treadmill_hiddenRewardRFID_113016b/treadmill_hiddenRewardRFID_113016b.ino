/*
treadmill_hiddenRewardRFID_(date).ino
Clay 2016

Serial code based on arduino SoftwareSerialExample

NOTE: 
This version is for Arduino UNO
(thus uses software serial for second serial port/rotary encoder)


111616
Added RFID code based upon Bildr tutorial.

112816
made state machine for reward solenoid
Turns on reward for lick during reward zone,
then checks after each rotary input to see if it's over.
Also records licks not in reward zone.

ToDo:
- make sure reward ends even if no rotary input
- make sure checkLicks doesn't interfere with rotary input

 */


#include <SoftwareSerial.h>
int rotSerPinRx = 2;
int rotSerPinTx = 3;
SoftwareSerial rotSerial(rotSerPinRx, rotSerPinTx); // RX, TX

int rfidSerPinRx = 6;
int rfidSerPinTx = 7;
SoftwareSerial rfidSerial(rfidSerPinRx, rfidSerPinTx); // RX, TX


// Other pins
int lickPin = 11;
int spkrPin = 5;
int ledPin = 13;

int solPin1 = 8; //5;
int solPin2 = 9; //6;

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

int prevRew = 0;
long rewStartTime = 0;
int rewDur = 50;

int prevLick = 0;

// RFID vars
int RFIDResetPin = 5;
char tag1[13] = "1E009A4067A3";

char tagString[13];
int index = 0;
boolean reading = false;

void setup()
{
  // set up pins
  pinMode(lickPin, INPUT);
  //pinMode(spkrPin, INPUT);
  pinMode(ledPin, INPUT);
  pinMode(solPin1, INPUT);
  pinMode(solPin2, INPUT);
  
  // Open serial communications
  Serial.begin(250000);//38400);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  Serial.println("Goodnight moon!");

  // set the data rate for the SoftwareSerial port
  rotSerial.begin(19200);
  
  //rotSerial.println("Hello, world?");// reserve 200 bytes for the inputString:
  inputString.reserve(200);

  // RFID
  rfidSerial.begin(9600); // need to change for correct DUE serial port
  pinMode(RFIDResetPin, OUTPUT);
  digitalWrite(RFIDResetPin, HIGH);

  
}  // end SETUP

// start LOOP
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

    if (prevRew == 1) {
      checkRewState();
    }

    checkLicks();

}  // end LOOP

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {

//  rotSerial.listen();
//  while (rotSerial.available()) {
//    // get the new byte:
//    char inChar = (char)rotSerial.read();
//    // add it to the inputString:
//    inputString += inChar;
//    // if the incoming character is a newline, set a flag
//    // so the main loop can do something about it:
//    if (inChar == '\n') {
//      stringComplete = true;
//    }
//  }

  // now look for RFID serial
  char tagString[13];
  int index = 0;
  boolean reading = false;

  rfidSerial.listen();

  while(rfidSerial.available()){

    int readByte = rfidSerial.read(); //read next available byte

    if(readByte == 2) reading = true; //begining of tag
    if(readByte == 3) reading = false; //end of tag

    if(reading && readByte != 2 && readByte != 10 && readByte != 13){
      //store the tag
      tagString[index] = readByte;
      index ++;
    }
  }

  checkTag(tagString); //Check if it is a match
  clearTag(tagString); //Clear the char of all value
  
}

void checkTag(char tag[]){
///////////////////////////////////
//Check the read tag against known tags
///////////////////////////////////

  if(strlen(tag) == 0) return; //empty, no need to contunue

  if(compareTag(tag, tag1)){ // if matched tag1, do this
    //lightLED(2);

  }else{
    Serial.println(tag); //read out any unknown tag
    digitalWrite(ledPin, HIGH);
    delay(1000);
    digitalWrite(ledPin, LOW);
    currPos = 0;
  }

}

void clearTag(char one[]){
///////////////////////////////////
//clear the char array by filling with null - ASCII 0
//Will think same tag has been read otherwise
///////////////////////////////////
  for(int i = 0; i < strlen(one); i++){
    one[i] = 0;
  }
}

boolean compareTag(char one[], char two[]){
///////////////////////////////////
//compare two value to see if same,
//strcmp not working 100% so we do this
///////////////////////////////////

  if(strlen(one) == 0) return false; //empty

  for(int i = 0; i < 12; i++){
    if(one[i] != two[i]) return false;
  }

  return true; //no mismatches
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

//    if (prevRew == 1) {
//      checkRewState();
//    }

    if (currPos >= rewBegPos && currPos <= rewEndPos) {
      if (digitalRead(lickPin) && prevRew == 0) {
        prevRew = 1;
        giveReward();
      }
      digitalWrite(ledPin, HIGH);
//      delay(1000);
//      digitalWrite(ledPin, LOW);
    }
    else if (currPos >= 2000) {
      currPos = 0;

    }
    else {
      digitalWrite(ledPin, LOW);
      prevRew = 0;  // reset reward if not in rew zone
    }
    
}

// reward functions
void giveReward() {
  rewStartTime = millis();
  digitalWrite(solPin1, HIGH);
  Serial.println("REWARD");
  Serial.println(rewStartTime);
}

void checkRewState() {
  if (prevRew == 1 && millis()-rewStartTime>=rewDur) {
    digitalWrite(solPin1, LOW);
    //prevRew = 0;
  }
}

void checkLicks() {
  if (prevLick == 0 && digitalRead(lickPin)== HIGH) {
    Serial.println("lick");
    Serial.println(millis());
    prevLick = 1;
  }
  else if (prevLick == 1 && digitalRead(lickPin)== LOW) {
    prevLick = 0;
  }
}


