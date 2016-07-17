#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>


const int LDR = A0;
const int BUTTON = 4;
const int RED = 15;
const int GREEN = 12;
const int BLUE = 13;


ESP8266WiFiMulti WiFiMulti;


#define USE_SERIAL Serial


ESP8266WiFiMulti wifiMulti;


void setup() 
{
  
    USE_SERIAL.begin(115200);
   // USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    wifiMulti.addAP("742nsf", "Wh0pp3rs");

        
    pinMode(LDR, INPUT);
    pinMode(BUTTON, INPUT);
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(BLUE, OUTPUT);
}
 
void loop()
{

     Serial.println();
  if (int stat = wifiMulti.run() != WL_CONNECTED) {
    Serial.print(">>WiFi not connected! run returned: ");
    Serial.println(stat);
    delay(500);

  }
  else{

      WiFi.printDiag(Serial);
      Serial.print("[loop] IP address: ");
      Serial.println(WiFi.localIP());


          HTTPClient http;

    USE_SERIAL.print("[HTTP] begin...\n");
    // configure traged server and url
    http.begin("https://api.thingspeak.com/update?api_key=FA8AIHKLTO167J50&field1=10", "78 60 18 44 81 35 BF DF 77 84 D4 0A 22 0D 9B 4E 6C DC 57 2C"); //HTTPS


    // start connection and send HTTP header
        int httpCode = http.POST("api_key=FA8AIHKLTO167J50&field1=73");

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                USE_SERIAL.println(payload);
            }
        } else {
            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();



  
      Serial.print("LDR: ");
      Serial.println(analogRead(LDR));
      Serial.print("BUTTON: ");
      Serial.println(digitalRead(BUTTON));
 
      analogWrite(RED, random(0,1023));
      analogWrite(GREEN, random(0,1023));
      analogWrite(BLUE, random(0,1023));

         
      delay(30000);
   }
    
}
