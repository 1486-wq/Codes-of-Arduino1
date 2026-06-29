#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <DHT.h>
//
int smoke=A0;
// =========================
// FIRE SENSOR
// =========================
int firePin = 3;
int buzzer=4;

// =========================
// RFID
// =========================
MFRC522 rfid(10, 9);

byte tag[4]  = {0x2B, 0x8C, 0x2C, 0x07};
byte card[4] = {0x2B, 0x98, 0x10, 0x07};

int doorRelay = 8;

// =========================
// RAIN SYSTEM
// =========================
Servo rainServo;

int rainSensor = A2;
int servoPin = 2;
int rainThreshold = 500;

// =========================
// DHT SYSTEM
// =========================
#define DHTPIN 5
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

int button = 7;
int climateRelay = 6;

int mode = 0; // 0 = SUMMER, 1 = WINTER
int lastButtonState = HIGH;

// =========================
// SETUP
// =========================
void setup() {

  Serial.begin(9600);

  // Fire
  pinMode(firePin, INPUT_PULLUP);

  // RFID
  SPI.begin();
  rfid.PCD_Init();

  // Door relay
  pinMode(doorRelay, OUTPUT);
  digitalWrite(doorRelay, HIGH);

  // Rain servo
  rainServo.attach(servoPin);

  // DHT
  dht.begin();

  pinMode(button, INPUT_PULLUP);

  pinMode(climateRelay, OUTPUT);
  digitalWrite(climateRelay, HIGH);
  pinMode(buzzer, OUTPUT);

  Serial.println("SMART SYSTEM STARTED");
}

// =========================
// LOOP
// =========================
void loop() {
   digitalWrite(buzzer, LOW);

  // =========================
  // FIRE SENSOR
  // =========================
  int fireValue = digitalRead(firePin);

  if (fireValue == LOW) {
    Serial.println("🔥 FIRE DETECTED");
    digitalWrite(buzzer, HIGH);
  } else {
    Serial.println("No Fire");
    digitalWrite(buzzer, LOW);
  }
//
  int value = analogRead(smoke);

  Serial.print("Smoke value: ");
  Serial.println(value);

  if (value < 50) {
    Serial.println("✅ Clean air");
    digitalWrite(buzzer, LOW);
  } 
  else if (value >= 185 && value < 600) {
    Serial.println("⚠️ Smoke detected");
    digitalWrite(buzzer, HIGH);
  } 
  /*else {
    Serial.println("🚨 High smoke / DANGER!");
    digitalWrite(buzzer, HIGH);
  }
*/
  delay(2000);


//
  // =========================
  // RAIN SENSOR
  // =========================
  int rainValue = analogRead(rainSensor);

  Serial.print("Rain: ");
  Serial.println(rainValue);

  if (rainValue < rainThreshold) {
    Serial.println("RAIN -> IN");
    rainServo.write(0);
  } else {
    Serial.println("NO RAIN -> OUT");
    rainServo.write(90);
  }

  // =========================
  // DHT22 + MODE SWITCH
  // =========================
  float temp = dht.readTemperature();

  if (!isnan(temp)) {

    Serial.print("Temp: ");
    Serial.println(temp);

    int buttonState = digitalRead(button);

    if (lastButtonState == HIGH && buttonState == LOW) {
      mode = 1 - mode;
    }

    lastButtonState = buttonState;

    if (mode == 0) {
      Serial.println("Mode: SUMMER");

      if (temp > 26) {
        digitalWrite(climateRelay, LOW);
        Serial.println("AC ON");
      } else {
        digitalWrite(climateRelay, LOW);//means on
      }

    } else {
      Serial.println("Mode: WINTER");

      if (temp < 20) {
        digitalWrite(climateRelay, HIGH);//MEANS ON
        Serial.println("HEATER ON");
      } else {
        digitalWrite(climateRelay, HIGH);
      }
    }
  }

  // =========================
  // RFID (FIXED - NO RETURN)
  // =========================
  if (rfid.PICC_IsNewCardPresent() &&
      rfid.PICC_ReadCardSerial()) {

    Serial.println("Card Detected");

    // Print UID (VERY IMPORTANT FOR DEBUG)
    Serial.print("UID: ");
    for (byte i = 0; i < rfid.uid.size; i++) {
      Serial.print(rfid.uid.uidByte[i], HEX);
      Serial.print(" ");
    }
    Serial.println();

    bool access = false;

    // TAG CHECK
    if (rfid.uid.uidByte[0] == tag[0] &&
        rfid.uid.uidByte[1] == tag[1] &&
        rfid.uid.uidByte[2] == tag[2] &&
        rfid.uid.uidByte[3] == tag[3]) {
      access = true;
    }

    // CARD CHECK
    if (rfid.uid.uidByte[0] == card[0] &&
        rfid.uid.uidByte[1] == card[1] &&
        rfid.uid.uidByte[2] == card[2] &&
        rfid.uid.uidByte[3] == card[3]) {
      access = true;
    }

    if (access) {

      Serial.println("ACCESS GRANTED");
      digitalWrite(doorRelay, LOW);
      delay(3000);
      digitalWrite(doorRelay, HIGH);

      Serial.println("DOOR CLOSED");

    } else {
      Serial.println("ILLEGAL CARD");
    }

    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }

  delay(2000);
}