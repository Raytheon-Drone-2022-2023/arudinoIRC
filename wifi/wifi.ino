/*

 This example connects to an unencrypted Wifi network.

 Then it prints the  MAC address of the Wifi module,

 the IP address obtained, and other network details.

 created 13 July 2010

 by dlf (Metodo2 srl)

 modified 31 May 2012

 by Tom Igoe

 */
#include <SPI.h>
#include <WiFiNINA.h>
#include <IRCClient.h>

#define IRC_SERVER   "chat.freenode.net"
#define IRC_PORT     6667
#define IRC_NICKNAME "SMUEvader2"
#define IRC_USER     "SMUEvader2"

WiFiClient wiFiClient;
IRCClient client(IRC_SERVER, IRC_PORT, wiFiClient);
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = "SamPhone";        // your network SSID (name)
char password[] = "$wagmoney";    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;     // the Wifi radio's status
String ircChannel = "#RTXDrone";
bool readyToJoin = false;
int loopCount = 0;

void setup() {
  Serial.begin(115200);

  delay(100);

  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  client.setCallback(callback);
  client.setSentCallback(debugSentCallback);
}

void loop() {
  if (!client.connected()) {
    Serial.println("Attempting IRC connection...");
    // Attempt to connect
    if (client.connect(IRC_NICKNAME, IRC_USER)) {
      Serial.println("connected");
      readyToJoin = true;
    } else {
      Serial.println("failed... try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
    return;
  } 

  if(readyToJoin && client.loop()) {
    Serial.println("tried to join");
    client.sendRaw("JOIN " + ircChannel);
    client.sendMessage(ircChannel, "SMUEvader Test");
  }


  client.loop();
}

void callback(IRCMessage ircMessage) {
  // PRIVMSG ignoring CTCP messages
  if (ircMessage.command == "PRIVMSG" && ircMessage.text[0] != '\001') {
    String message("<" + ircMessage.nick + "> " + ircMessage.text);
    Serial.println(message);
    return;
  }
  Serial.println(ircMessage.original);
}

void debugSentCallback(String data) {
  Serial.println(data);
}