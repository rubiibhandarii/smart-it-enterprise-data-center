#include <Blynk.h>

#include <SoftwareSerial.h>

// rx tx to connect with arduino
SoftwareSerial s(D7,D8);
#include <ArduinoJson.h>

#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>
#include <Servo.h>


//Gas Sensor

const int GasBuzzer = D4;
int smokePIN = A0;
int sensorThres = 250;
const char auth[] = "v-bKqTH0lArwiafuf9BJSTTYNZ3GcMWM"; //Auth code sent via Email
const char ssid[] = "Rakesh"; //Wifi name
const char pass[] = "123456789";  //Wifi Password


//Ultrasonic Sensor

#define trigpin D5 //set trigpin
#define echopin D6 //set echopin
int ultrasonicServoPIN = D0;
Servo ultrasonicServo;// declare servo name type servo
int duration, distance;//declare variable for unltrasonic
int pos = 0;


void setup() {
  // Initialize Serial port
  Serial.begin(9600);

    // --------------------Gas Sensor ---------------------------
  
  pinMode(GasBuzzer, OUTPUT);
  pinMode(smokePIN, INPUT);

  // -------------------Ultrasonic sensors -----------------------
  pinMode(trigpin, OUTPUT);
  pinMode(echopin, INPUT);
  ultrasonicServo.attach(ultrasonicServoPIN);// attach your servo
  ultrasonicServo.writeMicroseconds(1500);
  
//  s.begin(9600);
//  while (!Serial) continue;


  Blynk.begin(auth, ssid, pass);
 
}
 
void loop() {
 StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(s);
  if (root == JsonObject::invalid())
    return;
 
//  Serial.println("JSON received and parsed");
  root.prettyPrintTo(Serial);
  Serial.print("Temperature Data");
  Serial.println("");
  int data1=root["data1"];
  Serial.print(data1);


  // For temperature sensor notification
  if(data1 > 30){
    Blynk.notify("Alert : Temperature is high in the server room");
  }
  else{
    
  }



// ------------------------ Gas Sensor -----------------
  
  int analogSensor = analogRead(smokePIN);
//  root["data2"] = analogSensor;

  Serial.print("Gas Sensor: ");
  Serial.println(analogSensor);
  
  // Checks if it has reached the threshold value
  if (analogSensor > sensorThres){
    tone(GasBuzzer, 1000, 200);
    Blynk.notify("Alert : Gas is detected in the server room.");
  }
  else{
    noTone(GasBuzzer);
  }
//  delay(100); 

    Blynk.run();


// ----------------------------- Ultrasonic -------------------------- 


  digitalWrite(trigpin,HIGH);
//  _delay_ms(2);
  digitalWrite(trigpin, LOW);
  
  duration=pulseIn(echopin,HIGH); 
  distance=(duration/2)/29.1; 
  if(distance <=20)// if ultrasonic sensor detects an obstacle less than 20cm in 90 degree angle.
  {
     for (pos = 0; pos <= 60; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      ultrasonicServo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
  
    delay(5000);
    
    for (pos = 60; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      ultrasonicServo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
  
    delay(5000);
  }
  else
  {
    ultrasonicServo.write(0);// else servo stays at 90 degree angle.
    delay(600);
  }
  
  Serial.print("cm"); //print distance unit cm
  Serial.println(distance);//distance




 
}
