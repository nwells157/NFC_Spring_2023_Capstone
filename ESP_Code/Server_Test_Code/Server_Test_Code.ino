#include <WiFi.h> 
#include <HTTPClient.h> 

// Hardwiring WiFi connection to Doug's iPhone
const char* ssid = "iPh0n3"; 
const char* password = "123456789"; 
 
// Server IP address Path 
const char* serverName = "http://18.117.119.190/api/lock"; 

// Test Variables to only run code 1 time
const int i = 1; 
int j = 0; 

void setup() { 
  Serial.begin(115200); 
 
  WiFi.begin(ssid, password); 
  Serial.println("Connecting"); 
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500); 
    Serial.print("."); 
  } 
  Serial.println(""); 
  Serial.print("Connected to WiFi network with IP Address: "); 
  Serial.println(WiFi.localIP());  
} 
 
void loop() { 
    if(WiFi.status()== WL_CONNECTED){ 
      if(j<i)
      {
      WiFiClient client; 
      HTTPClient http;
      j=j+1;
      // Server with IP address as path 
      http.begin(client, serverName); 
       
      // Node-RED/server authentication, insert user and password below 
      // http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD"); 

      // HTTP request with a content type: application/json: 
      http.addHeader("Content-Type", "application/json"); 
      int httpResponseCode = http.POST("{}");  // Posting empty curly brackets to change lock state
       

             Serial.print("HTTP Response code: "); 
      Serial.println(httpResponseCode); 
         
      // Free resources 
      http.end(); 
      } // End of test loop to only run 1 time
    } 
    else { 
      Serial.println("WiFi Disconnected"); 
    } 
  } 
