/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-rfid-nfc
 */
#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN  5  // ESP32 pin GIOP5 
#define RST_PIN 27 // ESP32 pin GIOP27 

MFRC522 mfrc522(SS_PIN, RST_PIN); 

void setup() {
  Serial.begin(115200);
  SPI.begin(); // init SPI bus
  mfrc522.PCD_Init(); // init MFRC522

  Serial.println("Tap an RFID/NFC tag on the RFID-RC522 reader");
}

void loop() {
  byte block;
  byte len;
  MFRC522::StatusCode status;

  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
  
  if (mfrc522.PICC_IsNewCardPresent()) { // new tag is available
    if (mfrc522.PICC_ReadCardSerial()) { // NUID has been readed
      MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
      //Serial.print("RFID/NFC Tag Type: ");
      //Serial.println(rfid.PICC_GetTypeName(piccType));

      // print UID in Serial Monitor in the hex format
      Serial.print("{\"UID\":\"");
      for (int i = 0; i < mfrc522.uid.size; i++) {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
        Serial.print(mfrc522.uid.uidByte[i], HEX);
      }
     
      Serial.print("\",\"PN\":\"");
      byte buffer2[18]{0};
      block = 1;
      len=18;

      status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key, &(mfrc522.uid)); //line 834
      if (status != MFRC522::STATUS_OK) {
          //Serial.print(F("Authentication failed: "));
          //Serial.println(mfrc522.GetStatusCodeName(status));
          return;
       }

      status = mfrc522.MIFARE_Read(block, buffer2, &len);
      if (status != MFRC522::STATUS_OK) {
          //Serial.print(F("Reading failed: "));
          //Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }

     //PRINT LAST NAME
      for (uint8_t i = 0; i < 16; i++) {
        Serial.write(buffer2[i] );
       }
      

      Serial.println("\"}");

      mfrc522.PICC_HaltA(); // halt PICC
      mfrc522.PCD_StopCrypto1(); // stop encryption on PCD
    }
  }
  
 
}
