#include <WiFiNINA.h>              // Library to connect Nano 33 IoT to WiFi
#include <ArduinoHttpClient.h>     // Library for sending HTTP requests
#include "DHT.h"                   // Library for interfacing with the DHT sensor

// WiFi credentials (update these as needed)
char ssid[] = "F14";               // WiFi SSID
char pass[] = "512512512";         // WiFi password

// ThingSpeak settings
String apiKey = "J9C7BVP4NGB2IZXZ";  // Your ThingSpeak Write API Key
const char server[] = "api.thingspeak.com";  // ThingSpeak server
const int port = 80;                        // HTTP port

// DHT sensor configuration
#define DHTPIN 2                  // DHT data pin connected to digital pin 2
#define DHTTYPE DHT11            // Type of DHT sensor (DHT11)
DHT dht(DHTPIN, DHTTYPE);        // Create DHT object

// Create client objects for WiFi and HTTP
WiFiClient wifi;
HttpClient client = HttpClient(wifi, server, port);

void setup() {
  Serial.begin(9600);           // Start Serial monitor at 9600 baud rate
  dht.begin();                  // Initialize the DHT sensor

  Serial.print("Connecting to WiFi: ");
  WiFi.begin(ssid, pass);       // Begin WiFi connection

  // Wait until connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  // WiFi connected
  Serial.println("\nConnected to WiFi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());  // Print device IP address
}

void loop() {
  float temperature = dht.readTemperature();  // Read temperature in Celsius

  // Check if reading was successful
  if (isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(30000);  // Wait 30 seconds before trying again
    return;
  }

  // Print the temperature to Serial Monitor
  Serial.print("Temperature (°C): ");
  Serial.println(temperature);

  // Format data as HTTP POST payload
  String postData = "api_key=" + apiKey + "&field1=" + String(temperature);

  // Send data to ThingSpeak via HTTP POST
  client.post("/update", "application/x-www-form-urlencoded", postData);

  // Check and print response from ThingSpeak
  int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  Serial.print("HTTP Status: ");
  Serial.println(statusCode);    // 200 means OK
  Serial.print("Response: ");
  Serial.println(response);      // Usually returns entry ID or 0

  delay(5000); // Wait 5 seconds before sending the next data point
}
