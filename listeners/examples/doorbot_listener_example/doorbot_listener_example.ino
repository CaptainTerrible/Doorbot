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

    int msgRemotePort = DoorBotListener.remotePort();
    IPAddress msgRemoteIP = DoorBotListener.remoteIP();

    DoorBotListener.read(packetBuffer,packetsize);
    
    //Convert packetBuffer into a string object  
    String msgPacket = "";
    msgPacket.concat(packetBuffer);
    
    //Find the first tab character, after the event type
    int msgFirstTab = msgPacket.indexOf('\n');
    //Find the second tab character, after the serial but before the name
    int msgSecondTab = msgPacket.indexOf('\n',msgPacket.length());
    
    //Chop up msgPacket - this is hacky and doesn't work.
    
    //First four characters are the event type 
    String msgEvent = msgPacket.substring(0,4);
    //Characters between the first and second tabs are the serial
    String msgSerial = msgPacket.substring(msgFirstTab+1,msgSecondTab);
    //Characters after the second tab are the name
    String msgName = msgPacket.substring(msgSecondTab+1);
    
    //Dump the split parts of the event to serial
    Serial.print("Doorbot event from ");
    Serial.print(msgRemoteIP);
    Serial.print(":");
    Serial.print(msgRemotePort);
    Serial.println(" {");
    Serial.print("Event type:");
    Serial.println(msgEvent);
    Serial.print("Serial:");
    Serial.println(msgSerial);    
    Serial.print("Name:");
    Serial.println(msgName);        
    Serial.println("}");
    
    //Dump the raw packet received to aid in debugging (ha!)
    Serial.println("Raw dump:"); 
    Serial.println(msgPacket);
  }
}





