#include"defs.h"
#include"12f683.h"

#define SHUT_DOWN_TRANSMITTER OFF_GP5;

char error, udata, i, j;
unsigned char my_address = 80;
unsigned char return_address = 0;
unsigned char header = 45;
unsigned char last_message_id = 0;
unsigned char trigger_ID = 90;

enum MESSAGES {
	on = 0, off, toggle, flash, set_level_1,
	set_level_2, set_level_3, set_level_4, set_level_5,
	set_level_6, set_level_7, send_state, feedback, trigger, new_node
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

void broadcast_message(char* s, unsigned char len);
void on_action();
void off_action();
void toggle_action();
void flash_action();
void null_func();
void broadcaster();
void send_helper();

void(*take_action)() = null_func;

void Program();
void Reset_MyAddress();
void Bounce_Message();
void(*callback)();
void process_message(void);
void select_action(void);
void MyAddress_Init();
void Header_Init(void);
unsigned char read_com_port();

void interrupt()
{
	DISABLE_INTERRUPT;
	if (EXTERNAL_INTERRUPT)
	{
		Delay_ms(500);
		if (!IS_GP2_ON)
			callback = Program;
		else
			callback = Reset_MyAddress;
		CLEAR_EXTERNAL_INTERRUPT;
		CLEAR_GPIO_CHANGE_INTERRUPT;
		return;
	}
	else if (GPIO_CHANGE_INTERRUPT)
	{
		Delay_ms(500);
		//toggle_action();
		//send_helper();		
		CLEAR_GPIO_CHANGE_INTERRUPT;
		CLEAR_EXTERNAL_INTERRUPT;
	}
	ENABLE_INTERRUPT;
}

void send_helper() {
	msg_info.to = my_address;
	msg_info.from = my_address;
	msg_info.message_header = header;
	++msg_info.message_id;
	msg_info.message = toggle;	
	callback = broadcaster;
}

unsigned char read_com_port()
{
	unsigned char val = 0;
	unsigned char gpio_im = GPIO;
	ON_GP5;
	MAKE_GP2_OUTPUT;

	Delay_us(U_DURATION_PER_BIT);

	WRITE_ADDRESS_OF_COM_BIT0;
	Delay_us(U_DURATION_PER_BIT);
	if (IS_GP3_ON) val |= BIT0_ON;

	WRITE_ADDRESS_OF_COM_BIT1;
	Delay_us(U_DURATION_PER_BIT);
	if (IS_GP3_ON) val |= BIT1_ON;

	WRITE_ADDRESS_OF_COM_BIT2;
	Delay_us(U_DURATION_PER_BIT);
	if (IS_GP3_ON) val |= BIT3_ON;

	WRITE_ADDRESS_OF_COM_BIT3;
	Delay_us(U_DURATION_PER_BIT);
	if (IS_GP3_ON) val |= BIT2_ON;

	WRITE_ADDRESS_OF_COM_BIT4;
	Delay_us(U_DURATION_PER_BIT);
	if (IS_GP3_ON) val |= BIT6_ON;

	WRITE_ADDRESS_OF_COM_BIT7;

	MAKE_GP2_INPUT;
	GPIO = gpio_im;
	ON_GP4;

	return val;
}

void null_func()
{
	return;
}

void MyAddress_Init(void)
{
	my_address = read_com_port();
}

void Header_Init(void)
{
	header = read_com_port();
}

void Program()
{
	Header_Init();
	Delay_ms(10000);
	MyAddress_Init();
	callback = null_func;
	ENABLE_INTERRUPT;
	return;
}

void Reset_MyAddress()
{
	MyAddress_Init();
	callback = null_func;
	ENABLE_INTERRUPT;
	return;
}

void System_Init()
{
	TRISIO = BIT0_ON | BIT0_ON | BIT3_ON;
	CMCON0 = BIT0_ON | BIT1_ON | BIT2_ON;
	ADCON0 = ZERO;                // Internal ADC OFF
	ANSEL = ZERO;        // All Analog selections pins are assigned as digital I/O
	OPTION_REG = BIT3_ON | BIT2_ON | BIT1_ON | BIT0_ON; // Pre-Scaler for WDT 1:128
	WDTCON = BIT4_ON | BIT2_ON | BIT1_ON | BIT0_ON;
	INTCON = BIT4_ON | BIT3_ON; // Enable Global, INT external and GPIO Change Interrupts
	IOC = BIT3_ON;
	GPIO = BIT4_ON;
}

void send_preamble()
{
	Soft_UART_Write(0xFE);
	Soft_UART_Write(0xFE);
	Soft_UART_Write(0xFE);
	Soft_UART_Write(0xFB);
}

void broadcaster() {
	broadcast_message((char*)&msg_info, sizeof(msg_info));
	callback = null_func;
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

void Bounce_Message()
{
	msg_info.from += msg_info.bounce;
	if (msg_info.from == my_address)
		broadcast_message((char*)&msg_info, sizeof(msg_info));
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

void select_action()
{
	if (msg_info.message == toggle)
	{
		take_action = toggle_action;
	}
	else if (msg_info.message == on)
	{
		take_action = on_action;
	}
	else if (msg_info.message == off)
	{
		take_action = off_action;
	}
	else if (msg_info.message == flash)
	{
		take_action = flash_action;
	}
	else if (msg_info.message == trigger)
	{
		trigger_ID = msg_info.extra;
		take_action = flash_action;
	}
	else if (msg_info.message == feedback)
	{
		header = msg_info.message_id;
		my_address = msg_info.extra;
		take_action = flash_action;
	}
	else if (msg_info.message == send_state)
	{
		msg_info.extra = GPIO.F5;
		take_action = null_func;
	}
	else
	{
		take_action = null_func;
	}
}

void process_message(void)
{

	if (my_address == msg_info.to)
	{
		if (last_message_id != msg_info.message_id)
		{
			select_action();
			if (msg_info.message != feedback)
			{
				msg_info.to = return_address;
				msg_info.from += msg_info.bounce;
				//Change direction of bounce;
				msg_info.bounce = -1;
				msg_info.message = feedback;
				msg_info.message_header = header;
				Delay_ms(390);
				broadcast_message((char*)&msg_info, sizeof(msg_info));
				Delay_ms(420);
				broadcast_message((char*)&msg_info, sizeof(msg_info));
			}
			take_action();
		}
	}
	else if (msg_info.message == trigger)
	{
		if (trigger_ID == msg_info.from)
		{
			switch (msg_info.extra)
			{
			case 0:
				off_action();
				break;
			case 1:
				on_action();
				break;
			case 2:
				flash_action();
				break;
			case 3:
			default:
				toggle_action();
				break;
			}
		}
	}
	else if (my_address < 64)
	{
		Delay_ms(400);
		//Bounce_Message();
	}

	last_message_id = msg_info.message_id;
}

void main(void)
{
	System_Init();
	MyAddress_Init();
	error = Soft_UART_Init(&GPIO, 4, 5, 1200, 0);
	Delay_ms(100);
	callback = null_func;
	udata = 0;

	SHUT_DOWN_TRANSMITTER;
	ENABLE_INTERRUPT;
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
			//callback();			
			break;
		}
		//callback();
		asm CLRWDT;
	}
}