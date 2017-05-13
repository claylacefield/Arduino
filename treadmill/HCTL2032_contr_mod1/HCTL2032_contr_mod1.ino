#include <ArduinoJson.h>

//D13: read select 1 (S1)
unsigned char readSel1H = 0B00100000;
unsigned char readSel1L = 0B11011111;

//D12 read reset (RR)
unsigned char readResetH = 0B00010000;
unsigned char readResetL = 0B11101111;

//D11 reset EC2 (R2)
unsigned char resetEC2H = 0B00001000;
unsigned char resetEC2L = 0B11110111;

//D10 reset EC1 (R1)
unsigned char resetEC1H = 0B00000100;
unsigned char resetEC1L = 0B11111011;

//D9 read select 2 (S2)
unsigned char readSel2H = 0B00000010;
unsigned char readSel2L = 0B11111101;

//D8 EC read select (CS)
unsigned char selECReadH = 0B00000001;
unsigned char selECReadL = 0B11111110;

long prevPos;
long curPos;
long diffPos;
unsigned char *pos8BitArr = (unsigned char *)(&curPos); // not sure how this works (pointers?)

unsigned long tMillis;

StaticJsonBuffer<200> jsonBuffer;
JsonObject& message = jsonBuffer.createObject();
JsonObject& value = message.createNestedObject("position");;
void setup()
{
  Serial.begin(19200);
  value["dy"] = 0;
  message["millis"] = 0;

  //A0-A5 as digital input pins, for reading the data from
  //HCTL2032 bit 0-5 (D0-D5)
  // DDRC - The Port C (Analog 0-5) Data Direction Register - read/write
  DDRC = 0;  // set A0-5 as input
  pos8BitArr[3] = B00111111 & PINC;  // fill 8bit array 3 with A0-5 
  
  //D6-D7 as digital input pins for reading bit 6-7 (D6, D7)
  // DDRD is the direction register for Port D (Arduino digital pins 0-7). 
  // The bits in this register control whether the pins in PORTD are configured as inputs or outputs
  // (Pins 6 & 7 are only accessible on the Arduino Mini)?
  DDRD = DDRD & B00111111;  // sets (masks) direction registers for 0-7 
  // INPUT for D6,7 (what is initial state of DDRD?), output for rest?

  //control pins
  // PORTB maps to Arduino digital pins 8 to 13 The two high bits (6 & 7) map to the crystal pins and are not usable
  // OR makes sure these bits turned on, thus D8-13 are output, last 2 bits unchanged
  DDRB = DDRB | B00111111;

  //reset EC1
  PORTB = PORTB & resetEC1L;  // mask out 3rd bit (D10 LOW) (what is prev state of PORTB?)
  delayMicroseconds(10);
  PORTB = PORTB | resetEC1H;  // then set this pin HIGH

  //reset EC2
  PORTB = PORTB & resetEC2L;  // 4th bit (D11 low)
  delayMicroseconds(10);
  PORTB = PORTB | resetEC2H;  // resetEC2H = 0B00001000; high

  //disable read latch
  PORTB = PORTB | readResetH;  // 5th bit (D12), readResetH = 0B00010000; set D12 high

  //select read channel, low for EC1, high for EC2
  PORTB = PORTB & selECReadL;  // selECReadL = 0B11111110; set D8 low
  //  PORTB=PORTB | selECReadH;  //
}

void loop()
{
  //read MSB
  // (PORTB is control output)
  PORTB = PORTB & readResetL;  // readResetL = 0B11101111; set D12 low
  PORTB = PORTB & readSel1L;  // readSel1L = 0B11011111; set D13 low
  PORTB = PORTB | readSel2H;  // readSel2H = 0B00000010; set D9 high
  tMillis=millis();
  delayMicroseconds(2);
  pos8BitArr[3] = B11000000 & PIND;  // read D6,7
  pos8BitArr[3] = pos8BitArr[3] | B00111111 & PINC;  // read A0-5

  //next byte
  PORTB = PORTB | readSel1H; // readSel1H = 0B00100000; set D13 high
  delayMicroseconds(2);
  pos8BitArr[2] = B11000000 & PIND;
  pos8BitArr[2] = pos8BitArr[2] | B00111111 & PINC;
  
  //next byte
  PORTB = PORTB & readSel1L; // readSel1L = 0B11011111; set D13 low
  PORTB = PORTB & readSel2L;  // readSel2L = 0B11111101; set D8 low
  delayMicroseconds(2);
  pos8BitArr[1] = B11000000 & PIND;  // read
  pos8BitArr[1] = pos8BitArr[1] | B00111111 & PINC;

  //read LSB
  PORTB = PORTB | readSel1H; // readSel1H = 0B00100000; set D13 high
  delayMicroseconds(2);
  pos8BitArr[0] = B11000000 & PIND;
  pos8BitArr[0] = pos8BitArr[0] | B00111111 & PINC;
  
  PORTB = PORTB | readResetH;  // readResetH = 0B00010000; set D12 high (to end read seq)
  delayMicroseconds(2);

  diffPos=curPos-prevPos;
  if(diffPos==0)
  {
    return;
  }

  value["dy"]=diffPos;
  message["millis"]=tMillis;
  message.printTo(Serial);
  Serial.println();

  prevPos = curPos;

  delay(50);
}
