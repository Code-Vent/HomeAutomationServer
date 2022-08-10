#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ESP8266mDNS.h>
#include"localserver.h"

// DNS server
const byte DNS_PORT = 53;
DNSServer dnsServer;

ESP8266WebServer server(80);

LocalServer localserver(&server);

/* Soft AP network parameters */
IPAddress apIP(192, 168, 4, 1);
IPAddress netMsk(255, 255, 255, 0);

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
connected to this access point to see it.
*/


void handleNotFound() {
	digitalWrite(2, 1);
	String message = "File Not Found\n\n";
	message += "URI: ";
	message += server.uri();
	message += "\nMethod: ";
	message += (server.method() == HTTP_GET) ? "GET" : "POST";
	message += "\nArguments: ";
	message += server.args();
	message += "\n";
	for (uint8_t i = 0; i < server.args(); i++) {
		message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
	}
	server.send(404, "text/plain", message);
	digitalWrite(2, 0);
}

void setup() {
	delay(1000);
	Serial.begin(115200);
	Serial.println();
	Serial.print("Configuring access point...");
	WiFi.softAPConfig(apIP, apIP, netMsk);
	/* You can remove the password parameter if you want the AP to be open. */
	WiFi.softAP(localserver.getAccesspointID(), localserver.getAccesspointPassword());

	IPAddress myIP = WiFi.softAPIP();

	Serial.print("AP IP address: ");
	Serial.println(myIP);

	/* Setup the DNS server redirecting all the domains to the apIP */
	dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
	dnsServer.start(DNS_PORT, "*", apIP);

	if (MDNS.begin(localserver.getHostName())) {
		Serial.println("MDNS responder started");
		MDNS.addService("http", "tcp", 80);
	}

	localserver.setUrls();
	server.onNotFound(handleNotFound);
	server.begin();
	Serial.println("HTTP server started");
}

void loop() {
	dnsServer.processNextRequest();
	server.handleClient();
	MDNS.update();
	localserver.reply();
}
