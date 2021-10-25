//------------------------------------------------------------
//Salinity and Humidity Sensor EQUIPO 7
// ------------------------------------------------------
//
// Program to measure Salinity and humidity of the soil
// from two electrodes and a humidity sensor
// connected to the channel of the ADS1115
// and to the GPIO 5
// ------------------------------------------------------

#include <Wire.h>      // Library to use the I2C BUS
#include <Adafruit_ADS1X15.h> // ADS1115 Library 

//---> Ads1115 builder
Adafruit_ADS1115 ads1115;

//--->  Salinity sensor calibration variables
const int power_pin = 5; // GPIO 5 is used to power the salinity probe
const int ch_salinity = 0; //Channel where the salinity sensor is to be connected
const int without_salt = 11141; //Measurement without salt in the environment
const int with_salt = 19481; // Measured with salt in the environment

//--->  Humidity sensor calibration variables
const int AirValue = 175; // Dry value
const int WaterValue = 275; // Value in water
const int ch_humidity = 1; //Channel where the humidity sensor is to be connected

//---> Measure variables.
const int n_measure = 20;

void setup() {

  Serial.begin(115200);
  Serial.println("Inicializando...");

  // Set power_pin as output pin
  pinMode(power_pin, OUTPUT);

  //--->  Initialize the ads1115
  ads1115.begin();

  //---> Adjust the gain of ads1115
  Serial.println("Ajustando la ganancia...");
  ads1115.setGain(GAIN_ONE);

  Serial.println("Bienvenido al sensor, tomando medidas...");

}

void loop() {
  //Message displayed to the user.
  //Serial.println("Se tomaron las siguientes medidas: ");

  //Serial.println("Humedad: ");
  int x = calc_humidity(take_measure(ch_humidity));
  //Serial.println(x);
  
  if (x >= 100)
  {
    Serial.print("100");
  }
  else if (x <= 0)
  {
    Serial.print("0");
  }
  else
  {
    Serial.print(x);
  }
  Serial.println("%");

  Serial.print("Salinidad: ");
  
  int y = calc_salinity(take_measure(ch_salinity));
  //Serial.println(y);
  
  if (y >= 100)
  {
    Serial.print("100");
  }
  else if (y <= 0)
  {
    Serial.print("0");
  }
  else
  {
    Serial.print(y);
  }
  Serial.println("%");
  
  delay(1000);
  Serial.println("Tomando siguientes medidas...");
}

int calc_humidity(float humidityRAW) {
  //Serial.println("Humeda raw");
  int humidity = 100 * AirValue / (AirValue - WaterValue) - humidityRAW * 100 / (AirValue - WaterValue);
  return humidity;
}

int calc_salinity(float salinityRAW) {
  int salinity_percentage = map(salinityRAW, without_salt, with_salt, 0, 100);
  return salinity_percentage;
}

float take_measure(int num_channel) {

  int i;
  float reading, sum; // We use float to have as much precision as possible

  reading = 0.0;
  sum = 0.0;

  switch (num_channel) {
    case 0:
      for ( i = 1; i <= n_measure; i++ ) {
        digitalWrite( power_pin, HIGH ); // We give power to the sensor
        delay(100); // We wait for the sensor to initialize
        sum += ads1115.readADC_SingleEnded(num_channel); // We read the values
        digitalWrite( power_pin, LOW ); // We turn off the sensor
        delay(10); // We wait for the results
      }
      break;
    case 1:
      for ( i = 1; i <= n_measure; i++ ) {
        sum += ads1115.readADC_SingleEnded(num_channel); // We read the values
      }
      break;
    default:
      Serial.println("Se ha producido un error inesperado, contacte con el administrador del dispositivo y muestrele el codigo de error \"ZEBRA\"");
      delay(1000);
  }

  reading = sum / float(n_measure);
  return reading;
}
