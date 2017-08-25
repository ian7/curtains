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
int encoderPosCount = 0;
int pinALast;
int aVal;
int edgeSensor;
int lastEdgeSensor;
boolean bCW;

const int songLength = 18;

// Notes is an array of text characters corresponding to the notes
// in your song. A space represents a rest (no tone)

char notes[] = "cdfda ag cdfdg gf "; // a space represents a rest

// Beats is an array of values for each note and rest.
// A "1" represents a quarter-note, 2 a half-note, etc.
// Don't forget that the rests (spaces) need a length as well.

int beats[] = {1,1,1,1,1,1,4,4,2,1,1,1,1,1,1,4,4,2};

// The tempo is how fast to play the song.
// To make the song play faster, decrease this value.

int tempo = 113;


String message("world3");

unsigned long lastMillis = 0;

void setup() {
  WiFi.begin(ssid, pass);

  // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported by Arduino.
  // You need to set the IP address directly.
  client.begin("10.10.1.3", net);
  client.onMessage(messageReceived);
    pinMode(23, OUTPUT);      // set the LED pin mode
    pinMode(22, OUTPUT);      // set the LED pin mode
    pinMode(21, OUTPUT);      // set the LED pin mode
    pinMode(19, OUTPUT);      // set the LED pin mode
    pinMode(32, OUTPUT);
  connect();  
}


void connect() {
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  while (!client.connect("rotation")) {
    delay(1000);
  }


//  client.subscribe("/hello");
  client.subscribe("/on");
  client.subscribe("/off");
  client.subscribe("/message");

    dht.begin();
  // client.unsubscribe("/hello");
}

bool b=false;

void loop() {
  client.loop();

  if (!client.connected()) {
    connect();
  }

  // publish a message roughly every second.
  if (millis() - lastMillis > 2000) {
      lastMillis = millis();

     float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

    client.publish("/hello", String("t: ") + String(t) + String(" h: ") + String(h));
  }

  aVal = digitalRead(pinA);

  if (aVal != pinALast){ // Means the knob is rotating
    // if the knob is rotating, we need to determine direction
    // We do that by reading pin B.
    if (digitalRead(pinB) != aVal) { // Means pin A Changed first ­ We're Rotating Clockwise
      encoderPosCount ++;
      bCW = true;
    } else {// Otherwise B changed first and we're moving CCW
      bCW = false;
      encoderPosCount--;
      }
    if (bCW){
        client.publish("/rotated", "clockwise");
    }else{
        client.publish("/rotated", "counterclockwise");
    }
    client.publish("/position", String(encoderPosCount));
  }
  pinALast = aVal;


  edgeSensor = digitalRead(16);
  if( edgeSensor != lastEdgeSensor ){
    lastEdgeSensor = edgeSensor;
    if( edgeSensor == 0 ) {
      client.publish("/position", "open");    
    }
    else{
      client.publish("/position", "closed");          
    }
  }
}

void messageReceived(String &topic, String &payload) {
   digitalWrite(32, HIGH); 
   delay(5);
   digitalWrite(32,LOW);
 
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

}


