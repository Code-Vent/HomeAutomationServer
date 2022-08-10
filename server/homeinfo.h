#ifndef HOMEINFO_H
#define HOMEINFO_H

//#include <ESP8266WiFi.h>


enum ThingTypes {
	UNINSTALLED,
	SOCKET,
	LIGHT,
	FAN,
	DIMMER,
	TRIGGER,
	THING_TYPES_NUM
};

enum ThingState
{
	LOW_,
	HIGH_,
	FUZZY
};

struct Thing
{
	ThingTypes type;
	unsigned char address;
	ThingState state;
};

enum RoomTypes
{
	UNKNOWN_,
	KITCHEN,
	LIVING_ROOM,
	BEDROOM,
	RESTROOM,
	OUTDOOR,
	ROOM_TYPES_NUM
};

struct Room
{
	enum { SIZE = 20 };
	RoomTypes type;
	unsigned char ID;
	Thing things[SIZE];
	unsigned char number_of_things;
	Thing* NextThing(bool reset_ = false);
};

/*enum HomeOperationTypes
{
ADD_ROOM,
REMOVE_ROOM,
HOME_OP_TYPES_NUM
};*/

/*const HomeOperationTypes home_operations[]{
ADD_ROOM, REMOVE_ROOM };*/

struct Home
{
	friend Room* FindRoom(Home&, unsigned char);
	friend Room* FindFreeRoom(Home&);
	friend Thing* FindFreeThing(Room&);
	friend void HomeOperationHelper(Room* room, unsigned char id,
		RoomTypes type);
	friend unsigned char AddRoom(Home&, RoomTypes);
	friend void RemoveRoom(Home&, unsigned char id);
	friend unsigned char AddThing(Home&, unsigned char room_id, ThingTypes);
	friend void RemoveThing(Home&, unsigned char room_id, unsigned char thing_address);
	enum { SIZE = 20 };

	Home();
	Room* NextRoom(bool reset_ = false);
private:
	static unsigned char next_id;
	static unsigned char next_address;
	Room rooms[SIZE];
	unsigned char number_of_rooms;
};

Room* FindRoom(Home&, unsigned char);
Room* FindFreeRoom(Home&);
Thing* FindFreeThing(Room&);
void HomeOperationHelper(Room* room, unsigned char id,
	RoomTypes type = UNKNOWN_);
unsigned char AddRoom(Home&, RoomTypes);
void RemoveRoom(Home&, unsigned char id);
unsigned char AddThing(Home&, unsigned char room_id, ThingTypes);
void RemoveThing(Home&, unsigned char room_id, unsigned char thing_address);



const char* thingToText(ThingTypes t);
const char* roomToText(RoomTypes t);


#endif