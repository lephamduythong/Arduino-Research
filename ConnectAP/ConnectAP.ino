#include <ESP8266WiFi.h>
//Enter SSID and Password of your Wi-Fi Router
const char* ssid = "Faceless2";
const char* password = "rongcon2296736511";
void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("Setting Wi-Fi Mode..");
  WiFi.mode(WIFI_STA); //Set Wi-Fi mode as station
  WiFi.begin(ssid, password); //Begin Connection
  Serial.println("Connecting to ");
  Serial.print(ssid);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP()); //Display IP assigned by Wi-Fi Router
}
void loop()
{
  Serial.printf("Signal Strength in dB = %d\n", WiFi.RSSI());
  delay(3000);
}
