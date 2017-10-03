#define FASTLED_ALLOW_INTERRUPTS 0
#include "FastLED.h"
#include <MQTTClient.h>
#include <WiFi.h>

WiFiClient net;
MQTTClient client;


// How many leds in your strip?
#define NUM_LEDS 150

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 22
#define CLOCK_PIN 13


int red = 10;
int green = 10;
int blue = 10;

int hue = 10;
int saturation = 10;
int brightness = 10;

int oldRed = red;
int oldGreen = green;
int oldBlue = blue;


const char ssid[] = "HTM-IoT";
const char pass[] = "tofatofatofa";

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() { 
  LEDS.addLeds<WS2812B,DATA_PIN,RGB>(leds,NUM_LEDS,0);
  LEDS.setBrightness(250);
        WiFi.begin(ssid, pass);

        // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported by Arduino.
        // You need to set the IP address directly.
        client.begin("10.10.1.3", net);
        client.onMessage(messageReceived);
      connect(); 

}

void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } }

void connect() {
        while (WiFi.status() != WL_CONNECTED) {
                delay(1000);
        }

        while (!client.connect("light")) {
                delay(1000);
        }
        client.subscribe("/light/red");
        client.subscribe("/light/green");
        client.subscribe("/light/blue");
        client.subscribe("/light/hue");
        client.subscribe("/light/saturation");
        client.subscribe("/light/brightness");

}

void loop() { 
  
  while( true ){

        client.loop();

        if (!client.connected()) {
                connect();
        }

    
  // First slide the led in one direction
  for(int i = 0; i < NUM_LEDS; i++) {
    // Set the i'th led to red 
    leds[i] = CHSV(hue, saturation, brightness);
    // Show the leds
    }
    FastLED.show(); 
    delay(5);
  }
}


void messageReceived(String &topic, String &payload) {

        if( topic == String("/light/hue") ) {
                if( payload == String("+") ){
                  hue++;
                }
                else if( payload == String("-") ){
                  hue--;
                }
                else{
                  hue = payload.toInt();
                }
                client.publish("/light/ack","h: "+String(hue) + " s: " + String(saturation) + " b: " + String(brightness));
        }
        if( topic == String("/light/saturation") ) {
                if( payload == String("+") ){
                  saturation++;
                }
                else if( payload == String("-") ){
                  saturation--;
                }
                else{
                  saturation = payload.toInt();
                }
                client.publish("/light/ack","h: "+String(hue) + " s: " + String(saturation) + " b: " + String(brightness));
        }
        if( topic == String("/light/brightness") ) {
                if( payload == String("+") ){
                  brightness++;
                }
                else if( payload == String("-") ){
                  brightness--;
                }
                else{
                  brightness = payload.toInt();
                }

                client.publish("/light/ack","h: "+String(hue) + " s: " + String(saturation) + " b: " + String(brightness));
        }

        if( topic == String("/light/red") ) {
                red = payload.toInt();
                client.publish("/light/ack","red "+payload);
        }
        if( topic == String("/light/green") ) {
                red = payload.toInt();
                client.publish("/light/ack","green "+payload);
        }
        if( topic == String("/light/blue") ) {
                red = payload.toInt();
                client.publish("/light/ack","blue "+payload);
        }
        //show();
}

