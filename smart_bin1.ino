#define BLYNK_PRINT Serial

/* Fill in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL3nP0I0tR-"
#define BLYNK_TEMPLATE_NAME "Pathfinders"
#define BLYNK_AUTH_TOKEN "uCWGYK6zAdPxvKA_j3QHYmSVvwnd1qKO"

#include <ESP32Servo.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
// Define servo objects
Servo s1;
Servo s2;


const int sensor_pin = 32; //moisture
const int trigPin1 = 2; // Ultrasonic sensor's trigger pinint            input
const int echoPin1 = 4; // Ultrasonic sensor's echo pin    

const int trigPin2 = 18; // Ultrasonic sensor's trigger pinint            wet
const int echoPin2 = 19; // Ultrasonic sensor's echo pin    

const int trigPin3 = 22; // Ultrasonic sensor's trigger pinint            dry
const int echoPin3 = 23; // Ultrasonic sensor's echo pin
 //s1.attach(26);
 //s2.attach(25);

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "naveen";
char pass[] = "1234567890";
BlynkTimer timer;
void setup()
{
  // Debug console
  Serial.begin(9600);
  s1.attach(12);
  s2.attach(13);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
    pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
}

void loop()
{
   long duration1, distance1,duration2, distance2,duration3, distance3;
    ////////////////////////////////////////ultra 1,2,3  input
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = (duration1 / 2) / 29.1;
  Serial.print("\ninput: ");
  Serial.print(distance1);//input wastage
 //////////////////////////////////////////////////////wet ultra
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = (duration2 / 2) / 29.1;
  Serial.print("\nwet waste: ");
  Serial.println(distance2);
//////////////////////////////////////////////////////dry ultra
  digitalWrite(trigPin3, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin3, LOW);
  duration3 = pulseIn(echoPin3, HIGH);
  distance3 = (duration3 / 2) / 29.1;
  Serial.print("dry waste: ");
  Serial.print(distance3);
  Serial.println(" cm");

  Blynk.run();
  float moisture_percentage;
  int sensor_analog;
  sensor_analog = analogRead(sensor_pin);
  moisture_percentage = ( 100 - ( (sensor_analog/1023.00) * 100 ) );
  Serial.print("\nMoisture Percentage = ");
  Serial.print(moisture_percentage);
  Serial.print("%");
  //condition for servo motor////////////////////////////////////wetwaste
  if ((distance1 > 0 && distance1 <= 15) && (moisture_percentage >= 32))
  {
    for(int i=90 ; i>=0 ; i-=1){
      int j = 180-i;
      digitalWrite(trigPin2, LOW);
      digitalWrite(trigPin1, LOW);
       digitalWrite(trigPin3, LOW);
      s1.write(j);
      delay(15);
      s2.write(i);
      delay(15);
    Serial.print("\nsegregating wet waste");
    
  }
delay(2000);
  // Reverse direction and rotate both servos simultaneously
  for(int i=0; i<=90 ; i+=1){
      int j = 180-i;
     s1.write(j);
      delay(15);
      s2.write(i);
      delay(15);
      Serial.print("\nlid closing");

  }
       digitalWrite(trigPin2, HIGH);
     digitalWrite(trigPin1, HIGH);
     digitalWrite(trigPin3, HIGH);
  }
  /////////////////////////////////////////////////dry waste
  else if ((distance1 > 0 && distance1 <= 15) && (moisture_percentage <= 32))
  {
     for(int i=90 ; i<=180 ; i+=1){
      int j = 180-i;
     digitalWrite(trigPin2, LOW);
     digitalWrite(trigPin1, LOW);
      digitalWrite(trigPin3, LOW);
      s1.write(j);
      delay(15);
      s2.write(i);
      delay(15);
       Serial.print("\nsegregating dry waste");
      
    }
delay(2000);
  // Reverse direction and rotate both servos simultaneously
  for(int i=180; i>=90 ; i-=1){
      int j = 180-i;
      s1.write(j);
      delay(15);
      s2.write(i);
      delay(15);
     Serial.print("\nlid closing");

   }
     digitalWrite(trigPin2, HIGH);
     digitalWrite(trigPin1, HIGH);
     digitalWrite(trigPin3, HIGH);
  }
  /////////////////////////////closed bin
  else
  {
      s1.write(90);
      delay(15);
      s2.write(90);
      delay(15);
     Serial.print("\nlid is closed");
     digitalWrite(trigPin2, HIGH);
     digitalWrite(trigPin1, HIGH);
      digitalWrite(trigPin3, HIGH);
  }


  Blynk.virtualWrite(V0, distance2);
  Blynk.virtualWrite(V2, distance2);

  if((distance2>=5)||(distance2==0))
  {
   Blynk.virtualWrite(V1, 0);
  }
  else
  {
    Blynk.virtualWrite(V1, 1);
    Blynk.logEvent("wetlimit");
    Serial.print("danger");
  }

  if((distance3>=5)||(distance3==0))
  {
   Blynk.virtualWrite(V3, 0);
  }
  else
  {
    Blynk.virtualWrite(V3, 1);
    Blynk.logEvent("drylimit");
    Serial.print("danger");
  }

  delay(1000);
}
