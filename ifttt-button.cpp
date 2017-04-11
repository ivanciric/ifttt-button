/**
 * IFTTT Button
 * 
 * Connect the button to D3 (GPIO0) and Ground.
 * When the button is pressed, an http GET request will be made 
 * to the IFTTT maker hook action that you've created.
 * You will need your IFTTT maker key and action name.
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;

int button_state = 0;

String action = ""; // maker action
String key = ""; // maker key

void setup() {
    pinMode(0, INPUT);
    USE_SERIAL.begin(115200);
    
    WiFiMulti.addAP("", ""); // your SSID and password
}

void loop() {
    if (digitalRead(0) == LOW && button_state != 1) {
        USE_SERIAL.print("Button Pushed\n");
        button_state = 1;
        sendRequest();
    }else{
        button_state = 0;
    }
    delay(200);
}

void sendRequest(){
    if((WiFiMulti.run() == WL_CONNECTED)) {
        HTTPClient http;
        http.begin("https://maker.ifttt.com/trigger/" + action + "/with/key/" + key, "C0:5D:08:5E:E1:3E:E0:66:F3:79:27:1A:CA:1F:FC:09:24:11:61:62"); 
        int httpCode = http.GET();
        if(httpCode < 0) {
          USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
    }
}
