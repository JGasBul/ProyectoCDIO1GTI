//------------------------------------------------------------
//Light Sensor EQUIPO 7
// ------------------------------------------------------
// Program to measure Lightness with a Resistance=100k ohms
// ------------------------------------------------------
#include <Wire.h>      // Library to use the I2C BUS
#include <Adafruit_ADS1X15.h> // ADS1115 Library 

//---> Ads1115 builder
Adafruit_ADS1115 ads1115;

//---> Measure variables.
const int n_measure = 20;
const int darkness = 25;
const int max_lightness = 29500;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Inicializando...");

  //--->  Initialize the ads1115
  ads1115.begin();

  //---> Adjust the gain of ads1115
  Serial.println("Ajustando la ganancia...");
  ads1115.setGain(GAIN_ONE);
}

void loop() {
  float x = take_mesure();
  //float x = 3.72;
  Serial.println(x);
  Serial.println();
  Serial.println();


  if (x <= darkness) {
    Serial.println("esta oscuro");
  }
  else if (x >= max_lightness) {
    Serial.println("esta sol");
  }
  else {
    Serial.println("esta nublado");
  }

  delay(1000);
}

float take_mesure() {
  int i;
  float reading, sum; // We use float to have as much precision as possible

  reading = 0.0;
  sum = 0.0;

  for ( i = 1; i <= n_measure; i++ ) {
    sum += ads1115.readADC_SingleEnded(0); // We read the values
  }

  reading = sum / float(n_measure);

  return reading;
}
