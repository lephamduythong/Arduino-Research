#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

ESP8266WebServer server(80);

const char* ssid = "Faceless2";
const char* password = "rongcon2296736511";

void setup(void) {
  Serial.begin(115200);
  Serial.setDebugOutput(false);
  Serial.println("");
  // Prevent connecting to Wi-Fi based on previous configuration
  WiFi.disconnect();
  WiFi.begin(ssid, password);
  WiFi.mode(WIFI_STA); //Wi-Fi mode station (connect to Wi-Fi router only
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); //IP address assigned to your ESP

  if (MDNS.begin("test")) {
    Serial.println("MDNS started, type test.local in browser to check");
  }

  server.on("/", []() {
    server.send(200, "text/plain", "hello from esp8266! mDNS demo");
  });
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  MDNS.update();
}
