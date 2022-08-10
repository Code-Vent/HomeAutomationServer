#include"defs.h"
//#include"12f683.h"

#define SHUT_DOWN_TRANSMITTER OFF_GP1;

char error, udata, i, j;
const unsigned char trigger_address = 255;
//unsigned char return_address = 0;
unsigned char header = 45;
unsigned char last_message_id = 0;
unsigned char trigger_ID = 0;

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

void on_action();
void off_action();
void toggle_action();
void flash_action();
void null_func();
void save_triggerID();
void read_triggerID();

void(*callback)();
void process_message(void);
void take_action(void);
void set_message();
void Trigger_Message();

void interrupt()
{
	DISABLE_INTERRUPT;
	if (EXTERNAL_INTERRUPT)
	{
		Delay_ms(500);
		trigger_ID = 255;
		CLEAR_EXTERNAL_INTERRUPT;
		CLEAR_GPIO_CHANGE_INTERRUPT;
	}
	else if (GPIO_CHANGE_INTERRUPT)
	{
		Delay_ms(500);
		callback = Trigger_Message;
		CLEAR_GPIO_CHANGE_INTERRUPT;
		CLEAR_EXTERNAL_INTERRUPT;
		return;
	}
	ENABLE_INTERRUPT;
}

void null_func()
{
	return;
}

void save_triggerID()
{
	EEPROM_Write(0, trigger_ID);
}

void read_triggerID()
{
	trigger_ID = EEPROM_Read(0);
}

void System_Init()
{
	TRISIO = BIT0_ON | BIT2_ON | BIT3_ON;
	CMCON0 = BIT0_ON | BIT1_ON | BIT2_ON;
	ADCON0 = ZERO;                // Internal ADC OFF
	ANSEL = ZERO;        // All Analog selections pins are assigned as digital I/O
	OPTION_REG = BIT3_ON | BIT2_ON | BIT1_ON | BIT0_ON; // Pre-Scaler for WDT 1:128
	WDTCON = BIT4_ON | BIT2_ON | BIT1_ON | BIT0_ON;
	INTCON = BIT4_ON | BIT3_ON; // Enable Global, INT external and GPIO Change Interrupts
	IOC = BIT3_ON;
}

void send_preamble()
{
	Soft_UART_Write(0xFE);
	Soft_UART_Write(0xFE);
	Soft_UART_Write(0xFE);
	Soft_UART_Write(0xFB);
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

void on_action()
{
	ON_GP5;
}

void off_action()
{
	OFF_GP5;
}

void toggle_action()
{
	TOGGLE_GP5;
}

void flash_action()
{
	for (i = 0;i < 15;++i)
	{
		on_action();
		Delay_ms(1000);
		off_action();
		Delay_ms(1000);
	}
}

void set_message()
{
	msg_info.message = trigger;
	//msg_info.to = my_address;
	msg_info.from = trigger_ID;
	//++msg_info.message_id;
	msg_info.bounce = 1;
	msg_info.message_header = header;
	msg_info.extra = 2;
}

void Trigger_Message()
{
	set_message();
	broadcast_message((char*)&msg_info, sizeof(msg_info));
	Delay_ms(100);
	broadcast_message((char*)&msg_info, sizeof(msg_info));
	callback = null_func;
	ENABLE_INTERRUPT;
	return;
}


void take_action()
{
	if (msg_info.message == feedback && trigger_ID == 255)
	{
		header = msg_info.message_header;
		trigger_ID = msg_info.extra;
		flash_action();
		save_triggerID();
	}
}

void process_message(void)
{

	if (trigger_address == msg_info.to)
	{
		if (last_message_id != msg_info.message_id)
		{
			take_action();
		}
	}

	last_message_id = msg_info.message_id;
}

void main(void)
{
	System_Init();
	read_triggerID();
	error = Soft_UART_Init(&GPIO, 0, 1, 1200, 0);
	callback = null_func;
	udata = 0;
	ENABLE_INTERRUPT;
	Delay_ms(100);
	SHUT_DOWN_TRANSMITTER;
	GPIO = ZERO;
	while (1)
	{
		udata = Soft_UART_Read(&error);
		switch (udata)
		{
		case '*':
			get_message((char*)&msg_info, sizeof(msg_info));
			if (msg_info.bounce == 1)
			{
				process_message();
			}
			break;
		default:
			callback();
			break;
		}
		asm CLRWDT;
	}
}