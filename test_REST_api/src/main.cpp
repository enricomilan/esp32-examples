#include <Arduino.h>
//#include <ezButton.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

const char *SSID = "YOUR-SSID";
const char *PWD = "PASSWORD";

//dichiaro su che porta ascolta esp32
WebServer server(80);

//alloco lo spazio per il file Json
StaticJsonDocument<250> jsonDocument;
char buffer[250];

int temperature = 10;


// funzione chiamata dalla funzione getTemperature e crea il file Json
void create_json(int value)
{
  jsonDocument.clear();
  jsonDocument["type"] = "temperature";
  jsonDocument["value"] = value;
  jsonDocument["unit"] = "°C";
  serializeJson(jsonDocument, buffer);
}

//funzione chiamata dal webserver quando si esegue una chiamata GET a /temp
void getTemperature()
{
  Serial.println("Get temperature");
  create_json(temperature);
  server.send(200, "application/json", buffer);
}

//funzione chiamata dal webserver ogni volta che si esegue una chiama POST
void handlePost()
{
  if (server.hasArg("plain") == false)
  {
    //gestione dell'errore se il POST ricevuto è vuoto
  }
  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);//legge il file Json ed estrapola i dati

  int value = jsonDocument["value"];//nel file Json c'è scritto ad es "value: 100" int value = 100
  //int value = jsonDocument["value1"]; //gestisco anche più valori
  //int value = jsonDocument["value2"];
  Serial.print("value from client: ");
  Serial.println(value);

  server.send(200, "application/json", "{}");//non ho ancora letto a cosa serve ma se la togli non va più
}

//funzione per dichiarare le chiamate GET e POST che mi aspetto di ricevere posso chiamarla una volta sola dal void setup
void setup_routing()
{
  server.on("/temp", getTemperature);//GET
  //server.on("/value", getValue);//volendo gestisco più chiamate
  server.on("/led", HTTP_POST, handlePost);//POST
  //server.on("/elettrovalvola", HTTP_POST, handlePostEV);//forse posso gestire più POST devo ancora provare e non so se ha senso farlo
  server.begin();
}

void setup()
{
  Serial.begin(115200);
  Serial.print("Connecting to Wi-Fi");
  //robaccia per gestire il wifi
  WiFi.begin(SSID, PWD);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.print("Connected! IP Address: ");
  Serial.println(WiFi.localIP());
  setup_routing();
}

unsigned long tempo = 0;

void loop()
{
  if (millis() - tempo >= 1000)
  {
    tempo = millis();
    Serial.println("sono vivo");
  }
  server.handleClient(); //necessaria nel loop per continuare ad ascoltare le chiamate
}
