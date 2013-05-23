/* 
Example doorbot listener

Tim Reynolds tim@christwithfries.net
*/

#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>
#include <util.h>
#include <SPI.h>

byte mac[] = {  
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
  
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
unsigned int BroadcastPort = 50000;

EthernetUDP DoorBotListener;

void setup()
{
  Serial.begin(9600);
  Serial.println("Example doorbot listener starting up.");

  Serial.print("Attempting to get IP via DHCP...");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("failed.");
    Serial.println("Unable to configure Ethernet using DHCP, halting.");
    for(;;)
      ;
  } 
  else {
    Serial.println("done.");
    Serial.print("IP address: ");
    for (byte thisByte = 0; thisByte < 4; thisByte++) {
      Serial.print(Ethernet.localIP()[thisByte], DEC);
      Serial.print("."); 
    }
  }

  Serial.println();
  Serial.print("Starting listener on port ");
  Serial.print(BroadcastPort);
  Serial.println();

  DoorBotListener.begin(BroadcastPort);

}

void loop()
{
  // if there's data available, read a packet
  int packetSize = DoorBotListener.parsePacket();
  if(packetSize)
  {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remote = DoorBotListener.remoteIP();
    for (int i =0; i < 4; i++)
    {
      Serial.print(remote[i], DEC);
      if (i < 3)
      {
        Serial.print(".");
      }
    }
    Serial.print(", port ");
    Serial.println(DoorBotListener.remotePort());

    // read the packet into packetBufffer
    DoorBotListener.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);
    Serial.println("Contents:");
    Serial.println(packetBuffer);
  }
}





