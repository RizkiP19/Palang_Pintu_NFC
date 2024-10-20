#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN D2
#define RST_PIN D3
#define BUZZER_PIN D4
#define IR_PIN D8

MFRC522 rfid(SS_PIN, RST_PIN);  // Instance of the class
Servo myServo;                  // Create a Servo object

// Ganti dengan UID dari kartu master dan keychain Anda
byte masterUID[] = {0x73, 0xB0, 0x23, 0x2D}; // UID master card
byte keychainUID[] = {0xC5, 0x2A, 0x2F, 0x00};

void setup() {
  Serial.begin(115200);  // Start serial communication
  SPI.begin();           // Init SPI bus0
  rfid.PCD_Init();      // Init MFRC522
  myServo.attach(D1);   // Attach servo to pin D4
  pinMode(BUZZER_PIN, OUTPUT);  // Set buzzer pin as output
  Serial.println("RFID Reader is ready");
}

void loop() {
  if (detectRFID()) {
    if (compareUID()) {
      Serial.println("Access granted");
      myServo.write(180);  // Put servo to position 90 degrees
      tone(BUZZER_PIN, 1000, 500);  // Produce a 1kHz tone for 500ms
    } else {
      Serial.println("Access denied");
      tone(BUZZER_PIN, 500, 500);  // Produce a 500Hz tone for 500ms
    }
  }
  // Add code for IR sensor
  if (digitalRead(IR_PIN) == LOW) {  // Assume IR_PIN is the pin connected to the IR sensor
    myServo.write(0);  // Close the servo
  }
}

bool detectRFID() {
    if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
        return false;
    }

    // Cetak UID yang dibaca
    Serial.print("UID yang dibaca: ");
    for (byte i = 0; i < rfid.uid.size; i++) {
        Serial.print(rfid.uid.uidByte[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
    
    return true;
}

bool compareUID() {
  // Bandingkan UID yang terbaca dengan UID master dan keychain
  for (int i = 0; i < 4; i++) {
    if (rfid.uid.uidByte[i] != masterUID[i] && rfid.uid.uidByte[i] != keychainUID[i]) {
      return false;
    }
  }
  return true;
}