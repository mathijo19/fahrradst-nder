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


void setup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  Serial.begin(115200);
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.setBrightness(25);
  Serial.println("Setup beendet");
}

void loop() {
  pixels.clear(); // Set all pixel colors to 'off'
  Serial.println("loop");
  for(int i=0; i<NUMPIXELS; i++) {
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels.setPixelColor(i, pixels.Color(0, 150, 0));
    Serial.println("zeige Farbe");
    pixels.show();   // Send the updated pixel colors to the hardware.

    delay(DELAYVAL); // Pause before next pass through loop
  }
}
