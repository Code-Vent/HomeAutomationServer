#include"bluewire.h"
#include"localserver.h"
#include"controller.h"


namespace global
{
	Home home_;
	Controller controller_;
	String content;
	unsigned char current_dir_id;
	unsigned char choice;
	unsigned char address;
	char uri_head[] = "<p align=\"center\"><a href=\"http\://bluewire.local/xyz?id=";
	char uri_head_[] = "\">";
	char uri_tail[] = "</a></p>";
}

#define X_URI_INDEX 49
#define Y_URI_INDEX 50
#define Z_URI_INDEX 51

enum ContentType
{
	TEXT_HTML
};

char* ContentTypeString[] = {
	"text/html"
};


/* hostname for mDNS. Should work at least on windows. Try http://bluewire.local */
const char *myHostname = "bluewire";
const char* Bluewire::type;
const char* Bluewire::content;
char* Bluewire::urls[] = { "/", "/dir","/opt", "/ope", "/opr", "/act" };
void(*Bluewire::handlers[])() = { handleRoot, handleDirectory, handleOptions, handleOperator, handleOperand, handleAction };
bool Bluewire::client_ = false;
LocalServer* Bluewire::server = 0;
TpMemFun Bluewire::callback = &LocalServer::getParamValue;
//State* Bluewire::bluewire_states[] = { &directoryStateObj, &actionStateObj, 0 };

RootState rootStateObj;
ActionState actionStateObj;
DirectoryState directoryStateObj;


const char* action_option_names[] = {
	"On",
	"Off",
	"Toggle",
	"Flash",
	"Increase Level",
	"Decrease Level"
};

const char* operator_names[] = {
	"Add",
	"Remove"
};

void Bluewire::handleRoot() {
	rootStateObj._Process(NONE);
	Bluewire::type = ContentTypeString[ContentType::TEXT_HTML];
	Bluewire::content = global::content.c_str();
	client_ = true;
}

void Bluewire::handleDirectory()
{
	unsigned char id = (server->*callback)();
	global::current_dir_id = id;
	directoryStateObj._Process(GEN_LIST);
	Bluewire::type = ContentTypeString[ContentType::TEXT_HTML];
	Bluewire::content = global::content.c_str();
	client_ = true;
}

void Bluewire::handleOptions()
{
	int opt = (server->*callback)();
	global::choice = opt;
	directoryStateObj._Process(CHOICE);
	Bluewire::type = ContentTypeString[ContentType::TEXT_HTML];
	Bluewire::content = global::content.c_str();
	client_ = true;
}

void Bluewire::handleOperator()
{
	int id = (server->*callback)();

	switch (id)
	{
	case Bluewire::OperatorType::ADD:
		directoryStateObj._Process(ADD_TO);
		break;
	case Bluewire::OperatorType::REMOVE:
		directoryStateObj._Process(REMOVE_FROM);
		break;
	}
	Bluewire::type = ContentTypeString[ContentType::TEXT_HTML];
	Bluewire::content = global::content.c_str();
	client_ = true;
}

void Bluewire::handleOperand()
{
	int id = (server->*callback)();
	global::address = id;
	actionStateObj._Process(OPERAND);
	Bluewire::type = ContentTypeString[ContentType::TEXT_HTML];
	Bluewire::content = global::content.c_str();
	client_ = true;
}

void Bluewire::handleAction()
{
	int opt = (server->*callback)();
	global::choice = opt;
	actionStateObj._Process(ACTION);
	Bluewire::type = ContentTypeString[ContentType::TEXT_HTML];
	Bluewire::content = global::content.c_str();
	client_ = true;
}

const char * Bluewire::getmyHostname()
{
	return myHostname;
}

const char * Bluewire::getAccesspointID()
{
	return ::getAccesspointID();
}

const char * Bluewire::getAccesspointPassword()
{
	return ::getAccesspointPassword();
}

