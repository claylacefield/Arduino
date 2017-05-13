/* mitopto_2led
 Clay 2017
 
 (from Blink without Delay) 
 Turns on and off light emitting diodes (LED) connected to arduino digital
 pins, without using the delay() function (state machine structure). 
 This allows two LEDs to be pulsed at different rates.

 This is for use with BuckPuck LED drivers, with Arduino digital pins going into
 buckpuck CTL pin. Arduino GND should be connected to LED- pin.
 Note that you should put a 220 Ohm resistor between buckpuck CTL and +5V
 to keep the pin pulled up, to prevent LED from lighting when Arduino is reset.

 based upon
 http://www.arduino.cc/en/Tutorial/BlinkWithoutDelay
 */

// EXPERIMENTAL PARAMETERS
const int stimDur1 = 5000;
const int stimDur2 = 3000;
const int stimFreq1 = 40;
const int stimFreq2 = 10;

// constants won't change. Used here to set a pin number :
const int ledPin1 =  8;      // the number of the LED pin
const int ledPin2 =  9;

// Variables will change :
int ledState1 = HIGH;             // ledState used to set the LED (NOTE: HIGH is OFF for buckPuck LED driver)
int ledState2 = HIGH;

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis1 = 0;        // will store last time LED was updated
unsigned long previousMillis2 = 0;

// constants won't change :
const long interval1 = 1000/stimFreq1/2;           // interval at which to blink (milliseconds)
const long interval2 = 1000/stimFreq2/2;

// other vars
const int buttonPin = 12;

unsigned long startTime1 = 0;
unsigned long startTime2 = 0;

int pulseOn1 = 0;
int pulseOn2 = 0;

unsigned long currentMillis = 0;

void setup() {
  // set the digital pin as output:
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);

  digitalWrite(ledPin1, HIGH);  // go ahead and set pins HIGH to keep LED OFF
  digitalWrite(ledPin2, HIGH);
  
  pinMode(buttonPin, INPUT_PULLUP);  // use pullup for button input

  Serial.begin(9600);
}

void loop() {
  // here is where you'd put code that needs to be running all the time.

  if (pulseOn1 == 0 && digitalRead(buttonPin)==LOW) {
    pulseOn1 = 1;
    startTime1 = millis();
    pulseOn2 = 1;
    startTime2 = millis();

    Serial.println("START pulse train");
    Serial.println(startTime1);
  }

  // check to see if it's time to blink the LED; that is, if the
  // difference between the current time and last time you blinked
  // the LED is bigger than the interval at which you want to
  // blink the LED.
  currentMillis = millis();

  if (currentMillis-startTime1<stimDur1 && pulseOn1 == 1) {
    checkLED1();
  }
  else {
    digitalWrite(ledPin1, HIGH);
    pulseOn1 = 0;
    }
  
  if (currentMillis-startTime2<stimDur2 && pulseOn2 == 1) {
    checkLED2();
  }
  else {
    digitalWrite(ledPin2, HIGH);
    pulseOn2 = 0;
    }
  
}


void checkLED1() {
  if (currentMillis - previousMillis1 >= interval1) {
    // save the last time you blinked the LED
    previousMillis1 = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState1 == LOW) {
      ledState1 = HIGH;
    } else {
      ledState1 = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin1, ledState1);
    
  }
}

void checkLED2() {
  if (currentMillis - previousMillis2 >= interval2) {
    // save the last time you blinked the LED
    previousMillis2 = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState2 == LOW) {
      ledState2 = HIGH;
    } else {
      ledState2 = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin2, ledState2);
    
  }
}

