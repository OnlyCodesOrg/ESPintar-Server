#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>
#include "Web.h"

const int matrixSize = 16;
const int numberOfLEDs = matrixSize * matrixSize;


#define PIN 2 // Pin de la matriz de LEDs.
#define NUM_PIXELS 256

Adafruit_NeoPixel strip = Adafruit_NeoPixel(numberOfLEDs, PIN, NEO_GRB + NEO_KHZ800);

struct RGBColor
{
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

//------------------Servidor Web en puerto 80---------------------

WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);
//---------------------Credenciales de WiFi-----------------------

const char *ssid = "IZZI-414B_EXT2";
const char *password = "F88B373A414B";

//---------------------VARIABLES GLOBALES-------------------------
int contconexion = 0;

const int salida = 2;

//------------------------CODIGO HTML------------------------------

//---------------------------SETUP--------------------------------
void setup()
{
  Serial.begin(115200);
  Serial.println("");
  strip.begin();
  strip.show();

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED and contconexion < 50)
  {
    ++contconexion;
    delay(500);
    Serial.print(".");
  }

  if (contconexion < 50)
  {
    Serial.println("");
    Serial.println("WiFi conectado");
    Serial.println(WiFi.localIP());
    server.on("/", webpage);
    server.begin();
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
  }
  else
  {
    Serial.println("");
    Serial.println("Error de conexion");
  }
}

//----------------------------LOOP----------------------------------

void loop()
{
  webSocket.loop();
  server.handleClient();
}

void webpage()
{
  server.send(200, "text/html", pagina);
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t welength)
{
  if (type == WStype_TEXT)
  {
 
      Serial.println("Received WebSocket message:");
    Serial.println((char *)payload);

    // Parse the JSON message
    DynamicJsonDocument doc(20000);
    DeserializationError error = deserializeJson(doc, (char *)payload);

    if (error)
    {
      Serial.print("JSON parsing error: ");
      Serial.println(error.c_str());
      return;
    }
    uint32_t color;
    int x, y, pixelNum;
    RGBColor cRGB;
    const char* strPayload = doc.as<const char*>();
    String strPayloadString = strPayload;
    if (doc[0][0].is<JsonArray>())
    {
      Serial.print("Matriz recibida");

      size_t arrayLength = doc.size();

      for (size_t i = 0; i < arrayLength; i++)
      {
        x = doc[i][0][0];
        y = doc[i][0][1];
        cRGB.red = doc[i][1][0];
        cRGB.green = doc[i][1][1];
        cRGB.blue = doc[i][1][2];

        pixelNum = x + y * matrixSize;
        if(y%2!=0)
        { 
          pixelNum=(15-x)+y*matrixSize;
        }
        color = strip.Color(cRGB.red, cRGB.green, cRGB.blue);

        strip.setPixelColor(pixelNum, color);
      }
    }
    else if (doc[0].is<JsonArray>())
    {
      Serial.print("Pixel recibido");

      x = doc[0][0];
      y = doc[0][1];
      cRGB.red = doc[1][0];
      cRGB.green = doc[1][1];
      cRGB.blue = doc[1][2];

      pixelNum = x + y * matrixSize;
      if(y%2!=0)
      { 
        pixelNum=(15-x)+y*matrixSize;
      }
      color = strip.Color(cRGB.red, cRGB.green, cRGB.blue);

      strip.setPixelColor(pixelNum, color);
      
    }
    else if (doc.is<const char *>()) // Check for a string
    {
      Serial.print("Cadena recibida");

      const char *strPayload = doc.as<const char *>();

     if (strcmp(strPayload, "LIMPIAR") == 0)
     {
      Serial.println("LIMPIANDO MATRIZ");
      strip.clear();
      
     }
    }
  }
  strip.show();
//  else if (type == WStype_CONNECTED)
//  {
//    StaticJsonDocument<8196> doc;
//    JsonArray matrixArray = doc.to<JsonArray>();
//
//    for (int x = 0; x < matrixSize; x++)
//    {
//      for (int y = 0; y < matrixSize; y++)
//      {
//        // Pixel
//        JsonArray ledArray = doc.createNestedArray();
//
//        // Coordenadas
//        JsonArray coordinatesArray = ledArray.createNestedArray();
//        coordinatesArray.add(x);
//        coordinatesArray.add(y);
//
//        // RGB
//        JsonArray colorArray = ledArray.createNestedArray();
//        colorArray.add(matrix[x][y].red);
//        colorArray.add(matrix[x][y].green);
//        colorArray.add(matrix[x][y].blue);
//      }
//    }
//    String jsonStr;
//    serializeJson(doc, jsonStr);
//    webSocket.sendTXT(jsonStr);
//  }
}
