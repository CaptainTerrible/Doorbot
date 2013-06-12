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

#define packetsize 1024

byte mac[] = {  
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
  
char packetBuffer[packetsize];

//unsigned int ListenPort = 50000; //Back Door
unsigned int ListenPort = 50002; //Front Door

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
  Serial.print(ListenPort);
  Serial.println();

  DoorBotListener.begin(ListenPort);

}

void loop()
{
  int packetSize = DoorBotListener.parsePacket();
  
  if(packetSize)
  {
    DoorBotListener.read(packetBuffer,packetsize);
    
    String msgPacket = "";
    msgPacket.concat(packetBuffer);

    int msgFirstTab = msgPacket.indexOf('\n');
    int msgSecondTab = msgPacket.indexOf('\n',msgPacket.length());
    
    String msgEvent = msgPacket.substring(0,4);
    String msgSerial = msgPacket.substring(msgFirstTab,msgSecondTab);
    String msgName = msgPacket.substring(msgSecondTab);
    
    Serial.println("===== Start of Event =====");
    Serial.print("Event type:");
    Serial.println(msgEvent);
    Serial.print("Serial:");
    Serial.println(msgSerial);    
    Serial.print("Name:");
    Serial.println(msgName);        
    Serial.println("===== End of Event =====");
    
    Serial.println("Raw dump:"); 
    Serial.println(msgPacket);
  }
}





