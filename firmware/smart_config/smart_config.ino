/*
  ESP8266 Blink by Simon Peter
  Blink the blue LED on the ESP-01 module
  This example code is in the public domain

  The blue LED on the ESP-01 module is connected to GPIO1
  (which is also the TXD pin; so we cannot use Serial.print() at the same time)

  Note that this sketch uses LED_BUILTIN to find the pin with the internal LED
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

uint8_t GPIO16 = 16,
        GPIO5 = 5,
        GPIO4 = 4,
        GPIO0 = 0,
        GPIO2 = 2,
        GPIO14 = 14,
        GPIO12 = 12,
        GPIO13 = 13,
        GPIO15 = 15;

IPAddress    apIP(192, 168, 3, 1);

const char *ssid = "Khoa_Smart_Home";
const char *password = "smart12345678";
const int DIGITAL_PIN = 12; // Digital pin to be read


ESP8266WebServer server(80);

char connect_ssid[30], connect_password[30];
char IP[20] = "192.168.3.1";
char mac[20];

//==============================================================
//     This rutine is exicuted when you open its IP in browser
//==============================================================
void handleRoot(){

  char html[1500];

  //WiFi.localIP().toString().toCharArray(IP, 11);
  //WiFi.macAddress().toCharArray(mac, 11);

  snprintf(html, 1500, 
  "<!DOCTYPE html>\
    <html>\
    <head>\
      <meta charset=\"UTF-8\">\
      <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
      <style>\
        button {\
          background-color: #4CAF50;\
          color: white;\
          padding: 12px 20px;\
          border: none;\
          border-radius: 4px;\
          cursor: pointer;\
          }\
          .container {\
            border-radius: 5px;\
            text-align: center;\
            padding: 20px;\
          }  \
          .device{\
            padding: 5px;\
          }\
          .status{\
            text-shadow: 2px;\
            padding: 5px;\
            color: #d6a8ff;\
          }\
      </style>\
      <title>My Smart Home</title>\
    </head>\  
    <body>\
      <div class=\"container\">\
        <h2>Smart Switch</h2>\
        <div class=\"device\">\
          <div class=\"status\">IP: %s </div>\
          <div class=\"status\">MAC: %s </div>\
        </div>\
        <div class=\"device\">\
          <button type=\"button\" class=\"btn btn-warning\" onclick=\"window.location.href=\'Config\'\">Config Wifi</button>\
        </div>\
        <div class=\"device\">\
          <button type=\"button\" class=\"btn btn-success\" onclick=\"window.location.href=\'smarthome\'\">Go To Smart Home Now</button>\
        </div>\
      </div>\
    </body>\
    </html>",

    IP,
    mac
    );
  
  server.send(200, "text/html", html);
}


void handleConfig() {

  //strcpy(connect_ssid, server.arg("ip"));
  server.arg("ip").toCharArray(connect_ssid, server.arg("ip").length()+1);
  server.arg("pwd").toCharArray(connect_password, server.arg("pwd").length()+1);
  Serial.println(connect_ssid);
  //Serial.println(server.arg("ip").length());
  Serial.println(connect_password);

  //if ((server.arg("ip").length() > 0) && (server.arg("pwd").length() > 0)){
  if ((server.arg("ip").length() > 0)){
  
    Serial.print("Connecting to ");
    Serial.println(connect_ssid);
    WiFi.begin(connect_ssid, connect_password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    // Print local IP address and start web server
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    WiFi.localIP().toString().toCharArray(IP, 20);
     
    WiFi.softAPdisconnect(true);

    server.begin();
    Serial.println("HTTP Server Begin");
  }
  
  
  server.send(200, "text/html", 
  "<!DOCTYPE html>\
    <html lang=\"en\">\
    <head>\
      <meta charset=\"UTF-8\">\
      <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\
      <style>\
        body {font-family: Arial, Helvetica, sans-serif;}\
        * {box-sizing: border-box;}\
   \
        input[type=text], select, textarea {\
          width: 100%;\
          padding: 12px;\
          border: 1px solid #ccc;\
          border-radius: 4px;\
          box-sizing: border-box;\
          margin-top: 6px;\
          margin-bottom: 16px;\
          resize: vertical;\
        }\
        \
        input[type=password], select, textarea {\
          width: 100%;\
          padding: 12px;\
          border: 1px solid #ccc;\
          border-radius: 4px;\
          box-sizing: border-box;\
          margin-top: 6px;\
          margin-bottom: 16px;\
          resize: vertical;\
        }\
    \
        input[type=submit] {\
          background-color: #4CAF50;\
          color: white;\
          padding: 12px 20px;\
          border: none;\
          border-radius: 4px;\
          cursor: pointer;\
        }\
        \
        button[type=submit] {\
          background-color: #4CAF50;\
          color: white;\
          padding: 12px 20px;\
          border: none;\
          border-radius: 4px;\
          cursor: pointer;\
        }\
    \
        input[type=submit]:hover {\
          background-color: #45a049;\
        }\
    \
        .container {\
          border-radius: 5px;\
          background-color: #f2f2f2;\
          padding: 20px;\
        }\
      </style>\
      <title>My Smart Home</title>\
    </head>\
    <body>\
      <div class=\"container\">\
        <h2>Smart Config</h2>\
        <form action=\"\" method=\"get\">\
        <div class=\"row\">\
          <div class=\"form-group col-md-12\">\
            <label for=\"ip\">SSID:</label>\
            <input type=\"text\" class=\"form-control\" id=\"ip\" name=\"ip\">\
          </div>\
          <div class=\"form-group col-md-12\">\
            <label for=\"pwd\">Password:</label>\
            <input type=\"password\" class=\"form-control\" id=\"pwd\" name=\"pwd\">\
          </div>\
          <div class=\"form-group col-md-12\">\
            <label for=\"server_ip\">server's ip:</label>\
            <input type=\"text\" class=\"form-control\" id=\"server_ip\" name=\"server_ip\">\
          </div>\
          <div class=\"form-group col-md-12\">\
            <button type=\"submit\" class=\"btn btn-success\">Submit</button>\
          </div>\
        </div>\
        </form>\
      </div>\
    </body>\
    </html>"
  );
}

void handleSmartHome(){

  if (server.arg("device1").toInt() == 1){ // gpio14
    digitalWrite(GPIO14, !digitalRead(GPIO14));
    Serial.print(server.arg("device1"));
  }
  if (server.arg("device2").toInt() == 1){ // gpio12
    digitalWrite(GPIO12, !digitalRead(GPIO12));
    Serial.print(server.arg("device2"));
  }
  bool device1_state = false, device2_state = false;
  char device1Text[150], device2Text[150];
  
  device1_state = digitalRead(GPIO14);
  device2_state = digitalRead(GPIO12);

  if (device1_state){
    strcpy(device1Text," <button type=\"button\" onclick=\"window.location.href=\'/smarthome?device1=1\'\">Turn off device 1</button>");
  }
  else{
    strcpy(device1Text," <button type=\"button\" onclick=\"window.location.href=\'/smarthome?device1=1\'\">Turn on device 1</button>");
  }

  if (device2_state){
    strcpy(device2Text," <button type=\"button\" onclick=\"window.location.href=\'/smarthome?device2=1\'\">Turn off device 2</button>");
  }
  else{
    strcpy(device2Text," <button type=\"button\" onclick=\"window.location.href=\'/smarthome?device2=1\'\">Turn on device 2</button>");
  }

  char html[1000];

  snprintf(html, 1000, 
  "<!DOCTYPE html>\
    <html>\
    <head>\
      <meta charset=\"UTF-8\">\
      <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
      <style>\
        button {\
          background-color: #ad6ae8;\
          color: white;\
          padding: 12px 20px;\
          border: none;\
          border-radius: 4px;\
          cursor: pointer;\
        }\
        .container {\
          border-radius: 5px;\
          text-align: center;\
          padding: 20px;\
        }  \
        .device{\
          padding: 5px;\
        }\
      </style>\
      <title>My Smart Home</title>\
    </head>\  
    <body>\
      <div class=\"container\">\
        <h2>Smart Switch</h2>\
        <div class=\"device\">\
          %s\
        </div>\
        <div class=\"device\">\
          %s\
        </div>\
      </div>\
    </body>\
    </html>", 
    
    device1Text, 
    device2Text
    );
  
  server.send(200, "text/html", html);
  
}

void handleDevice1(){
  char html[10];
  bool state = false;
  state = digitalRead(GPIO14);
  sprintf(html, "%d", state);
  server.send(200, "text/html", html);
}

void handleDevice2(){
  char html[10];
  bool state = false;
  state = digitalRead(GPIO12);
  sprintf(html, "%d", state);
  server.send(200, "text/html", html);
}

void handleNotFound() {
  digitalWrite ( LED_BUILTIN, 0 );
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }

  server.send ( 404, "text/plain", message );
  digitalWrite ( LED_BUILTIN, 1 ); //turn the built in LED on pin DO of NodeMCU off
}

void setup() {
  pinMode(GPIO4, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  pinMode(GPIO5, OUTPUT);
  pinMode(GPIO12, OUTPUT);
  pinMode(GPIO14, OUTPUT);
  
  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");

  //set-up the custom IP address
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));   // subnet FF FF FF 00  
  
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  myIP.toString().toCharArray(IP, 20);
  
  WiFi.macAddress().toCharArray(mac, 20);

  server.on("/", handleRoot);      //Which routine to handle at root location
  server.on("/Config", handleConfig);
  server.on("/smarthome", handleSmartHome);
  server.on("/device1", handleDevice1);
  server.on("/device2", handleDevice2);

  server.onNotFound ( handleNotFound );
 
  server.begin();                  //Start server
  Serial.println("HTTP server started");
}

// the loop function runs over and over again forever
void loop() {
  server.handleClient();          //Handle client requests
}
