#include"localserver.h"


LocalServer::LocalServer(ESP8266WebServer* pserver)
	:_pserver(pserver)
{
	Bluewire::server = this;
}

const char * LocalServer::getHostName()
{
	return Bluewire::getmyHostname();
}

const char * LocalServer::getAccesspointID()
{
	return Bluewire::getAccesspointID();
}

const char * LocalServer::getAccesspointPassword()
{
	return Bluewire::getAccesspointPassword();
}

void LocalServer::setUrls()
{
	for (int i = 0;i < Bluewire::NUM;++i)
	{
		_pserver->on(Bluewire::urls[i], Bluewire::handlers[i]);
	}
}

void LocalServer::reply()
{
	if (Bluewire::client_)
	{
		_pserver->send(400, Bluewire::type,
			Bluewire::content);
		Bluewire::client_ = false;
	}
}

int LocalServer::getParamValue()
{
	if (_pserver->args() != 0)
		return _pserver->arg(0).toInt();
	return 0;
}
