#include <SPI.h>
#include <MFRC522.h>
#include <WiFi.h> 
#include <HTTPClient.h> 
#include <Servo.h>

#define RST_PIN 10 // RFID RST to pin 10
#define SS_PIN 5 // RFID SDA to pin 5

Servo lockServo;
int pos = 0;

byte readCard[4];
String NicksKey = "7391611C";
String tagID = "";

MFRC522 mfrc522(SS_PIN, RST_PIN);


void setup() {
  Serial.begin(115200);   // Initialize serial communications with the PC
  Serial.println("Looking For Key");
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  
  lockServo.attach(34);  // Servo is on pin 34

}

void loop() {
  while (getID()) 
  {
    if ( tagID == NicksKey)
    {
      Serial.println("Found");
      // Move the DC motor forward at maximum speed 
      Serial.println("Unlocking for 5 seconds"); 
      for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        lockServo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
      }


      delay(5000); 


      Serial.println("Locking Door"); 
      for (pos = 180; pos <= 0; pos -= 1) { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        lockServo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
      }



    }
  }
}


  //Read new tag if available
boolean getID() 
{
  // Getting ready for Reading PICCs
  if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
  return false;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) { //Since a PICC placed get Serial and continue
  return false;
  }
  tagID = "";
  for ( uint8_t i = 0; i < 4; i++) { // The MIFARE PICCs that we use have 4 byte UID
  //readCard[i] = mfrc522.uid.uidByte[i];
  tagID.concat(String(mfrc522.uid.uidByte[i], HEX)); // Adds the 4 bytes in a single String variable
  }
  tagID.toUpperCase();
  mfrc522.PICC_HaltA(); // Stop reading
  return true;
}
