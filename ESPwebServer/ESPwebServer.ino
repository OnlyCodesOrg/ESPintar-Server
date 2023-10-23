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
RGBColor matriz[16][16];

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
    DynamicJsonDocument doc(10000);
    DeserializationError error = deserializeJson(doc, (char *)payload);

    if (error)
    {
      Serial.print("JSON parsing error: ");
      Serial.println(error.c_str());
      return;
    }

    uint32_t color;
    uint8_t x, y, pixelNum;
    RGBColor cRGB;
    const char *strPayload = doc.as<const char *>();
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
        matriz[x][y] = cRGB; // Matriz local
        pixelNum = calcPixelNum(x, y);

        color = strip.Color(cRGB.red, cRGB.green, cRGB.blue);
        strip.setPixelColor(pixelNum, color);
      }
      enviarMatriz(num); // Enviar matriz a otros clientes
    }
    else if (doc[0].is<JsonArray>())
    {
      Serial.print("Pixel recibido");
      DynamicJsonDocument docPixel(512);
      JsonArray pixelArray = docPixel.to<JsonArray>();

      x = doc[0][0];
      y = doc[0][1];
      cRGB.red = doc[1][0];
      cRGB.green = doc[1][1];
      cRGB.blue = doc[1][2];
      matriz[x][y] = cRGB; // Matriz local
      pixelNum = calcPixelNum(x, y);

      color = strip.Color(cRGB.red, cRGB.green, cRGB.blue);
      strip.setPixelColor(pixelNum, color);

      // Envia pixel a otros clientes
      JsonArray pixelPos = pixelArray.createNestedArray();
      pixelPos.add(x);
      pixelPos.add(y);

      JsonArray pixelRGB = pixelArray.createNestedArray();
      pixelRGB.add(matriz[x][y].red);
      pixelRGB.add(matriz[x][y].green);
      pixelRGB.add(matriz[x][y].blue);

      String jsonStr;
      serializeJson(docPixel, jsonStr);
      char payloadBuffer[jsonStr.length() + 1];
      jsonStr.toCharArray(payloadBuffer, sizeof(payloadBuffer));
      webSocket.sendTXT(num, payloadBuffer);
    }
    else if (doc.is<const char *>()) // Check for a string
    {
      Serial.print("Cadena recibida");

      const char *strPayload = doc.as<const char *>();

      if (strcmp(strPayload, "LIMPIAR") == 0)
      {
        Serial.println("LIMPIANDO MATRIZ");
        strip.clear();

        String jsonStr = "LIMPIAR";
        char payloadBuffer[jsonStr.length() + 1];
        jsonStr.toCharArray(payloadBuffer, sizeof(payloadBuffer));
        webSocket.sendTXT(num, payloadBuffer);
      }
    }
  }
  else if (type == WStype_CONNECTED)
  {
    enviarMatriz(num);
  }
  strip.show();
}

void enviarMatriz(uint8_t num)
{
  DynamicJsonDocument docGrid(10000);
  JsonArray matrixArray = docGrid.to<JsonArray>();

  for (int x = 0; x < matrixSize; x++)
  {
    for (int y = 0; y < matrixSize; y++)
    {
      if (matriz[x][y].red != 0 && matriz[x][y].green != 0 && matriz[x][y].blue != 0)
      {
        JsonArray ledArray = docGrid.createNestedArray();

        JsonArray pos = ledArray.createNestedArray();
        pos.add(x);
        pos.add(y);

        JsonArray rgb = ledArray.createNestedArray();
        rgb.add(matriz[x][y].red);
        rgb.add(matriz[x][y].green);
        rgb.add(matriz[x][y].blue);
      }
    }
  }

  String jsonStr;
  serializeJson(docGrid, jsonStr);
  char payloadBuffer[jsonStr.length() + 1];
  jsonStr.toCharArray(payloadBuffer, sizeof(payloadBuffer));
  webSocket.sendTXT(num, payloadBuffer);
}

uint8_t calcPixelNum(uint8_t x, uint8_t y)
{
  uint8_t pixelNum = x + y * matrixSize;
  if (y % 2 != 0)
  {
    pixelNum = (15 - x) + y * matrixSize;
  }

  return pixelNum;
}