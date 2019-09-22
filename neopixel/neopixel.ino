#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        5 

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 8 

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

const char* SSID = "Forum";
const char* PASSWORD = "Hack2019";
const char* MQTT_BROKER = "172.16.0.1";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
int farbe = 4;

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("espClient")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("ampel","nummer1 lebt");
      // ... and resubscribe
      client.subscribe("smike/#");
      if(client.subscribe("test")){
        Serial.println("sub erfolgreich");
      }
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  if((String((char)payload[0]))== "3"){
    farbe = 1;
    Serial.println("rot angekommen");
  }
  if((String((char)payload[0]))== "4"){
    farbe = 2;
    Serial.println("gelb angekommen");
  }
  else if((String((char)payload[0]))== "5"){
    farbe = 3;
    Serial.print("gruen angekommen");
  }
}


void setup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  Serial.begin(115200);
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.setBrightness(25);
  
  Serial.println("Starte Wlan");
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  client.setServer(MQTT_BROKER, 1883);
  client.setCallback(callback);
  delay(500);
 
  reconnect();
  
  Serial.println("Setup beendet");
}

void loop() {
  pixels.clear(); // Set all pixel colors to 'off'
  Serial.println("loop");
  for(int i=0; i<NUMPIXELS; i++) {
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    if(farbe == 4){
      pixels.setPixelColor(i, pixels.Color(0, 0, 255));
      Serial.println("blau");
    }
    else if(farbe == 3){
      pixels.setPixelColor(i, pixels.Color(0, 150, 0));
      Serial.println("gruen");
    }
    else if (farbe == 2){
      pixels.setPixelColor(i, pixels.Color(255, 128, 0));
      Serial.println("gelb");
    }
    else if (farbe == 1){
      pixels.setPixelColor(i, pixels.Color(255, 0, 0));
      Serial.println("rot");
    }
    Serial.println("zeige Farbe");
    client.loop();
    pixels.show();   // Send the updated pixel colors to the hardware.

    delay(DELAYVAL); // Pause before next pass through loop
  }
}
