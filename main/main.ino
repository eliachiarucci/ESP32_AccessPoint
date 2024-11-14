#include <WiFi.h>
#include <WiFiClient.h>
#include <EEPROM.h>
#include <WebServer.h>
#include <ESPmDNS.h>


String ssid = "";
String password = "";
const char* ap_ssid = "esp32-ap";
const char* ap_password = "testtesttest";
bool already_pressed = false;
bool connected = false;
WebServer server(80);

const int led = 2;

const int buttonPin = GPIO_NUM_0;
void setup(void) {
  Serial.begin(115200);
  pinMode(0, INPUT);
  pinMode(2, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  //eeprom_reset();
  establish_connection();
}

void establish_connection() {
  eeprom();
  uint8_t autoConnect = 0;
  WiFi.setAutoReconnect(autoConnect);
  //eeprom_reset();
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
      digitalWrite(led, HIGH);

      if (MDNS.begin("esp8266")) {
        Serial.println("MDNS responder started");
      }

      routes();

      server.begin();
      Serial.println("HTTP server started");
    } else {
      //access_point();
    }

  } else {
    access_point();
    digitalWrite(led, HIGH);
  }
}
int buttonState = 0;
void loop(void) {
  buttonState = digitalRead(buttonPin);
  server.handleClient();
  delay(10);
  if (buttonState == LOW) { // This can be either LOW or HIGH, might go on reset loop if wrong.
    // turn LED on:
    Serial.println("RESTARTING BECAUSE BUTTON PRESSED");
    if (already_pressed) {} else {
      already_pressed = true;
      eeprom_reset();
    }
    Serial.println("RESET");
    digitalWrite(led, HIGH);
  } else {
    already_pressed = false;
    //digitalWrite(led, LOW);
  }
}
