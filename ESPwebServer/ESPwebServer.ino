#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include "Web.h"

//WebSocketsClient webSocket;

const int matrixSize = 16;
const int numberOfLEDs = matrixSize * matrixSize;

struct RGBColor {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};
//------------------Servidor Web en puerto 80---------------------

WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);
//---------------------Credenciales de WiFi-----------------------

const char* ssid     = "Nombre de tu internet";
const char* password = "ingresa tu contraseña";

//---------------------VARIABLES GLOBALES-------------------------
int contconexion = 0;

const int salida = 2;

//------------------------CODIGO HTML------------------------------



//---------------------------SETUP--------------------------------
void setup() {
  Serial.begin(115200);
  Serial.println("");
  WiFi.begin(ssid,password);

  while (WiFi.status() != WL_CONNECTED and contconexion <50) { 
    ++contconexion;
    delay(500);
    Serial.print(".");
  }
  if (contconexion <50) {
      //para usar con ip fija
      //IPAddress ip(192,168,1,180); 
      //IPAddress gateway(192,168,1,1); 
      //IPAddress subnet(255,255,255,0); 
      //WiFi.config(ip, gateway, subnet); 
      
      Serial.println("");
      Serial.println("WiFi conectado");
      Serial.println(WiFi.localIP());
      server.on("/", webpage);
      server.begin(); webSocket.begin();
      webSocket.onEvent(webSocketEvent);
  }
  else { 
      Serial.println("");
      Serial.println("Error de conexion");
  }
}

//----------------------------LOOP----------------------------------

void loop(){
  webSocket.loop(); server.handleClient();

}
void webpage()
{
  server.send(200,"text/html", pagina);
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t welength) {
  if (type == WStype_TEXT) {
    Serial.println("Received WebSocket message:");
    Serial.println((char*)payload);

    // Parse the JSON message
    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, (char*)payload);

    if (error) {
      Serial.print("JSON parsing error: ");
      Serial.println(error.c_str());
      return;
    }

    if (doc.size() != 2) {
      Serial.println("Invalid JSON message format. Expecting a 2-element array.");
      return;
    }

    int x = doc[0][0];
    int y = doc[0][1];
    RGBColor color;
    color.red = doc[1][0];
    color.green = doc[1][1];
    color.blue = doc[1][2];

    Serial.print(x);
    Serial.println("");
    Serial.print(y);

    
  }
}
