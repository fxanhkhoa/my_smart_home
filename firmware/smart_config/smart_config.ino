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

//==============================================================
//     This rutine is exicuted when you open its IP in browser
//==============================================================
void handleRoot(){
  server.send(200, "text/html",
  "<!DOCTYPE html>\
    <html>\
    <head>\
      <meta charset=\"UTF-8\">\
      <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
      <link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css\" >\
      <link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap-theme.min.css\">\
      <script src=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js\" ></script>\
      <title>My Smart Home</title>\
    </head>\  
    <body>\
      <div class=\"container\">\
        <h2>Smart Switch</h2>\
        <div class=\"row\">\
          <button type=\"button\" class=\"btn btn-warning\" onclick=\"window.location.href=\'Config\'\">Config Wifi</button>\
        </div>\
        <div class=\"row\">\
          <button type=\"button\" class=\"btn btn-success\" onclick=\"window.location.href=\'smarthome\'\">Go To Smart Home Now</button>\
        </div>\
      </div>\
    </body>\
    </html>"
  );
}


void handleConfig() {

  //strcpy(connect_ssid, server.arg("ip"));
  server.arg("ip").toCharArray(connect_ssid, server.arg("ip").length()+1);
  server.arg("pwd").toCharArray(connect_password, server.arg("pwd").length()+1);
  Serial.println(connect_ssid);
  //Serial.println(server.arg("ip").length());
  Serial.println(connect_password);

  if ((server.arg("ip").length() > 0) && (server.arg("pwd").length() > 0)){
  
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
      <link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css\">\
      <link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap-theme.min.css\">\
      <script src=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js\"></script>\
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
  
  server.send(200, "text/html",
  "<!DOCTYPE html>\
    <html>\
    <head>\
      <meta charset=\"UTF-8\">\
      <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
      <link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css\" >\
      <link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap-theme.min.css\">\
      <script src=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js\" ></script>\
      <title>My Smart Home</title>\
    </head>\  
    <body>\
      <div class=\"container\">\
        <h2>Smart Switch</h2>\
          <button type=\"button\" class=\"btn btn-warning\" onclick=\"window.location.href=\'/smarthome?device1=1\'\">Device 1</button>\
          <button type=\"button\" class=\"btn btn-success\" onclick=\"window.location.href=\'/smarthome?device2=1\'\">Device 2</button>\
      </div>\
    </body>\
    </html>"
  );
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

  server.on("/", handleRoot);      //Which routine to handle at root location
  server.on("/Config", handleConfig);
  server.on("/smarthome", handleSmartHome);
 
  server.begin();                  //Start server
  Serial.println("HTTP server started");
}

// the loop function runs over and over again forever
void loop() {
  server.handleClient();          //Handle client requests
}
