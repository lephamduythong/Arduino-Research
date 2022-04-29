#include <ESP8266WiFi.h>

void setup(void) {
  Serial.begin(115200);
  Serial.println("");
  /* Set ESP32 to Wi-Fi Station mode */
  WiFi.mode(WIFI_STA);
  //Erases Previous Wi-Fi Configuration. It erases Wi-Fi config at ESP reset
  WiFi.disconnect(); //Comment this line in practical application
  //Without this line it will not connect to Wi-Fi
  // WiFi.begin(); //Connect to Wi-Fi
  //Display Previously Stored SSID and PASSWORD
  Serial.print("Local Stored SSID:");
  Serial.println(WiFi.SSID());
  Serial.print("Local Stored Password:");
  Serial.println(WiFi.psk());
  if (WiFi.SSID() == "") {
    //Start Smart Config
    WiFi.beginSmartConfig();
    //Wait for SmartConfig packet from mobile
    Serial.println("Waiting for SmartConfig.");
    //If Wi-Fi connects stop smart config
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("SmartConfig done.");
  }
  else
  {
    Serial.println("Connecting with previously stored configuration");
    while (WiFi.status() != WL_CONNECTED)
    {
      Serial.print(".");
      delay(500);
    }
  }
  Serial.println("");
  //Print Wi-Fi SSID
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());
  //Print connected network password
  Serial.print("Password:");
  Serial.println(WiFi.psk());
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop(void) {
}
