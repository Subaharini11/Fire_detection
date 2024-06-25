#include <ESP8266WiFi.h> 
#include <DHT.h> 
#include <WiFiClient.h> 
#include <ESP8266HTTPClient.h> 
#define FLAME_PIN D2 
#define SMOKE_PIN A0 
#define BUZZER_PIN D5 
#define DHTPIN D3 
#define DHTTYPE DHT11 DHT dht(DHTPIN,DHTTYPE); 
int flameValue; int smokeValue; const int SMOKE_THRESHOLD = 1000; 
const char* ssid = "Suba"; const char* password = "keerthana"; const String apiKey = "R5CPN4UDBB8AZH83"; const String server = "api.thingspeak.com"; void setup() {   Serial.begin(9600); 
  pinMode(FLAME_PIN, INPUT);   pinMode(SMOKE_PIN, INPUT);   pinMode(BUZZER_PIN, OUTPUT); 
  dht.begin(); 
  WiFi.begin(ssid, password); while (WiFi.status() != WL_CONNECTED) {     delay(1000); 
    Serial.println("Connecting to WiFi..."); 
  } 
  Serial.println("Connected to WiFi"); 
                    Serial.print("Local IP address: ");   Serial.println(WiFi.localIP()); 
} void loop() {   flameValue = digitalRead(FLAME_PIN);   smokeValue = analogRead(SMOKE_PIN);   float temperature = dht.readTemperature();   float humidity = dht.readHumidity(); 
  Serial.print("Flame: "); 
  Serial.println(flameValue); 
  Serial.print("Smoke: "); 
  Serial.println(smokeValue); 
  Serial.print("Temperature: "); 
  Serial.print(temperature); 
  Serial.print("°C, Humidity: "); 
  Serial.print(humidity);   Serial.println("%");   if (WiFi.status() == WL_CONNECTED) { 
    WiFiClient client; 
    HTTPClient http;     String url = "http://";     url += server;     url += "/update?";     url += "api_key=" + apiKey;     url += "&field1=" + String(flameValue);     url += "&field2=" + String(smokeValue);     url += "&field3=" + String(temperature);     url += "&field4=" + String(humidity);     http.begin(client,url);     int httpCode = http.GET();     String payload = http.getString(); 
    Serial.println("HTTP Code: " + String(httpCode));     Serial.println("Server response: " + payload);     http.end(); 
  } 
  if (flameValue==LOW) {     digitalWrite(BUZZER_PIN, HIGH);     delay(500);     digitalWrite(BUZZER_PIN, LOW);     delay(500); 
  } 
} 