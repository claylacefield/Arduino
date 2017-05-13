// Sucrose Preference task
// Clay Lacefield 
// col8@columbia.edu
// July 29, 2016


// MPR121 touch sensor (lick detection)
#include "mpr121.h"
#include <Wire.h>

int irqpin = 2;  // Digital 2
boolean touchStates[12]; //to keep track of the previous touch states

// other pins/variables

// speaker/tone
int spkrPin = 10;
int tone1freq = 2000;

// pushbutton
int button1pin = 9;
int button1state = 1; 

// solenoid valves
int sol1pin = 5;
int sol2pin = 4;

int solPins[2] = {4, 5};

int sol1dur = 50;
int sol2dur = 50;

int solDurs[2] = {50, 50};

// other pins
int ledPin = 13;
int servoPin = 12; // not used here but just pin placeholder
int trigPin = 11; // for triggering 2p scans or receiving trig from ethovision

int sessionStart = 0;
int sessionStartTime = 0;
int baselineDur = 60000;
long sessionDur = 20*60000;

long lickTime = 0;

void setup(){

  // MPR121 setup
  pinMode(irqpin, INPUT);
  digitalWrite(irqpin, HIGH); //enable pullup resistor
  
  Serial.begin(9600);
  Wire.begin();

  mpr121_setup();

  // setup other things
  pinMode(button1pin, INPUT_PULLUP);  // pullup button pin

  for (int i=0; i<2; i=i+1) {
    pinMode(solPins[i], OUTPUT);
  }

Serial.begin(9600);
  
}

void loop(){

if (sessionStart == 0) {
  button1state = digitalRead(button1pin);

  if (button1state == 0) {
    sessionStartTime = millis();
    digitalWrite(trigPin, HIGH);
    delay(50);
    digitalWrite(trigPin, LOW);
    
    Serial.println("Session START");
    Serial.println(sessionStartTime);
    
    sessionStart = 1;
  
    delay(baselineDur);
  
    tone(spkrPin, tone1freq, 1000); // play cue tone for session start
  }

}

if (millis()- sessionStartTime >= sessionDur) {
  sessionStart = 0;
  tone(spkrPin, 2000, 1000);
  Serial.println("Session END");
  Serial.println(millis());
  
}

if (sessionStart == 1) {
    readTouchInputs();
}


  
} // END void loop()


void readTouchInputs(){
  if(!checkInterrupt()){
    
    //read the touch state from the MPR121
    Wire.requestFrom(0x5A,2); 
    
    byte LSB = Wire.read();
    byte MSB = Wire.read();
    
    uint16_t touched = ((MSB << 8) | LSB); //16bits that make up the touch states

    
    for (int i=0; i < 12; i++){  // Check what electrodes were pressed
      if(touched & (1<<i)){
      
        if(touchStates[i] == 0){
          //pin i was just touched

          lickTime = millis();

          // give reward based upon which port is licked
          digitalWrite(solPins[i], HIGH);
          delay(solDurs[i]);
          digitalWrite(solPins[i], LOW);
          
          Serial.print("lick port ");
          Serial.println(i);
          Serial.println(lickTime);

          // add in post-lick/reward delay here? (1s?)
        
        }else if(touchStates[i] == 1){
          //pin i is still being touched
        }  
      
        touchStates[i] = 1;      
      }else{
//        if(touchStates[i] == 1){
//          Serial.print("pin ");
//          Serial.print(i);
//          Serial.println(" is no longer being touched");
//          
//          //pin i is no longer being touched
//       }
        
        touchStates[i] = 0;
      }
    
    }
    
  }
}




void mpr121_setup(void){

  set_register(0x5A, ELE_CFG, 0x00); 
  
  // Section A - Controls filtering when data is > baseline.
  set_register(0x5A, MHD_R, 0x01);
  set_register(0x5A, NHD_R, 0x01);
  set_register(0x5A, NCL_R, 0x00);
  set_register(0x5A, FDL_R, 0x00);

  // Section B - Controls filtering when data is < baseline.
  set_register(0x5A, MHD_F, 0x01);
  set_register(0x5A, NHD_F, 0x01);
  set_register(0x5A, NCL_F, 0xFF);
  set_register(0x5A, FDL_F, 0x02);
  
  // Section C - Sets touch and release thresholds for each electrode
  set_register(0x5A, ELE0_T, TOU_THRESH);
  set_register(0x5A, ELE0_R, REL_THRESH);
 
  set_register(0x5A, ELE1_T, TOU_THRESH);
  set_register(0x5A, ELE1_R, REL_THRESH);
  
  set_register(0x5A, ELE2_T, TOU_THRESH);
  set_register(0x5A, ELE2_R, REL_THRESH);
  
  set_register(0x5A, ELE3_T, TOU_THRESH);
  set_register(0x5A, ELE3_R, REL_THRESH);
  
  set_register(0x5A, ELE4_T, TOU_THRESH);
  set_register(0x5A, ELE4_R, REL_THRESH);
  
  set_register(0x5A, ELE5_T, TOU_THRESH);
  set_register(0x5A, ELE5_R, REL_THRESH);
  
  set_register(0x5A, ELE6_T, TOU_THRESH);
  set_register(0x5A, ELE6_R, REL_THRESH);
  
  set_register(0x5A, ELE7_T, TOU_THRESH);
  set_register(0x5A, ELE7_R, REL_THRESH);
  
  set_register(0x5A, ELE8_T, TOU_THRESH);
  set_register(0x5A, ELE8_R, REL_THRESH);
  
  set_register(0x5A, ELE9_T, TOU_THRESH);
  set_register(0x5A, ELE9_R, REL_THRESH);
  
  set_register(0x5A, ELE10_T, TOU_THRESH);
  set_register(0x5A, ELE10_R, REL_THRESH);
  
  set_register(0x5A, ELE11_T, TOU_THRESH);
  set_register(0x5A, ELE11_R, REL_THRESH);
  
  // Section D
  // Set the Filter Configuration
  // Set ESI2
  set_register(0x5A, FIL_CFG, 0x04);
  
  // Section E
  // Electrode Configuration
  // Set ELE_CFG to 0x00 to return to standby mode
  set_register(0x5A, ELE_CFG, 0x0C);  // Enables all 12 Electrodes
  
  
  // Section F
  // Enable Auto Config and auto Reconfig
  /*set_register(0x5A, ATO_CFG0, 0x0B);
  set_register(0x5A, ATO_CFGU, 0xC9);  // USL = (Vdd-0.7)/vdd*256 = 0xC9 @3.3V   set_register(0x5A, ATO_CFGL, 0x82);  // LSL = 0.65*USL = 0x82 @3.3V
  set_register(0x5A, ATO_CFGT, 0xB5);*/  // Target = 0.9*USL = 0xB5 @3.3V
  
  set_register(0x5A, ELE_CFG, 0x0C);
  
}


boolean checkInterrupt(void){
  return digitalRead(irqpin);
}


void set_register(int address, unsigned char r, unsigned char v){
    Wire.beginTransmission(address);
    Wire.write(r);
    Wire.write(v);
    Wire.endTransmission();
}
