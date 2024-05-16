/*This code is intended for an ESP32 microprocessor to operate two sensors,
a Capacitive Soil Moisture Sensor and a UV Detection Sensor, 
and send the data obtained every one minute time interval via Wi-Fi.
Remember to calibrate the Capacitive Soil Moisture Sensor before using it.*/

#include <Arduino.h>

#define SENSOR_PIN 14 // Define the analog pin connected to the moisture sensor
#define UV_SENSOR_PIN 23 // Define the analog pin connected to the UV sensor

#include <WiFi.h>
#include <HTTPClient.h>

int moistureLevel;
int uvLevel;

//wlan info
const char* ssid = "HHWLAN";
const char* password = "HHWLAN12345";

//server info, update accordingly. The data will be written at http://www.hh3dlab.fi/iot-demo/iotwrite.txt
const char* serverName = "http://www.hh3dlab.fi/groove/iotwrite.php";

//server update loop control variables
unsigned long lastTime = 0;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

void setup() {
  Serial.begin(9600); // Initialize serial communication
  //Serial.begin(115200);
  startWLAN();

}

void loop() {
  // Read moisture level from the sensor
  moistureLevel = analogRead(SENSOR_PIN);
  Serial.print("Moisture Level: ");
  Serial.println(moistureLevel);

  // Read UV level from the sensor
  uvLevel = analogRead(UV_SENSOR_PIN);
  Serial.print("UV Level: ");
  Serial.println(uvLevel);

  delay(60000); // Delay for stability

  //Send message every delay milliseconds
  if ((millis() - lastTime) > timerDelay) {
    sendMessage();
  }
}

void startWLAN() {
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}


//void sendMessage() {
//  //Check WiFi connection status
//  WiFi.reconnect();
//  delay(3000);
//  Serial.println("WiFi connected");
//
//  if (WiFi.status() == WL_CONNECTED) {
//    WiFiClient client;
//    HTTPClient http;
//
//    // Your Domain name with URL path or IP address with path
//    http.begin(client, serverName);
//
//    // Specify content-type header
//    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
//    // Data to send with HTTP POST
//    // Send HTTP POST request
//    int httpResponseCode = http.POST(httpRequestData);
//    Serial.println(serverName);
//    Serial.println(uvLevel + moistureLevel);
//
//    // If you need an HTTP request with a content type: application/json, use the following:
//    //http.addHeader("Content-Type", "application/json");
//    //int httpResponseCode = http.POST("{\"api_key\":\"tPmAT5Ab3j7F9\",\"sensor\":\"BME280\",\"value1\":\"24.25\",\"value2\":\"49.54\",\"value3\":\"1005.14\"}");
//
//    // If you need an HTTP request with a content type: text/plain
//    //http.addHeader("Content-Type", "text/plain");
//    //int httpResponseCode = http.POST("this works");
//
//    Serial.print("HTTP Response code: ");
//    Serial.println(httpResponseCode);
//
//    // Free resources
//    http.end();
//    WiFi.disconnect();
//    Serial.println("WiFi Disconnected");
//    delay(3000);
//  }
//  else {
//    Serial.println("WiFi Disconnected");
//  }
//  lastTime = millis();
//
//}

void sendMessage() {
  //Check WiFi connection status
  WiFi.reconnect();
  delay(3000);
  Serial.println("WiFi connected");

  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName);

    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    // Data to send with HTTP POST
    String str1 = "UV Level is: ";
    String str2 = "Moisture Level is: ";
    String httpRequestData = str1 + uvLevel + " " + str2 + moistureLevel;
    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);
    Serial.println(serverName);
    Serial.println(httpRequestData);

    // If you need an HTTP request with a content type: application/json, use the following:
    //http.addHeader("Content-Type", "application/json");
    //int httpResponseCode = http.POST("{\"api_key\":\"tPmAT5Ab3j7F9\",\"sensor\":\"BME280\",\"value1\":\"24.25\",\"value2\":\"49.54\",\"value3\":\"1005.14\"}");

    // If you need an HTTP request with a content type: text/plain
    //http.addHeader("Content-Type", "text/plain");
    //int httpResponseCode = http.POST("this works");

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    // Free resources
    http.end();
    WiFi.disconnect();
    Serial.println("WiFi Disconnected");
    delay(3000);
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  lastTime = millis();
}
/* THIS IS THE RECEIVING END PHP FILE
 * text/x-generic iotwrite.php ( PHP script, ASCII text, with CRLF line terminators )
<?php
    $_POST = file_get_contents('php://input');
    $myYear = date("d.m.Y");
    $myHour = date("H:i");
    $myFile = "iotwrite.txt";
    $fh = fopen($myFile, 'a') or die("can't open file");
    fwrite($fh, $myYear . " " .  $myHour);
    fwrite($fh, "\t");
    fwrite($fh, $_POST);
    fwrite($fh, "\n");    
    fclose($fh);
?>
*/
