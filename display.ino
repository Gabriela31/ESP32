// librerias
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

AsyncWebServer server(80);

// variables del conexion
#define RXD2 16
#define TXD2 17

// hotspot
const char* ssid = "gabs";
const char* password = "hugovega123";

// variable que recibe desde la web
const char* PARAM_INPUT_1 = "input1";

// si la pagina no se encuentra sale esto
void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void setup() {
  Serial.begin(9600);
  if(!SPIFFS.begin()){
   Serial.println("Error con el SPIFFS");
   return;
}
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("¡Falló WiFi!");
    return;
  }

  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Send web page with input fields to client
    server.on("/dashboard", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/dashboard.html", "text/html");
  });
 
  server.on("/dashboard.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/dashboard.css", "text/css");
  });

  server.on("/icon", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/icon.png", "image/png");
  });

  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      String hola = '~' + inputMessage + '}';
      //Serial.println(inputMessage);
      while(Serial2.available()){
        Serial2.println(hola);
        //Serial2.write(hola);
        break; 
      }
    }
    else {
      inputMessage = "No se envio el mensaje";
      inputParam = "none";
    }
    Serial.println(inputMessage);
    request->send(SPIFFS, "/dashboard.html", "text/html");
    /*request->send(200, "text/html", "Se envió satisfactoriamente el siguiente mensaje: " + inputMessage + "<br><a href=\"/\">Regresar al inicio</a>");*/
  });
  server.onNotFound(notFound);
  server.begin();
}

void loop() {
  // como es asincrona no hay nada en el loop
}
