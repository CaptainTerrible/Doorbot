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

    String msgPacket = "";
    String msgDoor = "";
    int msgFirstTab = 0;
    int msgSecondTab = 0;    
    String msgEvent = "";
    String msgSerial = "";
    String msgName = "";
    
    int msgRemotePort = DoorBotListener.remotePort();
    IPAddress msgRemoteIP = DoorBotListener.remoteIP();
    
    DoorBotListener.read(packetBuffer,packetSize);

    //Copy packetBuffer into a string object  
    msgPacket.concat(packetBuffer);

    //Zero packetBuffer    
    int i = 0;
    while (i < packetSize) {      
      packetBuffer[i] = 0;
      i++;
    }
    
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

    //Dump the raw packet received to aid in debugging (ha!)
    Serial.println("==Raw Dump Start=="); 
    Serial.println(msgPacket);
    Serial.println("==Raw Dump End==");
  }
}
