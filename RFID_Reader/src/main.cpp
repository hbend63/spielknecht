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
#include <ArduinoJson.h>

#define SS_PIN 5   // ESP32 pin GIOP5
#define RST_PIN 27 // ESP32 pin GIOP27

MFRC522 mfrc522(SS_PIN, RST_PIN);

String kennung = "";
int counter{0};

String toMessage(String message, String value)
{
  DynamicJsonDocument doc(1024);
  doc["msg"] = message;
  doc["val"] = value;
  String output0;
  serializeJson(doc, output0); // Serialize Json Document to output String
  return output0;
}

void fromMessage(const String input, String &_message, String &_value)
{
  if ((input[0] == '{') && (input[input.length() - 1] == '}'))
  {
    String json = input;
    // Serial.print("JSON-String: "); Serial.println(json);
    //  Allocate the JSON document
    //
    //  Inside the brackets, 200 is the capacity of the memory pool in bytes.
    //  Don't forget to change this value to match your JSON document.
    //  Use https://arduinojson.org/v6/assistant to compute the capacity.
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, json);
    // Test if parsing succeeds.
    if (error)
    {
      // Serial.print(F("deserializeJson() failed: "));
      // Serial.println(error.f_str());
      return;
    }
    String message = doc["msg"];
    String value = doc["val"];
    _message = message;
    _value = value;
  }
}

void writeData()
{
  Serial.println("Writing....");
  // return;
  //  Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++)
    key.keyByte[i] = 0xFF;

  /*
  Serial.print(F("Card UID:"));    //Dump UID
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.print(F(" PICC type: "));   // Dump PICC type
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.println(mfrc522.PICC_GetTypeName(piccType));
  */
  byte buffer[34];
  byte block;
  MFRC522::StatusCode status;
  byte len;

  Serial.setTimeout(20000L); // wait until 20 seconds for input from serial
  // Ask personal data: Family name
  // Serial.println(F("Type Family name, ending with #"));

  counter++;

  String data = kennung + String(counter);
  for (int i = 0; i < data.length(); i++)
    buffer[i] = data[i];
  len = data.length();

  for (byte i = len; i < 30; i++)
    buffer[i] = ' '; // pad with spaces

  block = 1;
  // Serial.println(F("Authenticating using key A..."));
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else
    Serial.println(F("PCD_Authenticate() success: "));

  // Write block
  status = mfrc522.MIFARE_Write(block, buffer, 16);
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else
    Serial.println(F("MIFARE_Write() success: "));

  block = 2;
  // Serial.println(F("Authenticating using key A..."));
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  // Write block
  status = mfrc522.MIFARE_Write(block, &buffer[16], 16);
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else
    Serial.println(F("MIFARE_Write() success: "));
  /*
  // Ask personal data: First name
  Serial.println(F("Type First name, ending with #"));
  len = Serial.readBytesUntil('#', (char *) buffer, 20) ; // read first name from serial
  for (byte i = len; i < 20; i++) buffer[i] = ' ';     // pad with spaces

  block = 4;
  //Serial.println(F("Authenticating using key A..."));
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  // Write block
  status = mfrc522.MIFARE_Write(block, buffer, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else Serial.println(F("MIFARE_Write() success: "));

  block = 5;
  //Serial.println(F("Authenticating using key A..."));
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  // Write block
  status = mfrc522.MIFARE_Write(block, &buffer[16], 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else Serial.println(F("MIFARE_Write() success: "));


  Serial.println(" ");
  */
}

void readData()
{
  byte buffer[32]{0};
  byte block;
  byte len;
  MFRC522::StatusCode status;

  DynamicJsonDocument doc(2048);

  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++)
    key.keyByte[i] = 0xFF;

  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  //Serial.print("RFID/NFC Tag Type: ");
  //Serial.println(mfrc522.PICC_GetTypeName(piccType));  

  String uid = "";
  
  for (int i = 0; i < mfrc522.uid.size; i++)
  {
    char buffer[4];    
    snprintf(buffer, 4, "%02x", mfrc522.uid.uidByte[i]);    
    uid += buffer;   
  }
  
  doc["UID"] = uid;

  block = 1;
  len = 18;

  // Wenn Userdaten möglich sind:
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key, &(mfrc522.uid)); // line 834
  
  if (status == MFRC522::STATUS_OK)
  {
    status = mfrc522.MIFARE_Read(block, buffer, &len);
    if (status != MFRC522::STATUS_OK)
    {
      Serial.print(F("Reading failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }    
    String code = "";
    // PRINT PRODUCTCODE
    for (uint8_t i = 0; i < 16; i++)
    {
      code += char(buffer[i]);
    }
    code.trim();
    doc["PN"] = code;
  }

  String output0;
  serializeJson(doc, output0); // Serialize Json Document to output String
  Serial.println(output0);
}

bool doRead{true};

String inputString = ""; // a String to hold incoming data

void serialEvent()
{
  while (Serial.available())
  {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '}')
    {
      // String auswerten
      String msg, val;
      fromMessage(inputString, msg, val);
      inputString = "";
      if (msg == "cmd")
      {
        if (val[0] == 'w')
        {
          doRead = false;
          // Kennung auslesen
          kennung = "tst-";
          // Startwert auslesen
          counter = 5;
        }
        else
          doRead = true;
      }
    }
  }
}

void setup()
{
  Serial.begin(115200);
  SPI.begin();        // init SPI bus
  mfrc522.PCD_Init(); // init MFRC522

  // Serial.println("Tap an RFID/NFC tag on the RFID-RC522 reader");
}

void loop()
{
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }

  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial())
  {
    return;
  }

  if (doRead)
    readData();
  else
    writeData();

  delay(1000);               // change value if you want to read cards faster
  mfrc522.PICC_HaltA();      // halt PICC
  mfrc522.PCD_StopCrypto1(); // stop encryption on PCD
}
