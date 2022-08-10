#include"credentials.h"

#ifndef APSSID
#define APSSID "ESPap"
#define APPSK  "thereisnospoon"
#endif

const char* getAccesspointID()
{
	return APSSID;
}

const char* getAccesspointPassword()
{
	return APPSK;
}
