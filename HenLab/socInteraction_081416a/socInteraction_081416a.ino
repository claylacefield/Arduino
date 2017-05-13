// Social Interaction task (headfixed)
// Aug. 14, 2016



//pins

int servoPin1 = 12;
int ledPin1 = 13;
int ledPin2 = 11;

int trigOutPin = 8;
//int trigInPin = 9;

int spkrPin = 10;

// pushbutton
int button1pin = 9;
int button1state = 1; 

//
int freq1 = 2000;
int tone1freq = 1000;

int sessionStart = 0;
int sessionStartTime = 0;
int baselineDur = 60000;
long sessionDur = 20*60000;

int interactDur = 20000;
int iti = 5000;

// libraries

#include <Servo.h>
Servo door1servo;
int servo1pos1 = 0;
int servo1pos2 = 90;

void setup() {

  door1servo.attach(servoPin1);

  pinMode(button1pin, INPUT_PULLUP);
  //pinMode(trigInPin, INPUT_PULLUP);
  pinMode(trigOutPin, OUTPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);

  Serial.begin(9600);

}

void loop() {
  if (sessionStart == 0) {
    button1state = digitalRead(button1pin);
  
    if (button1state == 0) {
      sessionStartTime = millis();
      digitalWrite(trigOutPin, HIGH);  // trigger 2p scan
      delay(50);
      digitalWrite(trigOutPin, LOW);
      
      Serial.println("Session START");
      Serial.println(sessionStartTime);
      
      sessionStart = 1;
    
      delay(baselineDur);
    
      tone(spkrPin, tone1freq, 1000); // play cue tone for session start
    }
  
  }

  if (millis()- sessionStartTime >= sessionDur && sessionStart == 1) {
    sessionStart = 0;
    tone(spkrPin, 2000, 1000);
    Serial.println("Session END");
    Serial.println(millis());
    
  }
  
  if (sessionStart == 1) {

    tone(spkrPin, tone1freq, 1000);
    digitalWrite(ledPin1, HIGH); // IR led marking trial start for video synch
    Serial.println("Interaction Trial START");
    Serial.println(millis());
    delay(5000);
    digitalWrite(ledPin1, LOW);
    
    door1servo.write(servo1pos2);  // open door
    delay(interactDur);  // leave open for a while

    // NOTE: here put in check for no door IR beam break
    
    door1servo.write(servo1pos1);  // close door
    delay(iti);
  }
  
}
