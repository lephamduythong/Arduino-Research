#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>
#include <EasyDDNS.h>

#define LED_BUILTIN 2

static const uint8_t D0   = 16;
static const uint8_t D1   = 5;
static const uint8_t D2   = 4;
static const uint8_t D3   = 0;
static const uint8_t D4   = 2;
static const uint8_t D5   = 14;
static const uint8_t D6   = 12;
static const uint8_t D7   = 13;
static const uint8_t D8   = 15;
static const uint8_t RX   = 3;
static const uint8_t TX   = 1;

const char* ssid     = "Faceless";
const char* password = "rongcon2296736511";

ESP8266WebServer server(5555);

// Assign output variables to GPIO pins
const int output = D2;
const int statusPC = D0;
const int resetPin = D1;

IPAddress local_IP(192, 168, 1, 123);
IPAddress subnet(255, 255, 255, 0);
IPAddress gateway(192, 168, 1, 1);
IPAddress primaryDNS(1, 1, 1, 1);   
IPAddress secondaryDNS(8, 8, 8, 8); 

void sendSignal() {
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(output, HIGH);
    delay(750);
    digitalWrite(output, LOW);
    digitalWrite(LED_BUILTIN, LOW);
    server.send(200, "text/plain", "Sent the signal successful");
}

void hanleRouting() {
  server.on("/", []() {
    server.send(200, "text/plain", "Hello from esp8266 over HTTP!");
  });

  server.on("/on", []() {
    int checkStatus = digitalRead(statusPC);
    if (checkStatus == HIGH) {
      sendSignal();
    }
    else {
      server.send(200, "text/plain", "Your PC is already waked up, use /off if you want to turn off the PC");
    }
  });

  server.on("/off", []() {
    int checkStatus = digitalRead(statusPC);
    if (checkStatus == LOW)
    {
      sendSignal();
    }
    else {
      server.send(200, "text/plain", "Your PC is offline, use /on if you want to turn on the PC");
    }
  });

  server.on("/reset", []() {
    int checkStatus = digitalRead(statusPC);
    if (checkStatus == HIGH) {
      server.send(200, "text/plain", "Your PC is turned off");
    }
    else {
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(resetPin, HIGH);
      delay(100);
      digitalWrite(resetPin, LOW);
      digitalWrite(LED_BUILTIN, LOW);
      server.send(200, "text/plain", "Sent the signal successful");
    }
    Serial.println(checkStatus);
  });

  server.on("/status", []() {
    int checkStatus = digitalRead(statusPC);
    if (checkStatus == HIGH) {
      server.send(200, "text/plain", "Offline");
    }
    else {
      server.send(200, "text/plain", "Online");
    }
  });
}

void connectWIFI()
{
  WiFi.mode(WIFI_STA);
  
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
  
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    Serial.print(".");
    digitalWrite(LED_BUILTIN, LOW);
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  hanleRouting();
  
  server.begin();
  Serial.println("HTTP server started");
}

void setup() {
  Serial.begin(115200);
  
  pinMode(LED_BUILTIN, OUTPUT); 
  digitalWrite(LED_BUILTIN, LOW);
  
  pinMode(output, OUTPUT);
  digitalWrite(output, LOW);
  
  pinMode(resetPin, OUTPUT);
  digitalWrite(resetPin, LOW);
  
  pinMode(statusPC, INPUT);

  // DDNS setup
  Serial.println("Begin DDNS Setup");
  EasyDDNS.service("noip");
  EasyDDNS.client("thonglee.ddns.net", "strimpize2296", "BsIG8\"s$Et1GdbS,");
  EasyDDNS.onUpdate([&](const char* oldIP, const char* newIP){
    Serial.print("EasyDDNS - IP Change Detected: ");
    Serial.println(newIP);
  });
  Serial.println("End DDNS Setup");
  
  connectWIFI();
}

void loop(){
  delay(1000);
  EasyDDNS.update(10000);
  if (WiFi.status() == WL_CONNECTION_LOST || WiFi.status() == WL_CONNECT_FAILED || WiFi.status() == WL_DISCONNECTED)
  {
    Serial.println("Reconnecting...");
    connectWIFI();
  }
  server.handleClient();
  MDNS.update();
}
