
#include <ESP8266WiFi.h>
#include <ESPSendMail.h>

#define WIFINAME "your_wifi_name"
#define WIFIPWD "your_wifi_password"
#define MAILSERVER "your_mailserver"
#define MAILUSERNAME "your_mailserver_username"
#define MAILUSERPWD "our_mailserver_password"


WiFiClientSecure client;



void setup() {
    Serial.begin(115200);
    delay(10);
 
    // Connect to WiFi network
    Serial.print("Connecting to ");
    Serial.println(WIFINAME);
  
    WiFi.begin(WIFINAME, WIFIPWD);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("My IP address: ");
    Serial.println(WiFi.localIP());
    delay(1000);
    
    ESPSendMail SendMail(MAILSERVER,MAILUSERNAME,MAILUSERPWD, &client);
    SendMail.From = "examples@gmail.com"; //put your address here
    SendMail.DisplayFrom = "From_Name"; //put display name here
    SendMail.To = "testtest@gmail.com"; //recipient address
    SendMail.Subject = "Test e-mail"; //subject
    SendMail.ClearMessage();
    SendMail.AddMessageLine("First line of message"); //lines of message
    SendMail.AddMessageLine("Second line");

    SendMail.Send();
}

void loop() {
  // nothing to do here
}