int roomList()
{
	Room* room;
	int i = 0;
	global::home_.NextRoom(true);
	while (room = global::home_.NextRoom())
	{
		global::content += global::uri_head;
		global::content += room->ID;
		global::content += global::uri_head_;
		global::content += roomToText(room->type);
		global::content += global::uri_tail;
		++i;
	}
	return i;
}

int thingList(Room* room)
{
	Thing* thing;
	int i;
	room->NextThing(true);
	while (thing = room->NextThing())
	{
		global::content += global::uri_head;
		global::content += thing->address;
		global::content += global::uri_head_;
		global::content += thingToText(thing->type);
		global::content += global::uri_tail;
		++i;
	}
	return i;
}

void operatorList()
{
	global::content += global::uri_head;
	global::content += Bluewire::OperatorType::ADD;
	global::content += global::uri_head_;
	global::content += operator_names[Bluewire::OperatorType::ADD];
	global::content += global::uri_tail;

	global::content += global::uri_head;
	global::content += Bluewire::OperatorType::REMOVE;
	global::content += global::uri_head_;
	global::content += operator_names[Bluewire::OperatorType::REMOVE];
	global::content += global::uri_tail;
}

void roomOptionsList()
{
	for (int i = RoomTypes::KITCHEN; i < RoomTypes::ROOM_TYPES_NUM; ++i)
	{
		global::content += global::uri_head;
		global::content += i;
		global::content += global::uri_head_;
		global::content += roomToText((RoomTypes)i);
		global::content += global::uri_tail;
	}
}

void thingOptionsList()
{
	for (int i = ThingTypes::SOCKET; i < ThingTypes::THING_TYPES_NUM; ++i)
	{
		global::content += global::uri_head;
		global::content += i;
		global::content += global::uri_head_;
		global::content += thingToText((ThingTypes)i);
		global::content += global::uri_tail;
	}
}

void actionsList()
{
	for (int i = Bluewire::ActionType::ON; i < Bluewire::ActionType::NUM_OF_ACTION_TYPES; ++i)
	{
		global::content += global::uri_head;
		global::content += i;
		global::content += global::uri_head_;
		global::content += action_option_names[i];
		global::content += global::uri_tail;
	}
}

void emptyListExceptionHandler()
{
	global::content = "<h2 align=\"center\">Empty List!</h2>";
}

void operationFeedbackHandler()
{
	global::content = "<h2 align=\"center\">Your request has been processed.</h2>";
}

unsigned char gen_curr_directory()
{
	Room* room;
	unsigned char num = 0;

	if (global::current_dir_id == Bluewire::HOME)
	{
		global::uri_head[X_URI_INDEX] = 'd';
		global::uri_head[Y_URI_INDEX] = 'i';
		global::uri_head[Z_URI_INDEX] = 'r';
		num = roomList();
	}
	else
	{
		room = FindRoom(global::home_, global::current_dir_id);
		if (room)
		{
			global::uri_head[X_URI_INDEX] = 'o';
			global::uri_head[Y_URI_INDEX] = 'p';
			global::uri_head[Z_URI_INDEX] = 'r';
			num = thingList(room);
		}
	}

	if (!num)
	{
		emptyListExceptionHandler();
	}
	global::uri_head[X_URI_INDEX] = 'o';
	global::uri_head[Y_URI_INDEX] = 'p';
	global::uri_head[Z_URI_INDEX] = 'e';
	operatorList();
	return num;
}

