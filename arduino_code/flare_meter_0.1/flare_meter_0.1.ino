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
  
  // Zero the meter. Sent as the ASCII text "24" (HEX formatting of 0x24),
  // NOT the raw byte 0x24. Confirm which the gauge firmware expects: if it
  // wants the raw byte use Serial1.write(0x24) instead.
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
  connectWiFi();

}

// Connect (or reconnect) to the configured WiFi network, blocking until
// the link is up. Safe to call repeatedly; returns immediately if already
// connected.
void connectWiFi() {
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
    // wait 5 seconds for connection:
    delay(5000);
  }
  status = WL_CONNECTED;
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
  
    // Read the response, skipping the HTTP headers explicitly: everything
    // up to and including the blank line (CRLF CRLF) that terminates them is
    // discarded, and only the body is captured. This is more robust than
    // capturing from the first '{', which could trip on a '{' in a header.
    bool headers_done = false;
    String header_line;
    while (client.available()) {
      char c = client.read();

      if (!headers_done) {
        if (c == '\n') {
          // A line containing only "\r" (or nothing) is the blank line that
          // separates headers from the body.
          if (header_line.length() == 0 || header_line == "\r") {
            headers_done = true;
          }
          header_line = "";
        } else {
          header_line = header_line + c;
        }
      } else {
        server_result = server_result + c;
      }
    }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    //Serial.println("disconnecting from server.");
    client.stop();
  }

  return server_result;   // always return, even if still connected
}

void loop() {

    // Recover from WiFi dropouts: if the link is down, reconnect before
    // attempting an update. Without this a single AP dropout would stall
    // updates forever.
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("WiFi connection lost, reconnecting");
      connectWiFi();
    }

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

      // Validate the flare class before mapping. NOAA should return a class
      // letter followed by a numeric magnitude (e.g. "C3"), but an empty,
      // short, or malformed string would otherwise produce a garbage dial
      // value. On bad data, leave the needle where it is and skip this cycle.
      if (current_class.length() < 2 || !isDigit(current_class.charAt(1))) {
        Serial.print("Unexpected current_class format, holding position: ");
        Serial.println(current_class);
        delay(300000);
        return;
      }

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
