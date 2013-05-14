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

unsigned int BroadcastPort = 50000;

EthernetUDP UDP;

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
  
  /*
    The standard Arduino ethernet library does not support multicast. Modified version does.
    http://tkkrlab.nl/wiki/Arduino_UDP_multicast
  */
  
}

void loop()
{

}


