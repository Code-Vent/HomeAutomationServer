#ifndef LOCALSERVER_H
#define LOCALSERVER_H

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ESP8266mDNS.h>
#include"bluewire.h"

class LocalServer
{
public:
	LocalServer(ESP8266WebServer* pserver);
	const char* getHostName();
	const char* getAccesspointID();
	const char* getAccesspointPassword();
	void setUrls();
	void reply();
	int getParamValue();
private:
	ESP8266WebServer* _pserver;
};

#endif
