/*
	FILE: 		ESPSendMail.cpp
	VERSION: 	1.0.0
	PURPOSE: 	ESP8266 Library to send e-mail using SMTP protocol
	LICENCE:	MIT
 */
#include "ESPSendMail.h"

ESPSendMail::ESPSendMail(String _mailServer, String _mailUser, String _mailPwd, WiFiClientSecure *_client, int delaysec){
    mailServer = _mailServer;
	mailUser = _mailUser;
	mailPwd = _mailPwd;
	client = _client;

    client->setInsecure();
    From = "";
    To = "";
    Subject = "";
    Message = "";
    DisplayFrom = "";
    delaytime = delaysec*1000;

}



void ESPSendMail::ClearMessage(){
    Message = "";
}
		
void ESPSendMail::AddMessageLine(String _line){
    if (Message==""){
        Message=_line;
    }else{
        Message += "\r\n"+_line;
    }
}


byte ESPSendMail::Send()
{
  Serial.println("Attempting to connect to mail server");
  if (client->connect(mailServer, 465) == 1) {
    Serial.println(F("Connected"));
  } else {
    Serial.print(F("Connection failed:"));
    return 0;
  }
  if (!Response())
    return 0;

  Serial.println(F("Sending Extended Hello"));
  client->println("EHLO smtp2go.com");
  if (!Response())
    return 0;
 
  Serial.println(F("Sending AUTH LOGIN"));
  client->println("AUTH LOGIN");
  if (!Response())
    return 0;

  Serial.println(F("Sending User"));
  client->println(base64::encode(mailUser));
  if (!Response())
    return 0;

  Serial.println(F("Sending Password"));
  client->println(base64::encode(mailPwd));
  if (!Response())
    return 0;

  Serial.println("Sending From <"+From+">");
  client->println("MAIL FROM: <"+From+">");
  if (!Response())
    return 0;

  Serial.println("Sending To <"+To+">");
  client->println("RCPT TO: <"+To+">");
  if (!Response())
    return 0;

  Serial.println(F("Sending DATA"));
  client->println(F("DATA"));
  if (!Response())
    return 0;

  Serial.println(F("Sending email"));
  // recipient address (include option display name if you want)
  client->println("To: "+To);

  // change to your address
  if (DisplayFrom != "")
    client->println("From: "+DisplayFrom);

  client->println("Subject:"+Subject+"\r\n");
  client->println(Message);
  
  client->println(F("."));
  if (!Response())
    return 0;

  Serial.println(F("Sending QUIT"));
  client->println(F("QUIT"));
  if (!Response())
    return 0;

  client->stop();
  Serial.println(F("Disconnected"));
  return 1;
}




byte ESPSendMail::Response(){
  int loopCount = 0;
  while (!client->available()) {
    delay(1);
    loopCount++;
    if (loopCount > delaytime) {
      client->stop();
      Serial.println(F("\r\nTimeout"));
      return 0;
    }
  }

  byte respCode = client->peek();
  while (client->available())
  {
    Serial.write(client->read());
  }

  if (respCode >= '4')
  {
    Serial.print("Failed in eRcv with response: ");
    Serial.print(respCode);
    return 0;
  }

  return 1;
}