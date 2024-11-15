#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Preferences.h>

String ssid;
String password;
const char* ap_ssid = "esp32-ap";
const char* ap_password = "testtesttest";
bool already_pressed = false;
bool connected = false;
WebServer server(80);

const int led = 2;

const int buttonPin = GPIO_NUM_0;
void setup(void) {
  Serial.begin(115200);
  establish_connection();
}

void establish_connection() {
  ssid = get_saved_ssid();
  password = get_saved_pwd();
  bool autoConnect = true;
  WiFi.setAutoReconnect(autoConnect);
  
  Serial.println();
  Serial.println(ssid);
  Serial.println(password);
  if (ssid != "" && password != "") {
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

      if (MDNS.begin("esp32")) {
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
  //buttonState = digitalRead(buttonPin);
  server.handleClient();
  delay(10);
  //Serial.println(buttonState);
  if (buttonState == HIGH) {  // This can be either LOW or HIGH, might go on reset loop if wrong.
    // turn LED on:
    Serial.println("RESTARTING BECAUSE BUTTON PRESSED");
    if (already_pressed) {
    } else {
      already_pressed = true;
      storage_reset();
    }
    Serial.println("RESET");
    digitalWrite(led, HIGH);
  } else {
    already_pressed = false;
    //digitalWrite(led, LOW);
  }
}
