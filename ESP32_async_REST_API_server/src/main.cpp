#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "AsyncJson.h"
#include "ArduinoJson.h"

/*    LINK UTILI

https://developer.vonage.com/en/blog/using-vonage-sms-api-for-home-automation-using-esp32
https://raphaelpralat.medium.com/example-of-json-rest-api-for-esp32-4a5f64774a05
*/

AsyncWebServer server(80);
// variabili per wifi access point mode
const char *AP_ssid = "esp32AP";
IPAddress local_IP(192, 168, 4, 1); // non serve mettere il define e non so perchè
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 0, 0);

// variabili per wifi client mode
const char *ssid = "tortuga";
const char *password = "e9e444c1a4e9e.";

// dichiaro la dimensione del file Json chiamato jsonDocument
StaticJsonDocument<250> json;

// values
int val = 0;

// risposta di errore se viene chiesta una route non valida
void notFound(AsyncWebServerRequest *request)
{
    request->send(404, "application/json", "{\"message\":\"Not found\"}");
}

void routeSetup()
{
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "application/json", "{\"message\":\"Welcome\"}"); });

    AsyncCallbackJsonWebHandler *handler = new AsyncCallbackJsonWebHandler("/post", [](AsyncWebServerRequest *request, JsonVariant &json)
                                                                           {
    Serial.println("Request received");

    StaticJsonDocument<200> doc = json.as<JsonObject>();
    const char* message = doc["message"];
    Serial.print("Message received: ");
    Serial.println(message);

    if (!doc.containsKey("value")) {//gestione di parametri mancanti
      request->send(400, "application/json", "{\"message\":\"Missing 'value' key\"}");
      return;
    }

    // Supponiamo che il valore sia un numero
    int value = doc["value"];
    Serial.print("Value received: ");
    Serial.println(value);

    // Rispondi alla richiesta
    request->send(200, "application/json", "{\"message\":\"Data received\"}"); });
    server.addHandler(handler);
} // fine route setup

void wifiClientSetup()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.waitForConnectResult() != WL_CONNECTED) // aspetto qui finchè non sono connesso al wifi
    {
        Serial.printf("WiFi Failed!\n");
    }
    Serial.println("wifi CLIENT mode");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}

void wifiAPsetup()
{
    WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_ssid, password);
    Serial.println("wifi AP mode");
    if (!WiFi.config(local_IP, gateway, subnet))
    {
        Serial.println("STA Failed to configure");
    }
}

void setup()
{
    Serial.begin(115200);
    // wifiClientSetup();
    wifiAPsetup();
    routeSetup();
    server.onNotFound(notFound); // vedi funzione notFound
    server.begin();
}

void debug()
{
    static int debugWait = 5000;
    static long debugTimeTick = 0;

    if (millis() - debugTimeTick >= debugWait)
    {
        debugTimeTick = millis();
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
    }
}

void loop()
{
    // debug();
}