void DirectoryState::_Process(StateInput input)
{
	Room* room;
	unsigned char num = 0;
	global::content = "";

	switch (input)
	{
	case GEN_LIST:
		num = gen_curr_directory();
		next = GEN_LIST;
		prev = GEN_LIST;
		break;
	case ADD_TO:
		if (prev != GEN_LIST)
			return;
		global::uri_head[X_URI_INDEX] = 'o';
		global::uri_head[Y_URI_INDEX] = 'p';
		global::uri_head[Z_URI_INDEX] = 't';
		if (global::current_dir_id == Bluewire::HOME)
		{
			roomOptionsList();
		}
		else
		{
			thingOptionsList();
		}
		next = CHOICE;
		prev = ADD_TO;
		break;
	case REMOVE_FROM:
		if (prev != GEN_LIST)
			return;
		global::uri_head[X_URI_INDEX] = 'o';
		global::uri_head[Y_URI_INDEX] = 'p';
		global::uri_head[Z_URI_INDEX] = 't';
		if (global::current_dir_id == Bluewire::HOME)
		{
			num = roomList();
		}
		else
		{
			room = FindRoom(global::home_, global::current_dir_id);
			if (room)
			{
				num = thingList(room);
			}
		}
		if (!num)
		{
			emptyListExceptionHandler();
		}
		next = CHOICE;
		prev = REMOVE_FROM;
		break;
	case CHOICE:
		if (next != CHOICE)
			return;
		if (global::current_dir_id == Bluewire::HOME)
		{
			if (prev == REMOVE_FROM)
				RemoveRoom(global::home_, global::choice);
			else
				AddRoom(global::home_, (RoomTypes)global::choice);
		}
		else
		{
			if (prev == REMOVE_FROM)
				RemoveThing(global::home_, global::current_dir_id, global::choice);
			else
			{
				if ((ThingTypes)global::choice != TRIGGER)
				{
					num = AddThing(global::home_, global::current_dir_id, (ThingTypes)global::choice);
					if (num)
					{
						global::controller_.control_thing(Controller::MESSAGES::new_node,
							80, num);
						delay(300);
						global::controller_.control_thing(Controller::MESSAGES::trigger,
							num, global::current_dir_id);
					}
				}
				else
				{
					global::controller_.control_thing(Controller::MESSAGES::feedback,
						255, global::current_dir_id);
				}
			}

		}
		gen_curr_directory();
		next = GEN_LIST;
		prev = CHOICE;
		break;
	default:
		return;
	}

	return;
}

void ActionState::_Process(StateInput input)
{
	Room* room;
	int items = 0;
	global::content = "";

	switch (input)
	{
	case OPERAND:
		global::uri_head[X_URI_INDEX] = 'a';
		global::uri_head[Y_URI_INDEX] = 'c';
		global::uri_head[Z_URI_INDEX] = 't';
		actionsList();
		prev = OPERAND;
		next = ACTION;
		break;
	case ACTION:
		if (next != ACTION)
			return;
		if (prev == OPERAND)
		{
			switch (global::choice)
			{
			case Bluewire::ActionType::ON:
				global::controller_.control_thing(Controller::MESSAGES::on_,
					global::address, 0);
				break;
			case Bluewire::ActionType::OFF:
				global::controller_.control_thing(Controller::MESSAGES::off,
					global::address, 0);
				break;
			case Bluewire::ActionType::TOGGLE:
				global::controller_.control_thing(Controller::MESSAGES::toggle,
					global::address, 0);
				break;
			case Bluewire::ActionType::FLASH:
				global::controller_.control_thing(Controller::MESSAGES::flash,
					global::address, 0);
				break;
			case Bluewire::ActionType::INCREAMENT:
				global::controller_.control_thing(Controller::MESSAGES::up_level,
					global::address, 0);
				break;
			case Bluewire::ActionType::DECREAMENT:
				global::controller_.control_thing(Controller::MESSAGES::down_level,
					global::address, 0);
				break;
			default:
				break;
			}
		}
		gen_curr_directory();
		next = OPERAND;
		prev = ACTION;
		break;
	default:
		return;
	}
	return;
}

void RootState::_Process(StateInput)
{
	global::content = "<h1 align=\"center\">You are welcome to Bluewire</h1>";
	global::uri_head[X_URI_INDEX] = 'd';
	global::uri_head[Y_URI_INDEX] = 'i';
	global::uri_head[Z_URI_INDEX] = 'r';
	global::content += global::uri_head;
	global::content += 0;
	global::content += global::uri_head_;
	global::content += "Rooms";
	global::content += global::uri_tail;
	return;
}