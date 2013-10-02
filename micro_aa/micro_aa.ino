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
  int redOut, irOut, i; 
  float ratio;
 
  // measure the two wavelengths with a short delay between them
  redOut = readingRED(); 
  delay(5)
  irOut = readingIR(); 
 
  ratio = calculateSPO2(redOut, irOut); 
 
  Serial.print(redOut); 
  Serial.print(","); 
  Serial.print(irOut); 
  Serial.print(","); 
  Serial.println(ratio); 
} 

float readingRED() { 
  //--Initializations-- 
  float raw, i; 
  int avgRED = 0;
 
  analogWrite(RED, 60);           // turn on RED LED 
  for (i=0;i<AVGBUFFER;i++){      // start running average 
    raw = analogRead(inputPin);   // read photodiode signal 
    avgRED += raw;                // contribute to average 
  } 
 
  digitalWrite(RED,HIGH);              //turn off RED LED 
  return (float) avgRED / AVGBUFFER;   // return total average 
} 

float readingIR() { 
  //--Initializations-- 
  int raw, i;
  int avgIR = 0;
 
  analogWrite(IR, 250);           // turn on IR LED 
  for (i=0;i<AVGBUFFER;i++){      // start running average 
    raw = analogRead(inputPin);   // read photodiode signal 
    avgIR += raw;                 // contribute to average 
  } 
 
  digitalWrite(IR,HIGH);              // turn off IR LED 
  return (float) avgIR / AVGBUFFER;   // return total average 
} 

float calculateSPO2(float rawIR, float rawRED) { 
  
  float rmsIR, rmsRED, ratio;
  rmsIR = rawIR/sqrt(2);
  rmsRED = rawRED/sqrt(2); 
  ratio = log10(rmsRED)/log10(rmsIR); 
  return ratio; 
  
}
