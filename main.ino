#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>


String ssid = "";
String password = "";
const char* ap_ssid = "remote-key";
const char* ap_password = "testtesttest";
bool already_pressed = false;
bool connected = false;

ESP8266WebServer server(80);

const int led = 2;

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "hello from esp8266!");
  digitalWrite(led, 0);
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}
const int buttonPin = 4; 
void setup(void) {
  
  //eeprom_reset();
  establish_connection();

}

void establish_connection() {
  eeprom();
  uint8 autoConnect = 0;
  WiFi.setAutoConnect(autoConnect);
  //eeprom_reset();
  Serial.begin(115200);
  Serial.println();
  Serial.println(ssid);
  Serial.println(password);
  if (ssid != "" || password != "") {
    pinMode(led, OUTPUT);
    digitalWrite(led, 0);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("");

    // Wait for connection
    int times = 0;
    while (WiFi.status() != WL_CONNECTED && times < 20) {
      delay(500);
      Serial.print(".");
      times++;
      if (WiFi.status() == WL_CONNECTED) {
        connected = true;
      }
    }

    if (connected == true) {
      Serial.println("");
      Serial.print("Connected to ");
      Serial.println(ssid);
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());

      if (MDNS.begin("esp8266")) {
        Serial.println("MDNS responder started");
      }

      server.on("/", handleRoot);

      server.on("/inline", []() {
        server.send(200, "text/plain", "this works as well");
      });

      server.onNotFound(handleNotFound);

      server.begin();
      Serial.println("HTTP server started");
    } else {
      //access_point();
    }

  } else {
    access_point();
  }
}
int buttonState = 0; 
void loop(void) {
  server.handleClient();
  MDNS.update();
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    // turn LED on:
    if(already_pressed) {} else {already_pressed = true; eeprom_reset();}
    Serial.println("RESET");
    digitalWrite(led, HIGH);
  } else {
    // turn LED off:
    already_pressed = false;
    digitalWrite(led, LOW);
  }
}
