#include <math.h> 
//--GLOBAL VARIABLES-- 
int RED = 3;
int IR = 5;
int AVGBUFFER = 100; 
int inputPin = 0;

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
  float irOut, redOut, bgOut, ratio;

  bgOut = readingBG();
  delay(2)
  // measure the RMS transmission for the two wavelengths 
  // with a short delay between them
  redOut = readingRED(); 
  delay(3);
  irOut = readingIR(); 

  // subtract off average background levels
  redOut -= bgOut;
  irOut -= bgOut;
 
  ratio = calculateSPO2(irOut, redOut); 
 
  Serial.print(redOut);
  Serial.print("\t"); 
  Serial.print(irOut); 
  Serial.print("\t"); 
  Serial.println(ratio); 
} 


float readingBG() { 
  //--Initializations-- 
  int raw, i; 
  float avg = 0;
 
  digitalWrite(IR,HIGH);   // turn off IR LED 
  digitalWrite(RED,HIGH);  // turn off RED LED 

  for (i=0;i<AVGBUFFER;i++){         // start running average 
    raw = analogRead(inputPin);      // read photodiode signal
    avg +=  (float) raw / AVGBUFFER; 
  } 

  return avg;             // return total average 
}


float readingRED() { 
  //--Initializations-- 
  int raw, i; 
  float avg = 0;
 
  analogWrite(RED, 60);               // turn on RED LED 
  for (i=0;i<20;i++){                 // don't actually collect until the signal is in a steady state
    raw = analogRead(inputPin);       // read photodiode signal 
  } 
  for (i=0;i<AVGBUFFER-20;i++){       // start running average 
    raw = analogRead(inputPin);       // read photodiode signal 
    avg += (float) raw / AVGBUFFER;   // contribute to average
  } 
 
  digitalWrite(RED,HIGH);   // turn off RED LED 
  return avg;               // return total average 
} 

float readingIR() { 
  //--Initializations-- 
  int raw, i; 
  float avg = 0;
 
  analogWrite(IR, 60);               // turn on IR LED 
  for (i=0;i<20;i++){                 // don't actually collect until the signal is in a steady state
    raw = analogRead(inputPin);       // read photodiode signal 
  } 
  for (i=0;i<AVGBUFFER-20;i++){       // start running average 
    raw = analogRead(inputPin);       // read photodiode signal 
    avg += (float) raw / AVGBUFFER;   // contribute to average
  } 
  
  digitalWrite(IR,HIGH);  // turn off IR LED 
  return avg;             // return total average 
}


float calculateSPO2(float sigIR, float sigRED) { 
  
  float ratio, rmsRED, rmsIR;
  rmsRED = .5 * sigRED / sqrt(2);
  rmsIR = .5 * sigIR / sqrt(2);

  ratio = log10(rmsRED) / log10(rmsIR); 
  
  return ratio; 
}
