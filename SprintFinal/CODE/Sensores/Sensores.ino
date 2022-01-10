
#include <ESP8266WiFi.h>
#include <Wire.h>             // Library to use the I2C BUS.
#include <Adafruit_ADS1X15.h> // ADS1115 Library.
#include <Sensors.h>          // Library to use the sensors.

// Comentar/Descomentar para ver mensajes de depuracion en monitor serie y/o respuesta del HTTP server
#define PRINT_DEBUG_MESSAGES
//#define PRINT_HTTP_RESPONSE

// Comentar/Descomentar para conexion Fuera/Dentro de UPV
//#define WiFi_CONNECTION_UPV

// Selecciona que servidor REST quieres utilizar entre ThingSpeak y Dweet
#define REST_SERVER_THINGSPEAK //Selecciona tu canal para ver los datos en la web (https://thingspeak.com/channels/360979)
//#define REST_SERVER_DWEET //Selecciona tu canal para ver los datos en la web (http://dweet.io/follow/PruebaGTI)

// ------------------------------------------------------------
// Builders
// ------------------------------------------------------------

Adafruit_ADS1115 ads1115; // Ads1115 builder
Sensors sensors;          // Sensors Builder

// ------------------------------------------------------------
// WiFi Definitions
// ------------------------------------------------------------

#ifdef WiFi_CONNECTION_UPV //Conexion UPV
const char WiFiSSID[] = "GTI1";
const char WiFiPSK[] = "1PV.arduino.Toledo";
#else //Conexion fuera de la UPV
const char WiFiSSID[] = "WifiPepe";
const char WiFiPSK[] = "QWERTY123456";
#endif

// ------------------------------------------------------------
// SERVER Definitions
// ------------------------------------------------------------

#if defined(WiFi_CONNECTION_UPV) //Conexion UPV
const char Server_Host[] = "proxy.upv.es";
const int Server_HttpPort = 8080;
#elif defined(REST_SERVER_THINGSPEAK) //Conexion fuera de la UPV
const char Server_Host[] = "api.thingspeak.com";
const int Server_HttpPort = 80;
#else
const char Server_Host[] = "dweet.io";
const int Server_HttpPort = 80;
#endif

WiFiClient client;

// ------------------------------------------------------------
// HTTP REST Connection
// ------------------------------------------------------------

#ifdef REST_SERVER_THINGSPEAK
const char Rest_Host[] = "api.thingspeak.com";
String MyWriteAPIKey = "GR7GUHOV3CNDO2EO"; // Escribe la clave de tu canal ThingSpeak
#else
const char Rest_Host[] = "dweet.io";
String MyWriteAPIKey = "cdiocurso2021_2022g07.."; // Escribe la clave de tu canal Dweet
#endif

#define NUM_FIELDS_TO_SEND 4 //Numero de medidas a enviar al servidor REST (Entre 1 y 8)

// ------------------------------------------------------------
// Pin Definitions
// ------------------------------------------------------------

const int LED_PIN = 5; // Thing's onboard, green LED

const int powerPin = 5; // GPIO 5 is used to power the salinity probe

// ------------------------------------------------------------
// WiFi Connection
// ------------------------------------------------------------

void connectWiFi()
{
  byte ledStatus = LOW;

#ifdef PRINT_DEBUG_MESSAGES
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());
#endif

  WiFi.begin(WiFiSSID, WiFiPSK);

  while (WiFi.status() != WL_CONNECTED)
  {
    // Blink the LED
    digitalWrite(LED_PIN, ledStatus); // Write LED high/low
    ledStatus = (ledStatus == HIGH) ? LOW : HIGH;
#ifdef PRINT_DEBUG_MESSAGES
    Serial.println(".");
#endif
    delay(500);
  }
#ifdef PRINT_DEBUG_MESSAGES
  Serial.println("WiFi Connected");
  Serial.println(WiFi.localIP()); // Print the IP address
#endif
}

// ------------------------------------------------------------
// HTTP POST  ThingSpeak
// ------------------------------------------------------------

