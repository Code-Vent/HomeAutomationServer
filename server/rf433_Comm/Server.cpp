#include"defs.h"
#include"16f628a.h"

#define SHUT_DOWN_TRANSMITTER OFF_RB6

char error, udata, i, j;
unsigned char my_address = 0;
unsigned char header = 83;
unsigned char last_message_id = 0;
unsigned char message;
unsigned char recipient_address;
unsigned char recipient_address_hi;
unsigned char recipient_address_lo;
unsigned char extra;
unsigned char extra_hi;
unsigned char extra_lo;
unsigned char result = 1;

enum MESSAGES {
	on = 0, off, toggle, flash, up_level, down_level,
	refresh_node, send_state, feedback, trigger, new_node
};

struct Message_Info
{
	unsigned char message_header;
	unsigned char message_id;
	enum MESSAGES message;
	unsigned char from;
	unsigned char to;
	signed char bounce;
	unsigned char extra;
}msg_info;

void null_func();
void Serve_Message();
void(*callback)();
void process_message(void);
void take_action(void);
void Header_Init(void);
unsigned char read_wifiPort();

void begin_wifiCom_protocol()
{
	Delay_ms(10);
	ON_RB6;
	Delay_ms(10);
	OFF_RB6;
	Delay_ms(15);
	message = read_wifiPort();
	Delay_ms(30);
	recipient_address_lo = read_wifiPort();
	Delay_ms(30);
	recipient_address_hi = read_wifiPort();
	Delay_ms(30);
	extra_lo = read_wifiPort();
	Delay_ms(30);
	extra_hi = read_wifiPort();

	recipient_address = (recipient_address_hi << 4) | recipient_address_lo;
	extra = (extra_hi << 4) | extra_lo;
}

unsigned char read_wifiPort()
{
	return ((PORTB & 0b00011110) >> 1);
}

void null_func()
{
	return;
}

void Header_Init(void)
{
	header = PORTA;
}

void System_Init()
{
	TRISB = BIT6_OFF & BIT5_OFF;
	TRISA = ~ZERO;
	CMCON = BIT0_ON | BIT1_ON | BIT2_ON;
	OPTION_REG = BIT3_ON | BIT2_ON | BIT1_ON | BIT0_ON; // Pre-Scaler for WDT 1:128
	INTCON = ZERO;
	PORTB = ZERO;
}

void send_preamble()
{
	Soft_UART_Write(0xFE);
	Soft_UART_Write(0xFE);
	Soft_UART_Write(0xFE);
	Soft_UART_Write(0xFB);
}

void set_message()
{
	msg_info.message = message;
	msg_info.to = recipient_address;
	msg_info.from = my_address;
	++msg_info.message_id;
	msg_info.bounce = 1;
	if (msg_info.message == new_node)
	{
		msg_info.message_header = 45;
		msg_info.message_id = header;
	}
	else
	{
		msg_info.message_header = header;
	}
	msg_info.extra = extra;
	result = 1;
}

void broadcast_message(char* s, unsigned char len)
{
	for (i = 0, j = 0;i < 3;++i)
	{
		send_preamble();//4 bytes
		Delay_ms(1);
		send_preamble();//4 bytes
		Delay_ms(1);
		Soft_UART_Write('*');
		while (j++ < len)
		{
			Delay_ms(1);
			Soft_UART_Write(*s++);
		}
		Delay_ms(1);
	}
	SHUT_DOWN_TRANSMITTER;
}

void get_message(char* s, unsigned char len)
{
	SHUT_DOWN_TRANSMITTER;
	i = 0;
	while (i < len)
	{
		udata = Soft_UART_Read(&error);
		*s++ = udata;
		++i;
	}
	return;
}

void Serve_Message()
{
	set_message();
	broadcast_message((char*)&msg_info, sizeof(msg_info));
	callback = null_func;
	return;
}

void take_action()
{
	if (msg_info.message == feedback)
	{
		if (last_message_id != msg_info.message_id)
		{
			result = 0;
			last_message_id = msg_info.message_id;
			//TOGGLE_RA0;
		}
	}
}

void process_message(void)
{
	if (my_address == msg_info.to)
	{
		take_action();
	}
}

void main(void)
{
	System_Init();
	Header_Init();
	error = Soft_UART_Init(&PORTB, 7, 6, 1200, 0);
	callback = null_func;
	udata = 0;
	Delay_ms(100);
	SHUT_DOWN_TRANSMITTER;
	while (1)
	{
		udata = Soft_UART_Read(&error);
		switch (udata)
		{
		case '*':
			get_message((char*)&msg_info, sizeof(msg_info));
			if (msg_info.message_header == header)
			{
				process_message();
			}
			break;
		default:
			udata = read_wifiPort();
			if (udata == ZERO)
			{
				begin_wifiCom_protocol();
				callback = Serve_Message;
			}
			else if (result == 0 && udata == 0b00001111)
			{
				ON_RB5;
			}
			else if (result == 1)
			{
				OFF_RB5;
			}
			callback();
			break;
		}
		asm CLRWDT;
	}
}