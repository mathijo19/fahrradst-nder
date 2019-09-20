
const int taster =  5; //pin D1
const int led_rot= 4; //pin D2
const int led_gruen = 14; //pin D5 
const int led_blau = 15; //pin D8
int tasterstatus = 0; 

void setup() {
  pinMode(taster, INPUT_PULLUP);
  Serial.begin(115200);
  Serial.println("Programm gestartet");
  pinMode(led_rot, OUTPUT);
  pinMode(led_gruen, OUTPUT);
  pinMode(led_blau, OUTPUT);
}
void loop () {
  digitalWrite(led_blau, LOW);
  tasterstatus = digitalRead(taster); 
  if (tasterstatus == HIGH) {
    Serial.println("Kein Fahrrad da");
    digitalWrite(led_rot, LOW);
    digitalWrite(led_gruen, HIGH);
  }
  else {
    Serial.println("Fahrrad drin");
    digitalWrite(led_gruen, LOW);
    digitalWrite(led_rot, HIGH);
  }
  delay(1000);
}
