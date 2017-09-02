#include <Update.h>
#include <DHT.h>
#include <MQTTClient.h>
#include <WiFi.h>
#include "credentials.h"

#define DHTTYPE DHT11   // DHT 11
#define DHTPIN 17

WiFiClient net;
MQTTClient client;

DHT dht(DHTPIN, DHTTYPE);

int pinA = 18; // Connected to CLK on KY­040
int pinB = 5; // Connected to DT on KY­040
int curtainsPosition = 0;
int lastCurtainsPosition = 10000;
int pinALast;
int aVal;
int edgeSensor;
int lastEdgeSensor;

boolean isMoving = false;
boolean isClosing = false;

String message("world3");

unsigned long lastMillis = 0;

void setup() {
        WiFi.begin(ssid, pass);

        // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported by Arduino.
        // You need to set the IP address directly.
        client.begin("10.10.1.3", net);
        client.onMessage(messageReceived);
        pinMode(23, OUTPUT);  // set the LED pin mode
        pinMode(22, OUTPUT);  // set the LED pin mode
        pinMode(21, OUTPUT);  // set the LED pin mode
        pinMode(19, OUTPUT);  // set the LED pin mode
        pinMode(32, OUTPUT);
        connect();
}


void connect() {
        while (WiFi.status() != WL_CONNECTED) {
                delay(1000);
        }

        while (!client.connect("curtains")) {
                delay(1000);
        }

        client.subscribe("/curtains/command");
        dht.begin();
}

bool b=false;

void loop() {
        client.loop();

        if (!client.connected()) {
                connect();
        }

        // publish a message roughly every second.
        if (millis() - lastMillis > 1000) {
                lastMillis = millis();
                float h = dht.readHumidity();
                // Read temperature as Celsius (the default)
                float t = dht.readTemperature();
                // Read temperature as Fahrenheit (isFahrenheit = true)
                float f = dht.readTemperature(true);

                //client.publish("/curtains/air", String("t: ") + String(t) + String(" h: ") + String(h));

                // if within the last second it didn't move then stop
                if( isMoving && abs(lastCurtainsPosition - curtainsPosition) < 5 ){
                  motorStop();
                }
                else{
                  lastCurtainsPosition = curtainsPosition;
                }

        }

        aVal = digitalRead(pinA);

        if (aVal != pinALast) {
                if (digitalRead(pinB) != aVal) { // Means pin A Changed first ­ We're Rotating Clockwise
                        curtainsPosition++;
                } else {// Otherwise B changed first and we're moving CCW
                        curtainsPosition--;
                }
                client.publish("/curtains/position", String(curtainsPosition));
        }
        pinALast = aVal;

        edgeSensor = digitalRead(16);
        if( edgeSensor != lastEdgeSensor ) {
                lastEdgeSensor = edgeSensor;
                if( edgeSensor == 0 && isClosing == false ) {
                        // reached edge
                        client.publish("/curtains/position", "open");
                        motorStop();
                        curtainsPosition = 0;
                }
                else{
                        client.publish("/position", "closed");
                }
        }
}
/*
   Just beeps once
 */

bool isAtEdge(){
  return( edgeSensor == 0);
}

void beep() {
        digitalWrite(32, HIGH);
        delay(5);
        digitalWrite(32,LOW);
}

void motorStop(){
  digitalWrite(23, LOW);
  digitalWrite(22, LOW);
  isMoving = false;
}

void motorClose(){
  digitalWrite(23, LOW);
  digitalWrite(22, HIGH);
  isMoving = true;
  isClosing = true;
}

void motorOpen(){
  if( isAtEdge() ){
    // just complain with a beep
    beep();
    delay(500);
    beep();
  }
  else {
    digitalWrite(23, HIGH);
    digitalWrite(22, LOW);
    isMoving = true;
    isClosing = false;
  }
}

void motorSlow(){
  digitalWrite(21, LOW);
  digitalWrite(19, LOW);
}

void motorFast(){
  digitalWrite(21, LOW);
  digitalWrite(19, HIGH);
}

void messageReceived(String &topic, String &payload) {
        beep();

        if( payload == String("open") ){
          motorOpen();
        }

        if( payload == String("close") ){
          motorClose();
        }

        if( payload == String("stop") ){
          motorStop();
        }

        if( payload == String("fast") ){
          motorFast();
        }
        if( payload == String("slow") ){
          motorSlow();
        }
}
