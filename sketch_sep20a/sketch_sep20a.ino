
#include <Arduino.h>

#include <ESP8266WiFi.h>




#include <PubSubClient.h>



const int taster =  5; //pin D1
const int led_rot= 4; //pin D2
const int led_gruen = 14; //pin D5 
const int led_blau = 15; //pin D8
int tasterstatus = 0; //Variable für tastererkennung
int zustand = 0; //Variable für den zustand des wlans
const char* SSID = "Forum";
const char* PASSWORD = "Hack2019";
const char* MQTT_BROKER = "172.16.0.1";
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

//############################################################################

void setup() {
  pinMode(taster, INPUT_PULLUP);
  Serial.begin(115200);
  Serial.println("Programm gestartet");
  pinMode(led_rot, OUTPUT);
  pinMode(led_gruen, OUTPUT);
  pinMode(led_blau, OUTPUT);
  //WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  //PubSubClient client(WiFi);
  client.setServer(MQTT_BROKER, 1883);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("Rad1", "verbunden");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
  }

void loop () {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  delay(1000);
  digitalWrite(led_blau, LOW);
  tasterstatus = digitalRead(taster); 
  if (tasterstatus == HIGH) {
    Serial.println("Kein Fahrrad da");
    digitalWrite(led_rot, LOW);
    digitalWrite(led_gruen, HIGH);
    snprintf (msg, 50, "Rad1_frei", tasterstatus);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("Fahrradstaender/1", msg);    
  }
  else {
    Serial.println("Fahrrad drin");
    digitalWrite(led_gruen, LOW);
    digitalWrite(led_rot, HIGH);
    //if (zustand == 0) {
      //Serial.println("Wlan wird aktiviert");
       //if ((WiFi.run() == WL_CONNECTED)) {
          //WiFiClient client; //hier das Paket verschicken!
         // Serial.println("Wlan aktiv");
          //zustand = 1;
       //}
     
    //}
    snprintf (msg, 50, "Rad1_belegt", tasterstatus);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("Fahrradstaender/1", msg);
  }
}
