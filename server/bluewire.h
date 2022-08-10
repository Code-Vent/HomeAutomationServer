#ifndef BLUEWIRE_H
#define BLUEWIRE_H

#include"homeinfo.h"
#include"credentials.h"

class LocalServer;
typedef int (LocalServer::*TpMemFun)();

enum StateInput { NONE, ADD_TO, REMOVE_FROM, GEN_LIST, CHOICE, OPERAND, ACTION };

class State
{
public:
	virtual void _Process(StateInput) = 0;
protected:
	StateInput next;
	StateInput prev;
};

class RootState : public State
{
public:
	virtual void _Process(StateInput);
};

class ActionState : public State
{
public:
	virtual void _Process(StateInput);
};

class DirectoryState : public State
{
public:
	virtual void _Process(StateInput);
};


class Bluewire
{
public:
	enum { NUM = 6 };
	enum DirectoryType { HOME, ROOM };
	enum OperatorType { ADD, REMOVE };
	enum ActionType {
		ON, OFF, TOGGLE, FLASH,
		INCREAMENT, DECREAMENT, NUM_OF_ACTION_TYPES
	};
	enum OperandType { EMPTY, DIRECTORIES, THINGS };
	enum UriTypes { DIR = 1, OPT, OPE, OPR, ACT };

	static void handleRoot();
	static void handleDirectory();
	static void handleOptions();
	static void handleOperator();
	static void handleOperand();
	static void handleAction();
	static const char* type;
	static const char* content;
	static char* urls[NUM];
	static void(*handlers[NUM])();

	static const char* getmyHostname();
	static const char* getAccesspointID();
	static const char* getAccesspointPassword();
	static bool client_;
	static LocalServer* server;
	static TpMemFun callback;
};

#endif