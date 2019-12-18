



/*
 *  This sketch sends a message to a TCP server
 *
 */

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include<stdlib.h>
#include "SPI.h"
#include <TFT_22_ILI9225.h>

ESP8266WiFiMulti WiFiMulti;
 


#define TFT_RST 2
#define TFT_RS  4
#define TFT_CS  15  // SS
#define TFT_SDI 13  // MOSI
#define TFT_CLK 14  // SCK
#define TFT_LED 5   // 0 if wired to +5V directly
// Use hardware SPI (faster - on Uno: 13-SCK, 12-MISO, 11-MOSI)
TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_LED);
// Use software SPI (slower)
//TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_SDI, TFT_CLK, TFT_LED);

// Variables and constants
uint16_t x, y;
boolean flag = false;

byte mac[6]; 

void setup() {
  
    
    Serial.begin(115200);
     pinMode(16, OUTPUT);
   
    tft.begin();
    tft.setOrientation(3);

   
   
    // We start by connecting to a WiFi network
    WiFiMulti.addAP("JAZZTEL_teCr", "j6946x394749");

    Serial.println();
    Serial.println();
    Serial.print("Wait for WiFi... ");

    while(WiFiMulti.run() != WL_CONNECTED) {
        Serial.print(".");  
         tft.clear();   
         tft.setFont(Terminal6x8);
     
         tft.drawText(10, 10, "Conectando wifi...");  
         Serial.print("MAC: ");
   Serial.println(WiFi.macAddress());
         delay(500);
            
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    tft.drawText(10, 10, "WiFi conectado"); 
    tft.drawText(10, 30, "IP: " + WiFi.localIP().toString());
    delay(500);

   

}


void loop() {
    const uint16_t port = 3000;
    const char * host = "192.168.1.129"; // ip or dns
  
       
    Serial.print("connecting to ");
    Serial.println(host);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;

    if (!client.connect(host, port)) {
        Serial.println("connection failed");
        Serial.println("wait 5 sec...");
        delay(5000);
    
  
        return;
    }

  if(client.connect(host, port)) {

     char c = client.read();
     Serial.println("recibidooooo-----");
    Serial.println(c);
    Serial.println("recibidooooo-----");
   
    //Serial.println("Connected to server");
     tft.drawText(10, 60,"Conectado al server");
    // Make a HTTP request
   
 
  }

 
}

