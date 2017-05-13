// Social Interaction task (headfixed)
// Aug. 14, 2016

// timing variables
int baselineDur = 20000;
long sessionDur = 20*60000;
int interactDur = 20000;
int iti = 10000;

//pins
int servoPin1 = 12;
int ledPin1 = 13;
int ledPin2 = 11;

int trigOutPin = 8;
//int trigInPin = 9;

int spkrPin = 10;
int button1pin = 9;
int button1state = 1; 

// tones
int tone1freq = 2000;
int tone2freq = 1000;

int sessionStart = 0;
int sessionStartTime = 0;

// door servo

#include <Servo.h>
Servo door1servo;
int servo1pos1 = 90;  // closed position
int servo1pos2 = 0;   // open

void setup() {

  door1servo.attach(servoPin1);

  pinMode(button1pin, INPUT_PULLUP);
  //pinMode(trigInPin, INPUT_PULLUP);
  pinMode(trigOutPin, OUTPUT);    // 2p trigger
  pinMode(ledPin1, OUTPUT);     // red LED for session starts
  pinMode(ledPin2, OUTPUT);     // IR LED for trial start (for video synching)

  Serial.begin(9600);

}

void loop() {
  if (sessionStart == 0) {
    button1state = digitalRead(button1pin);
  
    if (button1state == 0) {   // if button pressed, start session
      sessionStartTime = millis();
      digitalWrite(trigOutPin, HIGH);  // trigger 2p scan
      digitalWrite(ledPin1, HIGH);    // flash session start LED
      delay(50);
      digitalWrite(trigOutPin, LOW);
      delay(450);
      digitalWrite(ledPin1, LOW);
      
      Serial.println("session START");
      Serial.println(sessionStartTime);
      
      sessionStart = 1;
    
      delay(baselineDur);   // baseline period before trials start
    
      //tone(spkrPin, tone1freq, 1000); // play cue tone for session start
    }
  
  }

  // end session if time is up
  if (millis()- sessionStartTime >= sessionDur && sessionStart == 1) {
    sessionStart = 0;
    tone(spkrPin, tone2freq, 1000);
    Serial.println("session END");
    Serial.println(millis());
    
  }
  
  if (sessionStart == 1) {

    tone(spkrPin, tone1freq, 1000);
    digitalWrite(ledPin2, HIGH); // IR led marking trial start for video synch
    Serial.println("interaction trial START");
    Serial.println(millis());
    delay(5000);
    digitalWrite(ledPin2, LOW);
    
    door1servo.write(servo1pos2);  // open door
    delay(interactDur);  // leave open for a while
          // NOTE: here put in check for no door IR beam break before closing
    door1servo.write(servo1pos1);  // close door
    delay(iti);   // intertrial interval
  }
  
}
