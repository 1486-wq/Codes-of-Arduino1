// Elevator buttons and LED
const int btn1 = 2;   // Floor 1 button
const int btn2 = 3;   // Floor 2 button
const int led  = 8;   // Elevator status LED

int currentFloor = 1;

// LDR and Relay
const int relay  = 4;
const int sensor = A0;
int sensorvalue = 0;
//dete
int dete=5;
int led1=9;
int led2=10;
//dete2
int dete2=11;
int led3=6;
int led4=7;


void setup() {
  //detec
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(dete, INPUT);//DETECTION
  //
    pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(dete2, INPUT);//2DETECTION

  // Elevator pins
  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);
  pinMode(led, OUTPUT);

  // LDR relay pin
  pinMode(relay, OUTPUT);

  // Elevator starts at Floor 1
  digitalWrite(led, HIGH);

  Serial.begin(9600);
}

void loop() {

  // ==========================
  // Elevator Control
  // ==========================

  // GO UP
  if (digitalRead(btn2) == LOW && currentFloor == 1) {

    Serial.println("Going Up");
    digitalWrite(led, LOW);   // Moving

    delay(4000);              // Travel time

    currentFloor = 2;

    digitalWrite(led, HIGH);  // Arrived
    Serial.println("Arrived Floor 2");
  }

  // GO DOWN
  if (digitalRead(btn1) == LOW && currentFloor == 2) {

    Serial.println("Going Down");
    digitalWrite(led, LOW);   // Moving

    delay(4000);              // Travel time

    currentFloor = 1;

    digitalWrite(led, HIGH);  // Arrived
    Serial.println("Arrived Floor 1");
  }

  // ==========================
  // LDR Light Control
  // ==========================

  sensorvalue = analogRead(sensor);

  Serial.print("Sensor: ");
  Serial.println(sensorvalue);

  if (sensorvalue > 250) {
    digitalWrite(relay, LOW);   // Relay ON
    Serial.println("Turn ON the LEDs");
  }
  else {
    digitalWrite(relay, HIGH);  // Relay OFF
    Serial.println("Turn OFF the LEDs");
  }

  delay(500);

//****DETECTION
if (dete>0){
    digitalWrite(led1, HIGH);
     digitalWrite(led2, HIGH);
    Serial.println("piso2 lights on");
    delay(1000);
  }
  else{
    digitalWrite(led1, LOW);
     digitalWrite(led2, LOW);
    Serial.println("lights off piso2");
  }
    delay(1000);

//****end DETECTION
//2****DETECTION
if (dete>0){
    digitalWrite(led3, HIGH);
     digitalWrite(led4, HIGH);
    Serial.println("piso3 on");
    delay(1000);
  }
  else{
    digitalWrite(led1, LOW);
     digitalWrite(led2, LOW);
    Serial.println("piso3 off");
  }
//****end DETECTION
  delay(1000);



}