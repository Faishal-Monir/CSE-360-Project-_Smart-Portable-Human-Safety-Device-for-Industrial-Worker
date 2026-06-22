#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <MFRC522.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
#define DHTPIN 8
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const int MQ2Pin = A0;
const int MQ135Pin = A1;
const int pulsePin = A2;
const int buzzerPin = 6;
const int buttonPin = 7;
const int threshold = 200;
const int pulseThreshold = 520;

unsigned long lastBeat = 0;
int bpm = 0;
unsigned long lastPulseUpdate = 0; 

SoftwareSerial BTSerial(2, 3);  

float lastTemp = -1;

Adafruit_MPU6050 mpu;
const float movementThreshold = 15.0;  

bool dangerDetected = false;
unsigned long lastDisplaySwitch = 0;
bool displayToggle = false;  


#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN);


bool accessGranted = false;

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
  lcd.init();
  lcd.backlight();
  dht.begin();
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  SPI.begin();
  rfid.PCD_Init();

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) delay(10);
  }
  Serial.println("MPU6050 Found!");
  delay(1000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CSE 360 Safety");
  lcd.setCursor(0, 1);
  lcd.print("Vest Project");
  delay(3000);
  Serial.println("Ready");


  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Scan RFID card");
  lcd.setCursor(0, 1);
  lcd.print("for Access");
}

void loop() {
  if (!accessGranted) {
    // Wait for RFID card and check authentication
    if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
      // Format UID string
      String readUID = "";
      for (byte i = 0; i < rfid.uid.size; i++) {
        if (rfid.uid.uidByte[i] < 0x10) readUID += "0";
        readUID += String(rfid.uid.uidByte[i], HEX);
        if (i != rfid.uid.size - 1) readUID += ":";
      }
      readUID.toUpperCase();

      // Compare to allowed UID "DC:EE:10:05" (case insensitive)
      if (readUID == "DC:EE:10:05") {
        accessGranted = true;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Access Granted");
        // Buzzer two beeps
        buzzerBeep(2, 200);
        delay(2000);
        lcd.clear();
      } else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Access Denied");
        lcd.setCursor(0, 1);
        lcd.print("Scan Again");
        buzzerContinuous(3000);
        delay(2000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Scan RFID card");
        lcd.setCursor(0, 1);
        lcd.print("for Access");
      }
      rfid.PICC_HaltA();
    }
    return; 
  }

 
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    String readUID = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
      if (rfid.uid.uidByte[i] < 0x10) readUID += "0";
      readUID += String(rfid.uid.uidByte[i], HEX);
      if (i != rfid.uid.size - 1) readUID += ":";
    }
    readUID.toUpperCase();
    if (readUID == "DC:EE:10:05") {
      accessGranted = false;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Scan RFID card");
      lcd.setCursor(0, 1);
      lcd.print("for Access");
      buzzerBeep(1, 300);
      rfid.PICC_HaltA();
      delay(2000);
      return; // start authentication again
    }
    rfid.PICC_HaltA();
  }



  bool buttonPressed = (digitalRead(buttonPin) == LOW);
  int mq2Value = analogRead(MQ2Pin);
  int mq135Value = analogRead(MQ135Pin);
  float temp = dht.readTemperature();
  int retryCount = 0;
  while (isnan(temp) && retryCount < 3) {
    delay(200);
    temp = dht.readTemperature();
    retryCount++;
  }
  if (!isnan(temp)) lastTemp = temp;
  bool sensorAlarm = (mq2Value > threshold) || (mq135Value > threshold);
  int pulseValue = analogRead(pulsePin);
  if (millis() - lastPulseUpdate > 2000) {
    if (pulseValue > pulseThreshold && (millis() - lastBeat) > 200) {
      unsigned long now = millis();
      if (lastBeat > 0) {
        float ibi = now - lastBeat;
        bpm = 60000.0 / ibi;
      }
      lastBeat = now;
    }
    lastPulseUpdate = millis();
  }
  sensors_event_t a, g, tempSensor;
  mpu.getEvent(&a, &g, &tempSensor);
  float accelMag = sqrt(a.acceleration.x * a.acceleration.x +
                        a.acceleration.y * a.acceleration.y +
                        a.acceleration.z * a.acceleration.z);
  if (accelMag > movementThreshold) {
    dangerDetected = true;
  } else {
    dangerDetected = false;
  }
  lcd.clear();
  if (dangerDetected) {
    lcd.setCursor(5, 0);
    lcd.print("DANGER!!!");
    digitalWrite(buzzerPin, HIGH);
    Serial.println("DANGER");
    BTSerial.println("DANGER");
  } else if (buttonPressed) {
    buzzerOnForDuration(10000);
    lcd.setCursor(0, 0);
    lcd.print("Safety Button");
    lcd.setCursor(0, 1);
    lcd.print("ALARM ON!");
    BTSerial.println("ALARM");
    Serial.println("ALARM");
    BTSerial.flush();
    delay(50);
  } else {
    digitalWrite(buzzerPin, sensorAlarm ? HIGH : LOW);
    if (millis() - lastDisplaySwitch > 3000) {
      displayToggle = !displayToggle;
      lastDisplaySwitch = millis();
    }
    if (!displayToggle) {
      lcd.setCursor(0, 0);
      lcd.print("MQ2:");
      lcd.print(mq2Value);
      lcd.setCursor(0, 1);
      lcd.print("MQ135:");
      lcd.print(mq135Value);
    } else {
      lcd.setCursor(0, 0);
      lcd.print("                ");  // clear first line
      lcd.setCursor(0, 0);
      lcd.print("Temp:");
      if (lastTemp == -1) {
        lcd.print("Err");
      } else {
        lcd.print(lastTemp, 1);
        lcd.print((char)223);
        lcd.print("C");
      }
      lcd.setCursor(0, 1);
      lcd.print("BPM:");
      lcd.print(bpm);
    }
  }
  delay(500);
}

void buzzerOnForDuration(unsigned long duration_ms) {
  unsigned long start = millis();
  while (millis() - start < duration_ms) {
    digitalWrite(buzzerPin, HIGH);
    delay(100);
    digitalWrite(buzzerPin, LOW);
    delay(100);
    if (digitalRead(buttonPin) == HIGH)
      break;
  }
  digitalWrite(buzzerPin, LOW);
}


void buzzerBeep(int count, unsigned int duration_ms) {
  for (int i = 0; i < count; i++) {
    digitalWrite(buzzerPin, HIGH);
    delay(duration_ms);
    digitalWrite(buzzerPin, LOW);
    if (i < count - 1) delay(duration_ms);  
  }
}


void buzzerContinuous(unsigned long duration_ms) {
  unsigned long start = millis();
  while (millis() - start < duration_ms) {
    digitalWrite(buzzerPin, HIGH);
  }
  digitalWrite(buzzerPin, LOW);
}
