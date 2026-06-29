#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

int dete=6;
int deterelay=7;
//detec
int soilPin = A0;
int ledPin = 3;
int dhtPin = 2;
int relay=4;
//soil
int laser=5;
const int ldrPin = A1;
int ldrvalue=0;
int buzzer =8;
//securityy
DHT dht(dhtPin, DHT11);
LiquidCrystal_I2C lcd(0x27, 16, 2);

int soilValue;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(relay, OUTPUT);
  pinMode(laser, OUTPUT);
  pinMode(dete, INPUT);//DETECTION
  pinMode(buzzer, OUTPUT);
  pinMode(deterelay, OUTPUT);
  Serial.begin(9600);

  dht.begin();
  lcd.init();
  lcd.backlight();
}

void loop() {
  
  digitalWrite(laser, HIGH);
 

//****DETECTION
/*
int detevalue = digitalRead(dete);  
  Serial.print("detection: ");
  Serial.println(detevalue);
  delay(500);
if (detevalue>0){
    digitalWrite(deterelay, LOW);
    Serial.println("turn on the leds");
    delay(1000);
  }
  else{
    digitalWrite(deterelay, HIGH);//OPPOSITE
  }
//****end DETECTION
*/

//******security
  ldrvalue = analogRead(ldrPin);
  Serial.print("ldrsensor: ");
  Serial.println(ldrvalue);
  if (ldrvalue>750) {
  Serial.println("buzzerhigh");
  digitalWrite(buzzer, HIGH); 
  delay(100);                      
  }
  else{
      digitalWrite(buzzer, LOW); 

  } 

//*******END SECURITY 


  soilValue = analogRead(soilPin);
  Serial.print("soilvalue :");
  Serial.println(soilValue );
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  lcd.clear();

  // First line
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(t);
  lcd.print(" H:");
  lcd.print(h);

  // Second line
  lcd.setCursor(0, 1);
//***** SOIL SYSYTEMS 
  if (soilValue <200) {
    lcd.print("Dry - Water!");
    digitalWrite(ledPin, HIGH);
    digitalWrite(relay, HIGH);//(0) )HERE IS HIGH BUT UTS WORKS LOW BECAUSE ITS WORKS INVERSE SO HERE ISTS LOW(0) 
    
  } else {
    lcd.print("Soil OK");
    digitalWrite(ledPin, LOW);
    digitalWrite(relay, LOW);//(1)HERE ITS WOKS HIGH BECAUSE THE IN OTHER FUNCTIONS IS LOW WHUCH MENAS ITS WORKS HERE HIGH(1)
    delay(1000);
  }

  delay(2000);
  //***ENDSOIL SYSYTEM
}