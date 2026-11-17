//DEVRAJ YADAV

#include <SPI.h>
#include <MFRC522.h>
#include <Keypad.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// RFID pins
#define SS_PIN 10
#define RST_PIN 9

MFRC522 rfid(SS_PIN, RST_PIN);

// Servo
Servo doorServo;
#define SERVO_PIN 6

// Buzzer and LED
#define BUZZER_PIN 7
#define LED_PIN 8

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Keypad configuration
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {A0, A1, A2, A3};

Keypad keypad = Keypad(
  makeKeymap(keys),
  rowPins,
  colPins,
  ROWS,
  COLS
);

// Authorized RFID card UID
// Replace this example with your own card UID
byte authorizedUID[] = {0x01, 0x02, 0x03, 0x04};

// Security PIN
String correctPIN = "1234";
String enteredPIN = "";

// Check RFID card
bool checkCard() {
  if (!rfid.PICC_IsNewCardPresent()) {
    return false;
  }

  if (!rfid.PICC_ReadCardSerial()) {
    return false;
  }

  Serial.print("Card UID: ");

  bool authorized = (rfid.uid.size == sizeof(authorizedUID));

  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i], HEX);
    Serial.print(" ");

    if (i >= sizeof(authorizedUID) ||
        rfid.uid.uidByte[i] != authorizedUID[i]) {
      authorized = false;
    }
  }

  Serial.println();

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

  return authorized;
}

// Access denied
void accessDenied() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ACCESS DENIED");

  digitalWrite(LED_PIN, LOW);

  tone(BUZZER_PIN, 400);
  delay(1000);
  noTone(BUZZER_PIN);

  delay(1500);
}

// Unlock door
void unlockDoor() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ACCESS GRANTED");

  lcd.setCursor(0, 1);
  lcd.print("DOOR UNLOCKED");

  digitalWrite(LED_PIN, HIGH);

  tone(BUZZER_PIN, 1500);
  delay(200);
  noTone(BUZZER_PIN);

  doorServo.write(90);

  delay(5000);

  doorServo.write(0);
  digitalWrite(LED_PIN, LOW);

  lcd.clear();
  lcd.print("SCAN YOUR CARD");
}

// PIN entry
bool enterPIN() {
  enteredPIN = "";

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ENTER YOUR PIN:");

  lcd.setCursor(0, 1);

  while (true) {
    char key = keypad.getKey();

    if (key) {
      if (key >= '0' && key <= '9') {
        if (enteredPIN.length() < 4) {
          enteredPIN += key;
          lcd.print("*");
        }
      }

      else if (key == '*') {
        enteredPIN = "";
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
      }

      else if (key == '#') {
        return enteredPIN == correctPIN;
      }
    }
  }
}

void setup() {
  Serial.begin(9600);

  SPI.begin();
  rfid.PCD_Init();

  lcd.init();
  lcd.backlight();

  doorServo.attach(SERVO_PIN);
  doorServo.write(0);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);

  lcd.setCursor(0, 0);
  lcd.print("SMART DOOR LOCK");

  lcd.setCursor(0, 1);
  lcd.print("SCAN YOUR CARD");

  Serial.println("SMART DOOR LOCK READY");
  Serial.println("Scan your RFID card");
}

void loop() {
  if (checkCard()) {

    lcd.clear();
    lcd.print("CARD ACCEPTED");

    tone(BUZZER_PIN, 1200);
    delay(200);
    noTone(BUZZER_PIN);

    delay(1000);

    if (enterPIN()) {
      unlockDoor();
    } else {
      accessDenied();
      lcd.clear();
      lcd.print("SCAN YOUR CARD");
    }
  }
}