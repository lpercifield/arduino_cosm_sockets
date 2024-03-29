/*
  DNS and DHCP-based Web client
 
 This sketch connects to a website (http://www.google.com)
 using an Arduino Wiznet Ethernet shield. 
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 
 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe, based on work by Adrian McEwen
 
 */

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };
char serverName[] = "api.cosm.com";

long previousUploadTime = 0;        // will store last time info was uploaded to cosm
long interval = 100;           // interval at which to upload (milliseconds)

char feedId[] = "104810";
char apiKey[] = "YOUR API KEY GOES HERE"


int counter=0;

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(115200);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    while(true);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");

  // if you get a connection, report back via serial:
  
  if (client.connect(serverName, 8081)) {
    Serial.println("connected");
  } 
  else {
    // kf you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}

void loop()
{
  
  unsigned long currentMillis = millis();
 
  if(currentMillis - previousUploadTime > interval) {
    client.print("{\"method\" : \"put\",");
    client.print("\"resource\" : \"/feeds/");
    client.print(feedId);
    client.print("\", \"headers\" :{\"X-ApiKey\" : \"");
    client.print(apiKey);
    client.print("\"},\"body\" :{ \"version\" : \"1.0.0\",\"datastreams\" : [{\"id\" : \"0\",\"current_value\" : \"");
    client.print(counter);
    client.print("\"}]}}");
    client.println();
    previousUploadTime = millis();
    counter++;
  }
  // if there are incoming bytes available 
  // from the server, read them and print them:
  if (client.available()) {
    //Serial.println(client.parseInt());
    char c = client.read();
    Serial.print(c);
    //client.flush();
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();

    // do nothing forevermore:
    while(true);
  }
}

