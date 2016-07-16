#include <ESP8266httpUpdate.h>

#include <ESP8266HTTPClient.h>

#include <OneWire.h>
#include <DallasTemperature.h>


// www.arduinesp.com 
//
// Plot DTH11 data on thingspeak.com using an ESP8266 
// April 11 2015
// Author: Jeroen Beemster
// Website: www.arduinesp.com
 
 
#include <DHT.h>
#include <ESP8266WiFi.h>


#define ONE_WIRE_BUS 2  // DS18B20 pin
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);



// replace with your channel's thingspeak API key, 
String apiKey = "M0LM4QPBGJXQ4ZWV";
const char* ssid = "AUTOMATION";
const char* password = "AUTOMATE";
 
const char* server = "api.thingspeak.com";
#define DHTPIN 2 // what pin we're connected to
 
DHT dht(DHTPIN, DHT11,15);
WiFiClient client;
   
 
void setup() {                
  Serial.begin(115200);
  delay(10);
  dht.begin();
  
  WiFi.begin(ssid, password);
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
   
  WiFi.begin(ssid, password);
   
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
}
 
 
void loop() {

  DS18B20.requestTemperatures(); 
  float t = DS18B20.getTempCByIndex(0);
  
  float h = 40;
//  float t = 105;
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");

    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" degrees Celcius Humidity: "); 
    Serial.print(h);
    return;
  }
 
  if (client.connect(server,443)) {  //   "184.106.153.149" or api.thingspeak.com
    String postStr = apiKey;
           postStr +="&field1=";
           postStr += String(t);
           postStr +="&field2=";
           postStr += String(h);
           postStr += "\r\n\r\n";
 
     client.print("POST /update HTTP/1.1\n"); 
     client.print("Host: api.thingspeak.com\n"); 
     client.print("Connection: close\n"); 
     client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n"); 
     client.print("Content-Type: application/x-www-form-urlencoded\n"); 
     client.print("Content-Length: "); 
     client.print(postStr.length()); 
     client.print("\n\n"); 
     client.print(postStr);
           
 
     Serial.print("Temperature: ");
     Serial.print(t);
     Serial.print(" degrees Celcius Humidity: "); 
     Serial.print(h);
     Serial.println("% send to Thingspeak");    
  }
  client.stop();
   
  Serial.println("Waiting...");    
  // thingspeak needs minimum 15 sec delay between updates
  delay(30000);  
}
