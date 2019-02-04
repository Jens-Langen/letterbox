/** The MIT License (MIT)

Copyright (c) 2019 Jens Langen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "settings.h"


WiFiClient espClient;
PubSubClient client(espClient);

String baseTopic = "LetterBox/";
String strTopicIP =  baseTopic + "INFO/IPADDRESS";
String strTopicTop =  baseTopic + "top_isOpen";
String strTopicFront =  baseTopic + "front_isOpen";
String strTopicBattery =  baseTopic + "battery";
  
String aliveTopic = baseTopic + "alive";

String strIP = "";
String clientId = "LetterBox";

bool stateTop = false;
bool stateFront = false;
bool lastStateTop = false;
bool lastStateFront = false;

#define LEDPIN LED_BUILTIN //  D8
#define top D7
#define front D6
#define LED D8

int scanDelay = 1000;

int batterySensor = A0;
bool batterySend = false;

unsigned long previousMillis = 0;

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
      clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pwd )) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      IPAddress localAddr = WiFi.localIP();
      byte oct1 = localAddr[0];
      byte oct2 = localAddr[1];
      byte oct3 = localAddr[2];
      byte oct4 = localAddr[3];
      char s[16];  
      sprintf(s, "%d.%d.%d.%d", oct1, oct2, oct3, oct4);
      
      client.publish(strTopicIP.c_str(), s ); 

      yield();

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Start program");
  
  pinMode(LEDPIN,OUTPUT);
  digitalWrite(LEDPIN,false);
  
  pinMode(top, INPUT); 
  pinMode(front, INPUT);

  pinMode(LED,OUTPUT);
  digitalWrite(LED,false);
  stateTop = digitalRead(top);
  stateFront = digitalRead(front);
  
  Serial.print("stateFront: ");
  Serial.println(stateFront);
  digitalWrite(LED,stateFront);

  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    yield();
  }

  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Subnet Mask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway IP: ");
  Serial.println(WiFi.gatewayIP());
  
  client.setServer(mqtt_server, mqtt_port);


}

void loop() {
   
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  

  if (millis() - previousMillis > 1000) {
    previousMillis = millis();
    stateTop = !digitalRead(top);
    stateFront = !digitalRead(front);


  
    unsigned int raw=0;
    float volt=0.0;
    
    if ( batterySend == false ) {
      raw = analogRead(batterySensor);
      volt = raw / 1023.0;
      String battery = String(volt*4.2);
      Serial.print("Battery:");
      Serial.println(battery);

      client.publish(strTopicBattery.c_str(), battery.c_str() );
      batterySend = true;
    }

    Serial.print("FRONT: ");
    Serial.println(stateFront);
    Serial.print("TOP  : ");
    Serial.println(stateTop);

    if ( stateTop != lastStateTop) {
      client.publish(strTopicTop.c_str(), ( stateTop ? "1":"0" ) );
      lastStateTop = stateTop;
    }
    
    if ( stateFront != lastStateFront) {
      client.publish(strTopicFront.c_str(), ( stateFront ? "1":"0" ) );
      lastStateFront = stateFront;
    }
    
    digitalWrite(LED,stateFront);
        
    if (stateFront == false && stateTop == false) {
      delay(2000);
      Serial.println("Sleep");
      system_deep_sleep(4294967295);
    }

    yield();
  }  
 
 
}




