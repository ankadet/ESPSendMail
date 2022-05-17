/*
	FILE: 		ESPSendMail.cpp
	VERSION: 	1.0.0
	PURPOSE: 	ESP8266 Library to send e-mail using SMTP protocol
	LICENCE:	MIT
 */
#include "ESPSendMail.h"

ESPSendMail::ESPSendMail(String _mailServer, String _mailUser, String _mailPwd, WiFiClientSecure *_clientsecure, int delaysec){
    mailServer = _mailServer;
	mailUser = _mailUser;
	mailPwd = _mailPwd;
	mailclient = _clientsecure;

    mailclient->setInsecure();
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


int ESPSendMail::Send()
{
  Serial.print("Attempting to connect to mail server:");
  Serial.println(mailServer);
  
  if (mailclient->connected()){
	   Serial.print("Client is just connected!!!!");
  }
  
  if (mailclient->connect(mailServer, 465) == 1) {
    Serial.println(F("Connected"));
  } else {
    Serial.print(F("Connection failed !!!"));
    return -1;
  }
  if (!Response())
    return 0;

  Serial.println(F("Sending Extended Hello"));
  mailclient->println("EHLO smtp2go.com");
  if (!Response())
    return 0;
 
  Serial.println(F("Sending AUTH LOGIN"));
  mailclient->println("AUTH LOGIN");
  if (!Response())
    return 0;

  Serial.println(F("Sending User"));
  mailclient->println(base64::encode(mailUser));
  if (!Response())
    return 0;

  Serial.println(F("Sending Password"));
  mailclient->println(base64::encode(mailPwd));
  if (!Response())
    return 0;

  Serial.println("Sending From <"+From+">");
  mailclient->println("MAIL FROM: <"+From+">");
  if (!Response())
    return 0;

  Serial.println("Sending To <"+To+">");
  mailclient->println("RCPT TO: <"+To+">");
  if (!Response())
    return 0;

  Serial.println(F("Sending DATA"));
  mailclient->println(F("DATA"));
  if (!Response())
    return 0;

  Serial.println(F("Sending email"));
  // recipient address (include option display name if you want)
  mailclient->println("To: "+To);

  // change to your address
  if (DisplayFrom != "")
    mailclient->println("From: "+DisplayFrom);

  mailclient->println("Subject:"+Subject+"\r\n");
  mailclient->println(Message);
  
  mailclient->println(F("."));
  if (!Response())
    return 0;

  Serial.println(F("Sending QUIT"));
  mailclient->println(F("QUIT"));
  if (!Response())
    return 0;

  mailclient->stop();
  Serial.println(F("Disconnected"));
  return 1;
}




byte ESPSendMail::Response(){
  int loopCount = 0;
  while (!mailclient->available()) {
    delay(1);
    loopCount++;
    if (loopCount > delaytime) {
      mailclient->stop();
      Serial.println(F("\r\nTimeout"));
      return 0;
    }
  }

  byte respCode = mailclient->peek();
  while (mailclient->available())
  {
    Serial.write(mailclient->read());
  }

  if (respCode >= '4')
  {
    Serial.print("Failed in eRcv with response: ");
    Serial.print(respCode);
    return 0;
  }

  return 1;
}