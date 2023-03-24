#include <SPI.h>
#include <MFRC522.h>


#define RST_PIN 10
#define SS_PIN 5

byte readCard[4];
String NicksKey = "7391611C";
String tagID = "";

MFRC522 mfrc522(SS_PIN, RST_PIN);

// Motor A 
int motor1Pin1 = 27;  
int motor1Pin2 = 26;  
int enable1Pin = 14;  
 
// Setting PWM properties 
const int freq = 60000; 
const int pwmChannel = 0; 
const int resolution = 8; 
int dutyCycle = 255;

void setup() {
  Serial.begin(115200);   // Initialize serial communications with the PC
  Serial.println("Looking For Key");
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522

   // sets the pins as outputs: 
  pinMode(motor1Pin1, OUTPUT); 
  pinMode(motor1Pin2, OUTPUT); 
  pinMode(enable1Pin, OUTPUT); 

   // configure LED PWM functionalitites 
  ledcSetup(pwmChannel, freq, resolution); 
   
  // attach the channel to the GPIO to be controlled 
  ledcAttachPin(enable1Pin, pwmChannel); 
}

void loop() {
  // put your main code here, to run repeatedly:
  while (getID()) 
  {
    if ( tagID == NicksKey)
    {
      Serial.println("Found");
      // Move the DC motor forward at maximum speed 
      Serial.println("Moving Forward"); 
      digitalWrite(motor1Pin1, LOW); 
      digitalWrite(motor1Pin2, HIGH);  
      analogWrite(enable1Pin, dutyCycle);
      delay(2000); 

      // Stop the DC motor 
      Serial.println("Motor stopped"); 
      digitalWrite(motor1Pin1, LOW); 
      digitalWrite(motor1Pin2, LOW); 

      delay(5000); 

      // Move DC motor backwards at maximum speed 
      Serial.println("Moving Backwards"); 
      digitalWrite(motor1Pin1, HIGH); 
      digitalWrite(motor1Pin2, LOW);  
      analogWrite(enable1Pin, dutyCycle);
      delay(2000); 

      // Stop the DC motor 
      Serial.println("Motor stopped"); 
      digitalWrite(motor1Pin1, LOW); 
      digitalWrite(motor1Pin2, LOW); 

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
