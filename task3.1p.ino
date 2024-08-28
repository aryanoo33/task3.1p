#include <WiFiNINA.h>
#include <HTTPClient.h>

// WiFi credentials
const char* ssid = "aryan iPhone";  // Your network SSID (WiFi name)
const char* password = "98765432";   // Your network password

// Sensor and threshold settings
const int lightSensorPin = A4;   // Analog pin connected to the light sensor
const int threshold = 500;       // Threshold value for sunlight detection (adjust as needed)

// IFTTT Webhook details
const char* serverAddress = "https://maker.ifttt.com";  // The server address for IFTTT Webhooks
const char* eventName = "Light_trigger";        // Your IFTTT event name
const char* IFTTT_Key = "ecptXDUSTOgRx9PL5zqy-7cReX1sWuZIckDcqcVgAU-";  // Your unique IFTTT key

void setup() {
  Serial.begin(9600);  // Start serial communication at 9600 baud rate for debugging
  connectToWiFi();     // Call function to connect to WiFi network
}

void loop() {
  // Read the light sensor value
  int lightLevel = analogRead(lightSensorPin);  
  Serial.print("Light Level: ");
  Serial.println(lightLevel);  // Print the current light level to the Serial Monitor

  // Check if the light level exceeds the threshold
  if (lightLevel > threshold) {  
    triggerIFTTTEvent();  // Trigger the IFTTT event if light is above the threshold
    delay(10000);         // Wait 10 seconds before checking the light level again
  }

  delay(1000);  // Delay for 1 second before the next reading to reduce sensor polling frequency
}

void connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);  // Wait 1 second before trying again
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");   // Print a message when connected
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());  // Print the device's IP address on the network
}

void triggerIFTTTEvent() {
  String url = String(serverAddress) + "/trigger/" + eventName + "/with/key/" + IFTTT_Key;

  HTTPClient http;
  http.begin(url);  // Begin HTTP connection
  int httpCode = http.GET();  // Send the HTTP GET request

  Serial.println("Sending request to IFTTT...");
  Serial.print("HTTP Response Code: ");
  Serial.println(httpCode);  // Print the HTTP status code

  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    Serial.println("Event triggered successfully!");  // Success message
    Serial.print("Response: ");
    Serial.println(payload);  // Print the response from the IFTTT server
  } else {
    Serial.println("Failed to trigger event.");  // Failure message
  }

  http.end();  // End the HTTP connection
}