void HTTPPost(String fieldData[], int numFields)
{

  // Esta funcion construye el string de datos a enviar a ThingSpeak mediante el metodo HTTP POST
  // La funcion envia "numFields" datos, del array fieldData.
  // Asegurate de ajustar numFields al número adecuado de datos que necesitas enviar y activa los campos en tu canal web

  if (client.connect(Server_Host, Server_HttpPort))
  {

    // Construimos el string de datos. Si tienes multiples campos asegurate de no pasarte de 1440 caracteres

    String PostData = "api_key=" + MyWriteAPIKey;
    for (int field = 1; field < (numFields + 1); field++)
    {
      PostData += "&field" + String(field) + "=" + fieldData[field];
    }

    String humedad = "&Humedad" + String() + "=" + fieldData[0];

// POST data via HTTP
#ifdef PRINT_DEBUG_MESSAGES
    Serial.println("Connecting to ThingSpeak for update...");
#endif
    client.println("POST http://" + String(Rest_Host) + "/update HTTP/1.1");
    client.println("Host: " + String(Rest_Host));
    client.println("Connection: close");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.println("Content-Length: " + String(PostData.length()));
    client.println();
    client.println(PostData);
#ifdef PRINT_DEBUG_MESSAGES
    Serial.println(PostData);
    Serial.println();
//Para ver la respuesta del servidor
#ifdef PRINT_HTTP_RESPONSE
    delay(500);
    Serial.println();
    while (client.available())
    {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
    Serial.println();
    Serial.println();
#endif
#endif
  }
}

// ------------------------------------------------------------
// HTTP GET
// ------------------------------------------------------------

void HTTPGet(String fieldData[], int numFields)
{

  // Esta funcion construye el string de datos a enviar a ThingSpeak o Dweet mediante el metodo HTTP GET
  // La funcion envia "numFields" datos, del array fieldData.
  // Asegurate de ajustar "numFields" al número adecuado de datos que necesitas enviar y activa los campos en tu canal web

  if (client.connect(Server_Host, Server_HttpPort))
  {
#ifdef REST_SERVER_THINGSPEAK
    String PostData = "GET https://api.thingspeak.com/update?api_key=";
    PostData = PostData + MyWriteAPIKey; //
#else
    String PostData = "GET http://dweet.io/dweet/for/";
    PostData = PostData + MyWriteAPIKey + "?";
#endif

    for (int field = 1; field < (numFields + 1); field++)
    {
      PostData += "&field" + String(field) + "=" + fieldData[field];
    }

#ifdef PRINT_DEBUG_MESSAGES
    Serial.println("Connecting to Server for update...");
#endif
    client.print(PostData);
    client.println(" HTTP/1.1");
    client.println("Host: " + String(Rest_Host));
    client.println("Connection: close");
    client.println();
#ifdef PRINT_DEBUG_MESSAGES
    Serial.println(PostData);
    Serial.println();
//Para ver la respuesta del servidor
#ifdef PRINT_HTTP_RESPONSE
    delay(500);
    Serial.println();
    while (client.available())
    {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
    Serial.println();
    Serial.println();
#endif
#endif
  }
}

void setup()
{

#ifdef PRINT_DEBUG_MESSAGES
  Serial.begin(9600);
#endif

  connectWiFi();
  digitalWrite(LED_PIN, HIGH);

#ifdef PRINT_DEBUG_MESSAGES
  Serial.print("Server_Host: ");
  Serial.println(Server_Host);
  Serial.print("Port: ");
  Serial.println(String(Server_HttpPort));
  Serial.print("Server_Rest: ");
  Serial.println(Rest_Host);
#endif

  //---> Set power_pin as output pin
  pinMode(powerPin, OUTPUT);

  //--->  Initialize the ads1115
  ads1115.begin();

  //---> Adjust the gain of ads1115
  ads1115.setGain(GAIN_ONE);

  //---> Send ads1115 to sensors.
  sensors.setAdaFruit(ads1115);
}

void loop()
{

  String data[NUM_FIELDS_TO_SEND + 1]; // Podemos enviar hasta 8 datos

  //---> Salinity Sensor.
  int salinityValue = sensors.getSalinityValue();
  if (salinityValue >= 100)
  {
    salinityValue = 100;
  }
  else if (salinityValue <= 0)
  {
    salinityValue = 0;
  }
  data[1] = String(salinityValue);
  //data[1] = String(random(1, 99));
#ifdef PRINT_DEBUG_MESSAGES
  Serial.print("Salinidad: ");
  Serial.print(data[1]);
  Serial.println("%");
#endif

  //---> Humidity Sensor.
  int humidityValue = sensors.getHumidityValue();
  if (humidityValue >= 100)
  {
    humidityValue = 100;
  }
  else if (humidityValue <= 0)
  {
    humidityValue = 0;
  }
  data[2] = String(humidityValue);
  //data[2] = String(random(1, 99));
#ifdef PRINT_DEBUG_MESSAGES
  Serial.print("Humedad: ");
  Serial.print(data[2]);
  Serial.println("%");
#endif

  //---> Temperature Sensor.
  int temperatureValue = sensors.getTemperatureValue();
  data[3] = String(temperatureValue);
  //data[3] = String(random(1, 99));
#ifdef PRINT_DEBUG_MESSAGES
  Serial.print("Temperatura: ");
  Serial.print(data[3]);
  Serial.println("º");
#endif

  //---> Light sensor.
  int lightnessValue = sensors.getLightnessValue();
  if (lightnessValue >= 100)
  {
    lightnessValue = 100;
  }
  else if (lightnessValue <= 0)
  {
    lightnessValue = 0;
  }
  data[4] = String(lightnessValue);
  //data[4] = String(random(1, 99));
#ifdef PRINT_DEBUG_MESSAGES
  Serial.print("Porcentaje de luz: ");
  Serial.print(data[4]);
  Serial.println("%");
#endif

  //Selecciona si quieres enviar con GET(ThingSpeak o Dweet) o con POST(ThingSpeak)
  HTTPPost(data, NUM_FIELDS_TO_SEND);
  //HTTPGet( data, NUM_FIELDS_TO_SEND );

  //Selecciona si quieres un retardo de 15seg para hacer pruebas o dormir el SparkFun
  delay(1500);//15000
  //Serial.print( "Goodnight" );
  //ESP.deepSleep( sleepTimeSeconds * 1000000 );
}
