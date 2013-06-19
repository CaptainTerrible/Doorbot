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
    
    String msgPacket = "";
    String msgDoor = "";
    int msgFirstTab = 0;
    int msgSecondTab = 0;    
    String msgEvent = "";
    String msgSerial = "";
    String msgName = "";
 
    //Flash LED twice, with 0.5s delay
    FlashLed(2,500);   
    
    //Save the IP and port of the remote machine that sent the packet.
    int msgRemotePort = DoorBotListener.remotePort();
    IPAddress msgRemoteIP = DoorBotListener.remoteIP();
    
    //Zero packetBuffer    
    int i = 0;
    while (i < packetSize) {      
      PacketBuffer[i] = 0;
      i++;
    }
    
    //Read the waiting data
    DoorBotListener.read(PacketBuffer,MaxPacketSize);

    //Copy packetBuffer into a string object  
    msgPacket.concat(PacketBuffer);

    //Work out what door we're dealing with
    switch (ListenPort) {
    case 50000:
      msgDoor = "back";
      break;
    case 50002:
      msgDoor = "front";
      break;
    default:
      msgDoor = "unknown"; //Beware of those entering from this door. 
    }   

    //Split up msgPacket - this is hacky and doesn't work. **FIXME**    

    //Find the first tab character, after the event type
    msgFirstTab = msgPacket.indexOf('\n');
    //Find the second tab character, after the serial but before the name
    msgSecondTab = msgPacket.indexOf('\n',msgPacket.length());
    //First four characters are the event type 
    msgEvent = msgPacket.substring(0,4);
    //Characters between the first and second tabs are the serial
    msgSerial = msgPacket.substring(msgFirstTab+1,msgSecondTab);
    //Characters after the second tab are the name
    msgName = msgPacket.substring(msgSecondTab+1);

    //Dump the split parts of the event to serial
    Serial.print("Doorbot event from ");
    Serial.print(msgRemoteIP);
    Serial.print(":");
    Serial.print(msgRemotePort);
    Serial.println(" {");
    Serial.print("Door: ");
    Serial.println(msgDoor);
    Serial.print("Event type: ");
    Serial.println(msgEvent);
    Serial.print("Serial: ");
    Serial.println(msgSerial);    
    Serial.print("Name: ");
    Serial.println(msgName);        
    Serial.println("}");

    //Dump the raw packet received to aid in debugging
    Serial.println("==Raw Dump Start=="); 
    Serial.println(PacketBuffer);
    Serial.println("==Raw Dump End==");
  }
}

void FlashLed(int flashnum, int flashdelay)
{
  int i = 0;
  
  while (i < flashnum)
  {  
  digitalWrite(13, HIGH);
  delay(flashdelay);
  digitalWrite(13, LOW);
  delay(flashdelay);
  i++;
  Serial.println("FLASH!");
  }
}
