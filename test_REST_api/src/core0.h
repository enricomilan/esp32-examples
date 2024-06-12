/*per accedere alla pagina update : http://ip.del.esp32/update   */

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <WebServer.h>
#include <ArduinoJson.h>
//#include <FreeRTOS.h>

// Set your Static IP address
IPAddress local_IP(192, 168, 4, 1); //non serve mettere il define e non so perchè
// Set your Gateway IP address
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 0, 0);

const char* ssid = "ESP32_tapp_cucina";
const char* password = "12345678";

AsyncWebServer server(80);

void setup0(){
  if (!WiFi.config(local_IP, gateway, subnet)) {
  Serial.println("STA Failed to configure");
  }
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  Serial.println("started server");


  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hi! I am master.");
  });

  AsyncElegantOTA.begin(&server);    // Start ElegantOTA
  server.begin();
  Serial.println("HTTP server started");

}


void loop0(){
    //Serial.println("core0");
    delay(1);
}