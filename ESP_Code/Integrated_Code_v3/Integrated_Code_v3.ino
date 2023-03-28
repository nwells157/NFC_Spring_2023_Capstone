#include <SPI.h>
#include <MFRC522.h>
#include <WiFi.h> 
#include <HTTPClient.h> 
#include <ESP32Servo.h>

// Hardwiring WiFi connection to Doug's iPhone
const char* ssid = "iPh0n3"; 
const char* password = "123456789"; 
 
// Server IP address Path 
const char* serverName = "http://18.117.119.190/api/lock"; 

#define RST_PIN 10 // RFID RST to pin 10
#define SS_PIN 5 // RFID SDA to pin 5

Servo lockServo;
int pos = 0;
int lockServoPin = 21;

byte readCard[4];
String NicksKey = "7391611C";
String tagID = "";

MFRC522 mfrc522(SS_PIN, RST_PIN);


void setup() {
  Serial.begin(115200);   // Initialize serial communications with the PC
  
  WiFi.begin(ssid, password); 
  Serial.println("Connecting"); 
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500); 
    Serial.print("."); 
  } 

  Serial.println(""); 
  Serial.print("Connected to WiFi network with IP Address: "); 
  Serial.println(WiFi.localIP());  
  Serial.println("Looking For Key");
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522

	// Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	lockServo.setPeriodHertz(50);    // standard 50 hz servo
	lockServo.attach(lockServoPin, 500, 2400); // attaches the servo on pin 18 to the servo object
	// using default min/max of 1000us and 2000us
	// different servos may require different min/max settings
	// for an accurate 0 to 180 sweep

}

void loop() {

  if(WiFi.status()== WL_CONNECTED)
  {  
    WiFiClient client; 
    HTTPClient http;
    // Server with IP address as path 
    http.begin(client, serverName); 

    while (getID()) 
    {
      if ( tagID == NicksKey)
      {
        Serial.println("Found");

        // HTTP request with Unlocking
        http.addHeader("Content-Type", "application/json"); 
        int httpResponseCode = http.POST("{}");  // Posting empty curly brackets to change lock state
        http.end(); 
        
        Serial.print("HTTP Response code: "); 
        Serial.println(httpResponseCode); 
        
        Serial.println("Unlocking for 5 seconds"); 
        
        for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
          // in steps of 1 degree
          lockServo.write(pos);    // tell servo to go to position in variable 'pos'
          delay(10);             // waits 15ms for the servo to reach the position
        }


        delay(5000); 


        Serial.println("Locking Door"); 

        // HTTP Locking Request Sent to Server        
        http.addHeader("Content-Type", "application/json"); 
        int LockinghttpResponseCode = http.POST("{}");  // Posting empty curly brackets to change lock state
        http.end(); 

        Serial.print("HTTP Response code: "); 
        Serial.println(LockinghttpResponseCode); 

        for (pos = 180; pos >= 0; pos -= 1) { // goes from 0 degrees to 180 degrees
          // in steps of 1 degree
          lockServo.write(pos);    // tell servo to go to position in variable 'pos'
          delay(10);             // waits 15ms for the servo to reach the position
        }

      } 

          // Free resources 
      http.end(); 
    }
  }

  else { 
  Serial.println("WiFi Disconnected"); 
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

