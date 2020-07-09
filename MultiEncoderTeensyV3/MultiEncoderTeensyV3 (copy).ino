/*
   MultiEncoderTeensy.ino

   Teensy4 polls six precision encoders (default every 10 ms as set by frameInterval)
   If any reading has changed, outputs header followed by six readings and timestamp as csv values

   Default values are actual distances in mm, however mode can be changed to report difference
   between setpoint and the actual distance.
     Serial  command: M=D  sets mode to report absolute distance (default)
     Serial command: M=E sets mode to report error values


   Readings are mm*10 if decimalPlaces equals 1 (divide by 10.0 to get mm as fp)
   or nearest mm if decimalPlaces equals 0

   timestamp is millis counter
   input checks:  'r' reset encoder counts to zero, 'R' also resets millis counter
                  'M' sets mode ('D' for absolute distance, 'E' for error value)
                  'S' sets setpoint:
                        Format for setpoint is:  S,s1,s2,s3,s4,s5,s6
                         where s1 through s6 are setpoint values in mm (todo, support floats?)

   Data format for mm:     D0,e1,e2,e3,e4,e5,e6,timestamp\n
   Data format for mm*10:  D1,e1,e2,e3,e4,e5,e6,timestamp\n
      where e1 through e6 are integer encoder readings
*/


#include <Encoder.h>

#define DISTANCE_MODE 'D'
#define ERROR_MODE  'E'
char outMode = DISTANCE_MODE;

//#define SUPPRESS_UNCHANGED 

const char HEADER = 'D';   // output messages begin with this char
const int decimalPlaces = 0; //(0-2) set to zero to round to nearest mm
const int nbrEncoders = 6;

//const int ROUND_FACTOR[3] = {100, 10, 1}; // add this factor to raw reading to round
//const int PRECISION_FACTOR[3] = {200, 20, 2}; // divide raw reading by this value to get mm

elapsedMillis tickCounter;  // this is a resetable millis
unsigned int frameInterval = 10; //  ms between frames

#define PCB // undefine this for prototype teensy version
#ifdef PCB
Encoder encoder0(3, 2);
Encoder encoder1(4, 5);
Encoder encoder2(6, 7);
Encoder encoder3(23, 22);
Encoder encoder4(21, 20);
Encoder encoder5(18, 19);
// pin 8 connected to switch but not used in this version
#else
Encoder encoder0(4, 3);
Encoder encoder1(6, 5);
Encoder encoder2(8, 7);
Encoder encoder3(22, 21);
Encoder encoder4(20, 19);
Encoder encoder5(18, 17);
// pin 2 connected to switch but not used in this version
#endif


Encoder *encoders[] = {&encoder0, &encoder1, &encoder2, &encoder3, &encoder4, &encoder5};

const int LED_PIN = 13;

float values[nbrEncoders] = {0.0}; // array to hold current encoder values
int     currentFrame = 0;   // frames since previous timestamp message
uint32_t prevousMessageTime;
uint32_t blinkTime;

int setpoints[nbrEncoders] = {0};

void printInfo()
{
#ifdef PCB
  Serial.print("#Production PCB, ");
#else
  Serial.print("#Prototype board, ");
#endif
  if (decimalPlaces > 0) {
    Serial.print("output is mm*");
    Serial.print(int(pow(10, decimalPlaces)));
  }
  else {
    Serial.print("output rounded to nearest mm");
  }
  Serial.print(", Frame interval is "); Serial.print(frameInterval);
  Serial.print(", Mode="); Serial.print(outMode);
  Serial.print(", Setpoints: ");
  for (int i = 0; i < nbrEncoders; i++) {
    Serial.print(setpoints[i]);
    if ( i < nbrEncoders - 1)
      Serial.print(",");
    else
      Serial.println();
  }
}

void setup() {
  Serial.begin(57600);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  printInfo();
}

void loop() {
  if ( millis() - prevousMessageTime >= frameInterval) {
    reportValues();  // write values to serial
    prevousMessageTime = millis();
  }
  if (millis() - blinkTime > 100) {
    digitalWrite(LED_PIN, HIGH);
  }
  // if 'r' or 'R' is received on serial port, reset all encoders
  if (Serial.available())
  {
    int c = Serial.read();
    if ( c == 'S') {
      readSetpoints();
    }
    else if ( c == 'M' && Serial.read() == '=' ) {
      c = Serial.read();
      if ( c == DISTANCE_MODE)
        outMode = DISTANCE_MODE;
      else if ( c == ERROR_MODE)
        outMode = ERROR_MODE;
    }
    else if ( c == 'R' || c == 'r') {
      for (int i = 0; i < nbrEncoders; i++)
        resetEncoder(i);
    }
    if ( c == 'R') {
      tickCounter = 0;
    }
    else if (c == 'I') {
      int i = Serial.parseInt();
      if (i > 0) {
        frameInterval = i;
      }
    }
    else if (c == '?') {
      printInfo();
    }
  }
}

void readSetpoints()
{
  for (int i = 0; i < nbrEncoders; i++) {
    setpoints[i] = Serial.parseInt();
  }
}

/*
int32_t rawReadingToMM(int32_t rawReading, int dp)
{
  int32_t mm = (rawReading + ROUND_FACTOR[dp]) / PRECISION_FACTOR[dp];
  // return value is mm*10 if decimalPlaces equals 1, mm*100 if decimalPlaces =2
  return mm;
}
*/

void reportValues()
{
  bool changed = false;
  for (int i = 0; i < nbrEncoders; i++) {
    // Read the encoders
    //int32_t val = rawReadingToMM( encoders[i]->read(), decimalPlaces);
    float val =  encoders[i]->read() / 200.0;
    if( decimalPlaces == 0)
       val = round(val);
    if (val != values[i]) {
      changed = true;
      values[i] = val;
    }
  }
#ifndef SUPPRESS_UNCHANGED   
  changed = true; // force output
#endif  
  if (changed) {
    // send csv values as millimeters * 10
    digitalWrite(LED_PIN, LOW);
    blinkTime = millis();
    Serial.print(HEADER);
    Serial.print(decimalPlaces);
    Serial.print(",");
    for (int i = 0; i < nbrEncoders; i++) {
      if (outMode == DISTANCE_MODE) {
        Serial.print(values[i], decimalPlaces);
      }
      else if (outMode == ERROR_MODE) {
        Serial.print(values[i] - setpoints[i], decimalPlaces);
      }
      Serial.print(",");
    }
    Serial.println(tickCounter);
  }
}

void resetEncoder(int index)
{
  encoders[index]->write(0);
}
