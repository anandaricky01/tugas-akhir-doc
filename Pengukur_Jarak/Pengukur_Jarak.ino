/*
 * Arduino communication with  a web server example
 */
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

//define sound velocity in cm/uS
#define SOUND_VELOCITY 0.034

const int LED = 16; //set LED on pin GPIO16
const char *ssid = "cikito";  
const char *password = "malang326744";
const String api_key = "$2y$10$EzMrYt0EFENCRFfDHzt7COTDu8ehQ3RHPd9YLeFprEvrNrczYy8cG";
 WiFiClient WifiClient;
 
//Web/Server address to read/write from 
const char *host = "http://192.168.1.9:8000/api/data";   //your IP/web server address
const int trigPin = 12;
const int echoPin = 14;

void setup() {
  delay(1000);
  Serial.begin(115200);
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(LED, OUTPUT); // Sets the LED as an Output
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");
 
  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  digitalWrite(LED, HIGH);
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
}
 
void loop() {
  //Declare object of class HTTPClient
  HTTPClient http;

  //Prepare data
  String device_id, data, postData;
//  int tmp_value=random(10, 25);
  int data_velocity = ultrasonic();
  device_id = String(14);
  data = String(data_velocity);
 
  //prepare request
  postData = "device_id=" + device_id + "&data=" + data + "&api_key=" + api_key;
  http.begin(WifiClient, host);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpCode = http.POST(postData);
  String payload = http.getString();
 
  Serial.println(httpCode);
  Serial.println(payload);
  http.end();
  delay(5000);
}

int ultrasonic(){
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  int duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  int distanceCm = duration * SOUND_VELOCITY/2;

  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  Serial.print("Waktu tempuh (ms):");
  Serial.println(duration);

  return distanceCm;
  }
