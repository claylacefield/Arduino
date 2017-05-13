/*
 * Lick training for Jack's 2-port task
 * Animals can lick either port for reward
 * 
 * Questions:
 * - alternate lick locations?
 * - tone with reward?
 * - standardize serial output
 */


// SET VARIABLES
int stimFreq1 = 5000;
int rewDur = 100;

// pins
int lickPin1 = 8;  // digital pins
int lickPin2 = 9;
int spkrPin1 = 10;
int spkrPin2 = 11;
int solPin1 = 4;
int solPin2 = 5;
int buttonPin = 6;


int lickVal1 = 0;
int lickVal2 = 0;

long rewTime = 0;

void setup() {
  pinMode(lickPin1, INPUT);
  pinMode(lickPin2, INPUT);
  pinMode(solPin1, OUTPUT);
  pinMode(solPin2, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

//  portTone1.begin(spkrPin1);
//  portTone2.begin(spkrPin2);

  Serial.begin(9600);

}

void loop() {
  lickVal1 = digitalRead(lickPin1);
  lickVal2 = digitalRead(lickPin2);

  if (lickVal1==1) {
    rewTime = millis();
    digitalWrite(solPin1, HIGH);
    tone(spkrPin1, stimFreq1, 1000);
    delay(rewDur);
    digitalWrite(solPin1, LOW);
    Serial.println("REWARD, port=1, millis=" + String(rewTime));
  }
  else if (lickVal2==1) {
    rewTime = millis();
    digitalWrite(solPin2, HIGH);
    tone(spkrPin1, stimFreq1, 1000);
    delay(rewDur);
    digitalWrite(solPin2, LOW);
    Serial.println("REWARD, port=2, millis=" + String(rewTime));
  }

}
