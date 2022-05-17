/*
	FILE: 		ESPSendMail.h
	VERSION: 	1.0.0
	PURPOSE: 	ESP8266 Library to send e-mail using SMTP protocol
	LICENCE:	MIT
 */
#ifndef _ESPSENDMAIL_H_
#define _ESPSENDMAIL_H_
#include <base64.h>
#include <ESP8266WiFi.h>

class ESPSendMail{
	public:
		ESPSendMail(String _mailServer, String _mailUser, String _mailPwd, WiFiClientSecure *_client, int delaysec=10);
		int Send();
		void ClearMessage();
		void AddMessageLine(String _line);
		String From;
		String To;
		String Subject;
		String DisplayFrom;
		
	private:
		String mailServer;
		String Message;
		String mailUser;
		String mailPwd;
		WiFiClientSecure *mailclient;
		unsigned long delaytime;
		byte Response();

};


#endif