#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* wifiName = "Faceless2";
const char* wifiPass = "rongcon2296736511";

unsigned int localPort = 2000; // local port to listen for UDP packets
IPAddress BroadCastIP(192, 168, 1, 255); //Modify first three numbers as per your IP range

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;
char packetBuffer[9]; //Where we get the UDP data

void setup()
{
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.print("Connecting");
  WiFi.begin(wifiName, wifiPass); //Connect to Wi-Fi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Wi-Fi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); //You can get IP address assigned to ESP
  
  // Start UDP
  Serial.println("Starting UDP");
  udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(udp.localPort());
}

void loop()
{
  int cb = udp.parsePacket();
  if (!cb) {
    // If serial data is recived send it to UDP
    if (Serial.available() > 0)
    {
      udp.beginPacket(BroadCastIP, localPort);
      //Send UDP requests are to port 2000
      char a[9];
      Serial.readBytes(a, 9); //Serial Byte Read
      udp.write(a, 9); //Send 9 bytes to ESP8266
      udp.endPacket();
    }
  }
  else {
    // We've received a UDP packet, send it to serial
    // Read the packet into the buffer, we are reading only one byte
    udp.read(packetBuffer, 9);
    for (int i = 0; i < 9; i++) {
      Serial.print((int)packetBuffer[i]);
      Serial.print("-");
    }
    Serial.println();
    Serial.print(packetBuffer);
    delay(20);
  }
}

/***
In linux
To send UDP packet use
  $echo "Test" > /dev/udp/192.168.1.9/2000
To see data recived use
  $sudo tcpdump -i wlp3s0 -n udp port 2000 -X
***/
