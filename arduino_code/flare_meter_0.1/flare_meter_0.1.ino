#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduinoJson.h>
#include "arduino_secrets.h"

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;     // the Wifi radio's status
char server[] = "services.swpc.noaa.gov";   // NOAA server
String server_results; // Contains the returned JSON string
WiFiSSLClient client;  // Wifi client definition

int dial_value; // Contains the numerical value for the flare
int dial_base;  // Contains the offset for each flare type, A, B, C, M, X
int dial_increment; // Contains the per dial position number for each whole number

StaticJsonDocument<768> doc; // Allocates the memory size of the JSON
     

void setup() {
  Serial.begin(9600); // Start USB Serial Port
  Serial1.begin(2400); // Start comms to meter
  
  Serial1.println(0x24,HEX); //Zero the meter

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");

  }

  // attempt to connect to Wifi network:

  while (status != WL_CONNECTED) {

    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
    // wait 5 seconds for connection:
    delay(5000);
  }

}
String connectandgrab(){
  
    String server_result;
    String output;
    // if you get a connection, report back via serial:
    output = client.connectSSL(server, 443);
    
    if (output) {                     //  port 443 for HTTPS
      //Serial.println("connected to server");
      // Make a HTTP request:
      client.println("GET /json/goes/primary/xray-flares-latest.json HTTP/1.1");
      client.println("Host: services.swpc.noaa.gov");
      client.println("Connection: close");
      client.println();
    }
    else
    {
      Serial.println("failed to connect");
      Serial.println(output);
    }
    delay(1000);
  
    int start = 0;
    // if there are incoming bytes available
     // from the server, read them and print them:
    while (client.available()) {
      char c = client.read();
      if(c == '{')
      {
        start = 1;
      }
    
      if(start == 1)
      {
        server_result = server_result + c;
      }   
    
    }
  
  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    //Serial.println("disconnecting from server.");
    client.stop();
    start = 0;

    return server_result;
}
}

void loop() {
 
    Serial.println("Going into grab routine");
    server_results = connectandgrab();
    Serial.println(server_results);    
    DeserializationError error = deserializeJson(doc, server_results);
    
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }
    else {
      String current_class = doc["current_class"];
      char flare_class = current_class.charAt(0);
      char flare_number = current_class.charAt(1);
      int flare_int = flare_number - '0';
      switch (flare_class) {
        case 'A':
          dial_base = 33;
          dial_increment = 37;
          Serial.println("Switch A");
          break;
        case 'B':
          dial_base = 403;
          dial_increment = 24;
          Serial.println("Switch B");
          break;
        case 'C':
          dial_base = 643;
          dial_increment = 15;
          Serial.println("Switch C");
          break;
        case 'M':
          dial_base = 793;
          dial_increment = 11;
          Serial.println("Switch M");
          break;  
        case 'X':
          dial_base = 903;
          dial_increment = 6;
          Serial.println("Switch X");
          break;  
        default:
          Serial.println("Invalid character inputed to class decoding switch");
          break;
      }

      Serial.println(current_class);
      dial_value = (flare_int*dial_increment) + dial_base;
      Serial1.println(dial_value); 
      
    }
      
    // wait 5 minutes for update:
    delay(300000);
}
