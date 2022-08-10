#include"controller.h"

#include <ESP8266WiFi.h>


#define BIT0_ON 0b00000001
#define BIT1_ON 0b00000010
#define BIT2_ON 0b00000100
#define BIT3_ON 0b00001000
#define BIT4_ON 0b00010000
#define BIT5_ON 0b00100000
#define BIT6_ON 0b01000000
#define BIT7_ON 0b10000000
#define ZEROS    0b00000000
#define ONES     0b11111111

#define BIT0_OFF 0b11111110
#define BIT1_OFF 0b11111101
#define BIT2_OFF 0b11111011
#define BIT3_OFF 0b11110111
#define BIT4_OFF 0b11101111
#define BIT5_OFF 0b11011111
#define BIT6_OFF 0b10111111
#define BIT7_OFF 0b01111111

#define D0 16
#define D1 5
#define D2 4
#define D3 0
#define D4 2
#define D5 14
#define D6 12
#define D7 13
#define D8 15

Controller::Controller()
{
	pinMode(D0, OUTPUT);//RESET
	pinMode(D1, OUTPUT);//B0
	pinMode(D2, OUTPUT);//B1
	pinMode(D3, OUTPUT);//B2
	pinMode(D4, OUTPUT);
	pinMode(D5, OUTPUT);//B3
	pinMode(D6, INPUT);
	pinMode(D7, INPUT);
	pinMode(D8, OUTPUT);
	reset_();
	send_word(0b00001111);
	delay(100);
}

void Controller::reset_()
{
	digitalWrite(D0, LOW);
	delay(100);
	digitalWrite(D0, HIGH);
	delay(100);
}

void Controller::control_thing(MESSAGES message, unsigned char address, unsigned char extra)
{
	send_word(0b00000000);
	while (digitalRead(D7) == LOW)yield();
	while (digitalRead(D7) == HIGH)yield();
	send_word(message);
	delay(30);
	send_word(address & 0b00001111);
	delay(30);
	send_word((address & 0b11110000) >> 4);
	delay(30);
	send_word(extra & 0b00001111);
	delay(30);
	send_word((extra & 0b11110000) >> 4);
	delay(30);
	send_word(0b00001111);
}

void Controller::send_word(unsigned char w)
{
	digitalWrite(D1, (w & BIT0_ON));
	digitalWrite(D2, (w & BIT1_ON) >> 1);
	digitalWrite(D3, (w & BIT2_ON) >> 2);
	digitalWrite(D5, (w & BIT3_ON) >> 3);
}
