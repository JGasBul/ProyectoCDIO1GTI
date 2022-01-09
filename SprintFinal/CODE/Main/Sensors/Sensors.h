#ifndef Sensors_h
#define Sensors_h

//#include "Arduino.h"
#include <Wire.h>             // Library to use the I2C BUS
#include <Adafruit_ADS1X15.h> // ADS1115 Library

class Sensors
{
private:
    int _num_chanel;
    Adafruit_ADS1115 _ads1115;
    static const int n_measure;
    static const int power_pin;
    float takeMeasure(int num_channel);

public:
    Sensors(int num_channel = -1, Adafruit_ADS1115 ads1115);
    void setNum_channel(int num_channel);
    int getNum_channel();
    int getMeasure();
};

Sensors::Sensors(int num_channel)
{
    const int n_measure = 20;
    const int power_pin = 5;
    this->_num_chanel = num_channel;
}
void Sensors::setNum_channel(int num_channel)
{
    this->_num_chanel = num_channel;
}
int Sensors::getNum_channel()
{
    return this->_num_chanel;
}
float Sensors::takeMeasure(int num_channel)
{
    int i;
    float reading, sum; // We use float to have as much precision as possible
    reading = 0.0;
    sum = 0.0;
    switch (num_channel)
    {

    case 0:
        for (i = 1; i <= n_measure; i++)
        {
            digitalWrite(power_pin, HIGH);                   // We give power to the sensor
            delay(100);                                      // We wait for the sensor to initialize
            sum += ads1115.readADC_SingleEnded(num_channel); // We read the values
            digitalWrite(power_pin, LOW);                    // We turn off the sensor
            delay(10);                                       // We wait for the results
        }
        break;
    case 1:
    case 2:
    case 3:
        // Serial.println("Soy yo, Concha. Entro");
        for (i = 1; i <= n_measure; i++)
        {
            sum += ads1115.readADC_SingleEnded(num_channel); // We read the values
        }
        break;
    default:
        Serial.println("Se ha producido un error inesperado, contacte con el administrador del dispositivo y muestrele el codigo de error \"ZEBRA\"");
        delay(1000);
    }
}
int Sensors::getMeasure()
{
    this->_ads1115;
    int result = takeMeasure(this->_num_chanel,);
}

#endif
