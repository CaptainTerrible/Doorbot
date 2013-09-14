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

//Maximum size for packet buffer. Default (UDP_TX_PACKET_MAX_SIZE) is very low.
#define MaxPacketSize 1024

//This is the MAC address the shield will use. Please change it!
byte mac[] = {  
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

//Buffer for storing incoming ethernet packets
char PacketBuffer[MaxPacketSize];

//Choose a port to listen  on. 
//unsigned int ListenPort = 50000; //Back Door
unsigned int ListenPort = 50002; //Front Door

//Initialise UDP listener
EthernetUDP DoorBotListener;

void setup()
{
  //Enable onboard LED
  pinMode(13, OUTPUT);

  //Setup serial monitor
  Serial.begin(9600);

  Serial.println("Example doorbot listener starting up.");

  Serial.print("Attempting to get IP via DHCP...");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("failed.");
    Serial.println("Unable to configure Ethernet using DHCP, halting.");
    //Can't get an IP, sit in an endless loop.
    for(;;)
      ;
  } 
  else {
    Serial.println("done.");
    Serial.print("IP address: ");
    //Serial.print(Ethernet.localIP());
    for (byte thisByte = 0; thisByte < 4; thisByte++) {
      Serial.print(Ethernet.localIP()[thisByte], DEC);
      Serial.print("."); 
    }
  }

  Serial.println();
  Serial.print("Starting listener on port ");
  Serial.print(ListenPort);
  Serial.println();

  //We have an IP, start listening.
  DoorBotListener.begin(ListenPort);

}

void loop()
{
  //Check if we have data waiting
  int packetSize = DoorBotListener.parsePacket();

  if(packetSize)
  { 
    //Read the waiting data
    DoorBotListener.read(PacketBuffer,MaxPacketSize);

    //Dump the raw packet
    Serial.println("Doorbot Event {");
    Serial.println(PacketBuffer);
    Serial.println("}");
    
    //Clear PacketBuffer
    memset(PacketBuffer, 0, sizeof(PacketBuffer));

  }
}


