#include"homeinfo.h"
//#include<memory.h>

using namespace std;

const char* thing_names[] = {
	"Uninstalled",
	"Socket",
	"Light",
	"Fan",
	"Dimmer",
	"Trigger"
};

const char* room_names[] = {
	"Unknown",
	"Kitchen",
	"LivingRoom",
	"Bedroom",
	"Restroom",
	"Outdoor",
};

const char* get_name_of_thing(Thing t)
{
	return thing_names[t.type];
}

unsigned char Home::next_id = 1;
unsigned char Home::next_address = 1;

Room* FindRoom(Home& home_, unsigned char id)
{
	unsigned char i = 0;
	Room* room;
	home_.NextRoom(true);
	while (room = home_.NextRoom())
	{
		if (room->ID == id)
			return room;
	}
	return 0;
}

Thing* FindThing(Room& room, unsigned char address)
{
	unsigned char i = 0;
	Thing* thing;

	room.NextThing(true);
	while (thing = room.NextThing())
	{
		if (thing->address == address)
			return thing;
	}
	return 0;
}

void HomeOperationHelper(Room* room, unsigned char id,
	RoomTypes type)
{
	room->type = type;
	room->ID = id;
}

unsigned char AddRoom(Home& home_, RoomTypes type)
{
	Room* room = FindFreeRoom(home_);
	if (room)
	{
		HomeOperationHelper(room, Home::next_id++, type);
		//Update the database
		++home_.number_of_rooms;
		return (Home::next_id - 1);
	}
	return 0;
}

void RemoveRoom(Home& home_, unsigned char id)
{
	Room* room = FindRoom(home_, id);
	if (room)
	{
		HomeOperationHelper(room, id);
		--home_.number_of_rooms;
	}
}

void RoomOperationHelper(Thing* thing, unsigned char address,
	ThingTypes type = UNINSTALLED)
{

	thing->address = address;
	thing->state = ThingState::LOW_;
	thing->type = type;
}

unsigned char AddThing(Home& home_, unsigned char id, ThingTypes type)
{
	Room* room = FindRoom(home_, id);
	if (room)
	{
		Thing* thing = FindFreeThing(*room);
		if (thing)
		{
			RoomOperationHelper(thing, Home::next_address++, type);
			//Update the database
			++room->number_of_things;
			return (Home::next_address - 1);
		}
	}
	return 0;
}

void RemoveThing(Home& home_, unsigned char id, unsigned char address)
{
	Room* room = FindRoom(home_, id);
	if (room)
	{
		Thing* thing = FindThing(*room, address);
		if (thing)
		{
			RoomOperationHelper(thing, 0);
			//Update the database
			--room->number_of_things;
		}
	}
}

const char* thingToText(ThingTypes t)
{
	return thing_names[t];
}

const char* roomToText(RoomTypes t)
{
	return room_names[t];
}

Home::Home()
{
	char* ch = (char*)rooms;
	//memset(rooms, 0, sizeof(rooms));
	for (int i = 0; i < sizeof(rooms); ++i)
		ch[i] = 0;
	number_of_rooms = 0;
}

Room* Home::NextRoom(bool reset_)
{
	static int curr = 0;
	static int index = 0;

	if (number_of_rooms == 0)
		return 0;

	if (reset_)
	{
		curr = 0;
		index = 0;
		return 0;
	}

	if (curr == number_of_rooms)
	{
		index = 0;
		curr = 0;
		return 0;
	}

	while (rooms[index++].type == UNKNOWN_) {}
	++curr;
	return &rooms[index - 1];
}

Thing* Room::NextThing(bool reset_)
{
	static int curr = 0;
	static int index = 0;

	if (number_of_things == 0)
		return 0;

	if (reset_)
	{
		curr = 0;
		index = 0;
		return 0;
	}

	if (curr == number_of_things)
	{
		index = 0;
		curr = 0;
		return 0;
	}

	while (things[index++].type == UNINSTALLED) {}
	++curr;
	return &things[index - 1];
}

Room* FindFreeRoom(Home& home_)
{
	Room* room = home_.rooms;
	if (home_.number_of_rooms < Home::SIZE)
	{

		while (room->type != UNKNOWN_)
			++room;
		return room;
	}
	return 0;
}

Thing* FindFreeThing(Room& room)
{
	Thing* thing = room.things;
	if (room.number_of_things < Room::SIZE)
	{

		while (thing->type != UNINSTALLED)
			++thing;
		return thing;
	}
	return 0;
}