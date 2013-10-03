#include <math.h> 
//--GLOBAL VARIABLES-- 
int RED = 3;
int IR = 5;
int lRED = 660;
int lIR = 940;
int AVGBUFFER = 100; 
int inputPin = 0;

// declare this global array
int vals[AVGBUFFER];

void setup() { 
  pinMode(RED, OUTPUT);       // set RED pin to output 
  pinMode(IR, OUTPUT);        // set IR pin to output
  pinMode(inputPin, INPUT);   // set input pin
  digitalWrite(RED, HIGH);    // turn off RED LED 
  digitalWrite(IR, HIGH);     // turn off IR LED
  Serial.begin(9600);         // launch communications
} 

void loop() { 
  int i; 
  float irOut, redOut, ratio;

  // measure the RMS transmission for the two wavelengths 
  // at separate times
  redOut = readingRED(); 
  // delay(1);
  irOut = readingIR(); 

  ratio = calculateSPO2(irOut, redOut); 

  // At this point need to use a lookup table to convert the ratio to a SPO2 level
 
  Serial.print(redOut);
  Serial.print("\t"); 
  Serial.print(irOut); 
  Serial.print("\t"); 
  Serial.println(ratio); 
} 

float readingRED() { 
  //--Initializations-- 
  int raw, i; 
  float avg = 0;
  float rms = 0;
 
  analogWrite(RED, 60);               // turn on RED LED 
  for (i=0;i<20;i++){                 // don't actually collect until the signal is in a steady state
    raw = analogRead(inputPin);       // read photodiode signal 
  } 

  for (i=0;i<AVGBUFFER;i++){          // start running average 
    raw = analogRead(inputPin);       // read photodiode signal 
    avg += (float) raw / AVGBUFFER;   // contribute to average
    vals[i] = raw;                    // log the value read
  }

  digitalWrite(RED,HIGH);             // turn off RED LED 

  // remove DC offset and calculate RMS of signal
  for (i=0; i<AVGBUFFER; i++){
    rms += ((float) vals[i] - avg)^2 / AVGBUFFER;
  }

  return sqrt(rms);                   // return RMS of the AC signal
} 

float readingIR() { 
  //--Initializations-- 
  int raw, i; 
  float avg = 0;
  float rms = 0;
 
  analogWrite(IR, 60);                // turn on IR LED 
  for (i=0;i<20;i++){                 // don't actually collect until the signal is in a steady state
    raw = analogRead(inputPin);       // read photodiode signal 
  } 

  for (i=0;i<AVGBUFFER;i++){          // start running average 
    raw = analogRead(inputPin);       // read photodiode signal 
    avg += (float) raw / AVGBUFFER;   // contribute to average
    vals[i] = raw;                    // log the value read
  } 

  digitalWrite(IR,HIGH);              // turn off IR LED 

  // remove DC offset and calculate RMS of signal
  for (i=0; i<AVGBUFFER; i++){
    rms += ((float) vals[i] - avg)^2 / AVGBUFFER;
  }

  return sqrt(rms);                   // return RMS of the AC signal
} 


float calculateSPO2(float rmsIR, float rmsRED) { 
  
  float ratio;

  ratio = log10(rmsRED) * lRED / (log10(rmsIR) * lIR); 
  
  return ratio; 
}
