/*#include <Arduino.h>
#include <Ewma.h>*/

// CONFIG
const int FLEX_PIN = A0;                    //pin A0 to read analog input
const float VCC = 4.98;                     // 4.98Measured voltage at Ardunio 5V line
const float R_DIV = 10000.0;                // Resistor used to voltage divider
const float STRAIGHT_RESISTANCE = 10000.0;  // Resistance when straight/flat
const float BEND_RESISTANCE = 110000.0;     // Resistance at 90 deg

//Ewma adcFilter(0.1); // Noise filter settings, equal to more smoothing


// SETUP
void setup(){
  Serial.begin(9600);                       // Begin serial communication
  pinMode(FLEX_PIN, INPUT);
}


// POLY. CLASSIFIER
double analogToDegree(int x){
  /* Analog value simulator=255-59 -- physical=360-290
  return map(x, 360, 290, 0.0, 180.0); */

  /* Every 15 deg.
  return (405 -5.34*x + 0.035*pow(x,2) -1.18e-04*pow(x,3) + 1.5e-07*pow(x,4)); */

  // Every 20 deg. (working on simulator)
  //return (247 - 1.51*x + 2.16e-03*pow(x,2));

  // Every 20 deg. (physical pad)
  /* 3 DEG. POLY -> -21992 + 248x + -0.923x^2 + 1.13E-03x^3
  	 2 DEG. POLY -> 597 + -2.47x + 1.74E-03x^2              */
  return (597 + -2.47*x + 1.74e-03*pow(x,2));
  //return (-21992 + 248*x + -0.923*pow(x,2) + 1.13e-03*pow(x,3));

}


void loop(){
  int flexADC = analogRead(FLEX_PIN);        // READ FLEX SENSOR

  // --- NOISE FILTER - BEGIN --------------
  float filteredFlexADC = flexADC; //adcFilter.filter(flexADC);

  // --- NOISE FILTER - END ----------------

  // --- DATA ANALYSIS - BEGIN -------------

  float flexV = flexADC * VCC / 1023.0;      // CALCULATE - Sensor's V and R
  float flexR = R_DIV * (VCC / flexV - 1.0);
  float angle = analogToDegree(filteredFlexADC);

  Serial.println("V: " + String(flexV) + "\t\tAnalog: " + String(flexADC) + "\tfilterd: " + String(filteredFlexADC) + "\t\tDegree: " + angle + char(176) + "\t"); // ° degree symbol
  // Serial.print(String(angle) + char(176) + "\t"); // ° degree symbol

  if (180 >= angle && angle > 150){
    Serial.println("CAUTION\tKnee joint under extreme force. High chance of injury!");
  } else if (150 >= angle && angle > 120){
    Serial.println("HIGH\tKnee joint under considerable force. Injury possibility minimal");
  } else if (120 >= angle && angle > 90){
    Serial.println("MEDIUM\tModerate force on knee joint");
  } else {
    Serial.println("LOW\tAngle is in safe zone");
  }

  // --- DATA ANALYSIS - END ---------------


  // Sleep
  delay(100);
}