// ------------------------------------------------------------
//  Sensor EQUIPO 7
// ------------------------------------------------------------

// ------------------------------------------------------------
//  Libraries
// ------------------------------------------------------------

#include <ESP8266WiFi.h>
#include <Wire.h>      // Library to use the I2C BUS.
#include <Adafruit_ADS1X15.h> // ADS1115 Library.
#include <Sensors.h> // Library to use the sensors. 

// ------------------------------------------------------------
// Builders
// ------------------------------------------------------------

//---> Ads1115 builder
Adafruit_ADS1115 ads1115;
//---> Humidity Builder
Sensors sensors;

// ------------------------------------------------------------
// Variables
// ------------------------------------------------------------

//---> GPIO 5 is used to power the salinity probe
const int powerPin = 5;

// ------------------------------------------------------------
// Setup
// ------------------------------------------------------------

void setup() {
  //---> Sets the data rate in bits per second (baud) for serial data transmission
  Serial.begin(9600);

  //---> Set power_pin as output pin
  pinMode(powerPin, OUTPUT);

  //--->  Initialize the ads1115
  ads1115.begin();

  //---> Adjust the gain of ads1115
  ads1115.setGain(GAIN_ONE);

  //---> Send ads1115 to sensors.
  sensors.setAdaFruit(ads1115);
}

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------

void loop() {
  //---> Salinity Sensor.
  Serial.print("Salinidad: ");
  int salinityValue = sensors.getSalinityValue();
  /*
  if (salinityValue >= 100)
  {
    Serial.print("100");
  }
  else if (salinityValue <= 0)
  {
    Serial.print("0");
  }
  else
  {
    Serial.print(salinityValue);
  }
  */
  Serial.print(salinityValue);
  Serial.println("%");

  //---> Humidity Sensor.
  Serial.print("Humedad: ");
  int humidityValue = sensors.getHumidityValue();
  if (humidityValue >= 100)
  {
    Serial.print("100");
  }
  else if (humidityValue <= 0)
  {
    Serial.print("0");
  }
  else
  {
    Serial.print(humidityValue);
  }
  Serial.println("%");

  //---> Temperature Sensor.  
  Serial.print("Temperatura: ");
  Serial.print(sensors.getTemperatureValue());
  Serial.println("º");

  //---> Lightness Sensor.
   Serial.print("Cantidad de luz: ");
   int LighnessValue = sensors.getLightnessValue(); //Proximamente enviar estos valores a la nube en forma de porcentaje.
   switch(LighnessValue){
    case -1:
      Serial.println("Esta oscuro");
      break;
    case 0:
      Serial.println("Esta soleado");
      break;
    case 1:
      Serial.println("Esta nublado");
      break;
    default:
      Serial.println("Se ha producido un error inesperado, contacte con el administrador del dispositivo y muestrele el codigo de error \"DARK\"");
   }
}
