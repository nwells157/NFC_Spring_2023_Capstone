// Motor A 
int motor1Pin1 = 27;  
int motor1Pin2 = 26;  
int enable1Pin = 14;  
 
void setup() { 
  // sets the pins as outputs: 
  pinMode(motor1Pin1, OUTPUT); 
  pinMode(motor1Pin2, OUTPUT); 
  pinMode(enable1Pin, OUTPUT); 
 
  Serial.begin(115200); 
 
  // testing 
  Serial.print("Testing DC Motor..."); 
} 
 
void loop() { 
  // Move the DC motor forward at maximum speed 
  Serial.println("Moving Forward"); 
  digitalWrite(motor1Pin1, LOW); 
  digitalWrite(motor1Pin2, HIGH);
  analogWrite(enable1Pin, 255);  
  delay(2000); 
 
  // Stop the DC motor 
  Serial.println("Motor stopped"); 
  digitalWrite(motor1Pin1, LOW); 
  digitalWrite(motor1Pin2, LOW); 
  delay(1000); 
 
  // Move DC motor backwards at maximum speed 
  Serial.println("Moving Backwards"); 
  digitalWrite(motor1Pin1, HIGH); 
  digitalWrite(motor1Pin2, LOW);  
  analogWrite(enable1Pin, 255); 
  delay(2000); 
 
  // Stop the DC motor 
  Serial.println("Motor stopped"); 
  digitalWrite(motor1Pin1, LOW); 
  digitalWrite(motor1Pin2, LOW); 
  delay(1000); 
 
} 
