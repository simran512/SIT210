#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include "DHT.h"

// WiFi credentials
char ssid[] = "F14";
char pass[] = "512512512";

// ThingSpeak details
String apiKey = "J9C7BVP4NGB2IZXZ";  
const char server[] = "api.thingspeak.com";
const int port = 80;

// DHT sensor setup
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

WiFiClient wifi;
HttpClient client = HttpClient(wifi, server, port);

void setup() {
  Serial.begin(9600);
  dht.begin();

  Serial.print("Connecting to WiFi: ");
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  float temperature = dht.readTemperature();

  if (isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(30000);
    return;
  }

  Serial.print("Temperature (°C): ");
  Serial.println(temperature);

  String postData = "api_key=" + apiKey + "&field1=" + String(temperature);

  // Send data to ThingSpeak
  client.post("/update", "application/x-www-form-urlencoded", postData);

  int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  Serial.print("HTTP Status: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);

  delay(5000);  // wait 5 seconds before sending again
}
