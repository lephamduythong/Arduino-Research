#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid     = "Faceless";
const char* password = "rongcon2296736511";

ESP8266WebServer server(5555);

// Assign output variables to GPIO pins
const char output = D2;
const char statusPC = D1;

IPAddress local_IP(192, 168, 1, 123);
IPAddress subnet(255, 255, 255, 0);
IPAddress gateway(192, 168, 1, 1);
IPAddress primaryDNS(1, 1, 1, 1);   
IPAddress secondaryDNS(8, 8, 8, 8); 

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

  server.on("/", []() {
    server.send(200, "text/plain", "Hello from esp8266 over HTTP!");
  });

  server.on("/on", []() {
    char checkStatus = digitalRead(statusPC);
    if (checkStatus == HIGH) {
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(output, HIGH);
      delay(1000);
      digitalWrite(output, LOW);
      digitalWrite(LED_BUILTIN, LOW);
      server.send(200, "text/plain", "Sent on signal successful");
    }
    else {
      server.send(200, "text/plain", "Your PC is already waked up, use /off if you want to turn off the PC");
    }
  });

  server.on("/off", []() {
    char checkStatus = digitalRead(statusPC);
    if (checkStatus == LOW)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(output, HIGH);
      delay(1000);
      digitalWrite(output, LOW);
      digitalWrite(LED_BUILTIN, LOW);
      server.send(200, "text/plain", "Sent off signal successful");
    }
    else {
      server.send(200, "text/plain", "Your PC is offline, use /on if you want to turn on the PC");
    }
  });

  server.on("/status", []() {
    char checkStatus = digitalRead(statusPC);
    if (checkStatus == HIGH) {
      server.send(200, "text/plain", "Online");
    }
    else {
      server.send(200, "text/plain", "Offline");
    }
  });
  
  server.begin();
  Serial.println("HTTP server started");
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT); 
  digitalWrite(LED_BUILTIN, LOW);
  pinMode(output, OUTPUT);
  digitalWrite(output, LOW);
  pinMode(statusPC, INPUT);
  connectWIFI();
}

void loop(){
  delay(1000);
  if (WiFi.status() == WL_CONNECTION_LOST || WiFi.status() == WL_CONNECT_FAILED || WiFi.status() == WL_DISCONNECTED)
  {
    Serial.println("Reconnecting...");
    connectWIFI();
  }
  server.handleClient();
  MDNS.update();
}