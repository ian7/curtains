#include <MQTTClient.h>
#include <WiFi.h>
#include "credentials.h"

WiFiClient net;
MQTTClient client;

String message("world2");

unsigned long lastMillis = 0;

void setup() {
  WiFi.begin(ssid, pass);

    pinMode(23, OUTPUT);      // set the LED pin mode
    pinMode(22, OUTPUT);      // set the LED pin mode
    pinMode(21, OUTPUT);      // set the LED pin mode
    pinMode(19, OUTPUT);      // set the LED pin mode

  // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported by Arduino.
  // You need to set the IP address directly.
  client.begin("10.10.1.3", net);
  client.onMessage(messageReceived);

  connect();
}

void connect() {
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  while (!client.connect("arduino", "try", "try")) {
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe("/hello");
  client.subscribe("/on");
  client.subscribe("/off");
  client.subscribe("/message");
  // client.unsubscribe("/hello");
}

void loop() {
  client.loop();

  if (!client.connected()) {
    connect();
  }

  // publish a message roughly every second.
  if (millis() - lastMillis > 1000) {
    lastMillis = millis();
    client.publish("/hello", message);
  }
}

void messageReceived(String &topic, String &payload) {
  //Serial.println("incoming: " + topic + " - " + payload);
  char topicChar[1024];
  topic.toCharArray(topicChar,256);
  int payloadInt = payload.toInt();
  String son = String("/on");
  String soff = String("/off");
  String smessage = String("/message");

  if( topic == smessage ){
    message = payload;
  }
  if( topic == son ){
    switch( payloadInt ){
      case 1: 
        digitalWrite(23, HIGH); 
        break;
      case 2: 
        digitalWrite(22, HIGH); 
        break;
      case 3: 
        digitalWrite(21, HIGH); 
        break;
      case 4: 
        digitalWrite(19, HIGH); 
        break;      
    }
  }
  if( topic == soff ){
    switch( payloadInt ){
      case 1: 
        digitalWrite(23, LOW); 
        break;
      case 2: 
        digitalWrite(22, LOW); 
        break;
      case 3: 
        digitalWrite(21, LOW); 
        break;
      case 4: 
        digitalWrite(19, LOW); 
        break;      
    }
    
  }
//  sleep(1000);
//  digitalWrite(23, LOW); 
}
