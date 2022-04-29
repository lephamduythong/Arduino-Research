#include <ESP8266WiFi.h>

IPAddress staticIP(192, 168, 1, 10);
IPAddress gateway(192, 168, 1, 1); 
IPAddress subnet(255, 255, 255, 0);
IPAddress dns1(1, 1, 1, 1);
IPAddress dns2(8, 8, 8, 8);

const char* deviceName = "Vailua"; 
const char* ssid = "Faceless2";
const char* password = "rongcon2296736511";

void setup(void) {
  Serial.begin(115200);
  Serial.setDebugOutput(false);
  Serial.println("");
  // Prevent connecting to Wi-Fi based on previous configuration
  WiFi.disconnect();
  // DHCP Hostname (useful for finding device for static lease)
  WiFi.hostname(deviceName);
  if (!WiFi.config(staticIP, gateway, subnet, dns1, dns2)) {
    Serial.println("STA Failed to configure");
  }
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
}

void loop(void) {
  // Nothing to do here
  delay(1000);
}
