//#include"12f683.h"

#ifndef EEPROM_Read
	#define EEPROM_Read
	#define EEPROM_Write
#endif

typedef int sbit;
#define at =

unsigned char w_REG;

struct Bits
{
	char F0 : 1;
	char F1 : 1;
	char F2 : 1;
	char F3 : 1;
	char F4 : 1;
	char F5 : 1;
	char F6 : 1;
	char F7 : 1;
	char B0 : 1;
	char B1 : 1;
	char B2 : 1;
	char B3 : 1;
	char B4 : 1;
	char B5 : 1;
	char B6 : 1;
	char B7 : 1;

	operator int() { return 0; }
	int operator=(int) { return 0; }
};

#define BIT0_ON 0b00000001
#define BIT1_ON 0b00000010
#define BIT2_ON 0b00000100
#define BIT3_ON 0b00001000
#define BIT4_ON 0b00010000
#define BIT5_ON 0b00100000
#define BIT6_ON 0b01000000
#define BIT7_ON 0b10000000
#define ZERO    0b00000000
#define ONE     0b00000001
#define ONES     0b11111111


#define BIT0_OFF 0b11111110
#define BIT1_OFF 0b11111101
#define BIT2_OFF 0b11111011
#define BIT3_OFF 0b11110111
#define BIT4_OFF 0b11101111
#define BIT5_OFF 0b11011111
#define BIT6_OFF 0b10111111
#define BIT7_OFF 0b01111111

#define U_DURATION_PER_BIT 116     //8.6 kbps
#define MSG_HEADER 0xFC

#define ENABLE_SENDING        ON_GP4
#define ENABLE_RECEIVING OFF_GP4

#define SENTINEL 224
#define SENTINEL_TIME_OUT  32
#define BAUD_RATE 8333
#define SERIAL_OUT_DELAY 150
#define SERIAL_OUT_LOOP 8
#define SERIAL_OUT_ONE 8
#define SERIAL_OUT_ZERO 2
#define SERIAL_IN_DELAY 25
#define SERIAL_IN_LOOP (2 * SERIAL_OUT_LOOP)
#define BYTE 8

#define TRUE ONE
#define FALSE ZERO

#define ON_STATE 30
#define OFF_STATE 40

#define SUCCESS 0x08
#define FAILED -1

#define MAX_SPEED 5

#define TOGGLE_DEVICE_STATE 0
#define GET_DEVICE_STATE 1
#define SET_DEVICE_SPEED 2
#define GET_DEVICE_SPEED 3
#define REGISTER_ME 4
#define HANDLE_BROADCAST    5
#define HOST_FEEDBACK    6
#define STATUS_MESSAGE_NONE 7
#define STATUS_MESSAGE_INCOMING 8
#define STATUS_MESSAGE_OUTGOING 9

#define DIRECT 0
#define BROADCAST    1

#define ALL_ACCESS 0xff

#define PING_TIMES 50
#define MAX_NODES 15

#define EEPROM_HOST_ADDRESS_LOCATION 0x00
#define EEPROM_MY_ADDRESS_LOCATION 0x01
#define EEPROM_REG_TOKEN_LOCATION 0x02

//#define B

#define STATUS_MESSAGE_RECEIVED 6


#define WRITE_ADDRESS_OF_COM_BIT0  {\
                                                                        OFF_GP0;\
                                                                        OFF_GP2;\
                                                                        OFF_GP4;\
                                   }

#define WRITE_ADDRESS_OF_COM_BIT1  {\
                                                                        ON_GP0;\
                                                                        OFF_GP2;\
                                                                        OFF_GP4;\
                                   }

#define WRITE_ADDRESS_OF_COM_BIT2  {\
                                                                        OFF_GP0;\
                                                                        ON_GP2;\
                                                                        OFF_GP4;\
                                   }

#define WRITE_ADDRESS_OF_COM_BIT3  {\
                                                                        ON_GP0;\
                                                                        ON_GP2;\
                                                                        OFF_GP4;\
                                   }

#define WRITE_ADDRESS_OF_COM_BIT4  {\
                                                                        OFF_GP0;\
                                                                        OFF_GP2;\
                                                                        ON_GP4;\
                                   }

#define WRITE_ADDRESS_OF_COM_BIT5  {\
                                                                        ON_GP0;\
                                                                        OFF_GP2;\
                                                                        ON_GP4;\
                                   }

#define WRITE_ADDRESS_OF_COM_BIT6  {\
                                                                        OFF_GP0;\
                                                                        ON_GP2;\
                                                                        ON_GP4;\
                                   }

#define WRITE_ADDRESS_OF_COM_BIT7  {\
                                                                        ON_GP0;\
                                                                        ON_GP2;\
                                                                        ON_GP4;\
                                   }


/*
Global variables:

SoftSpi_SDI: Data in line
SoftSpi_SDO: Data out line
SoftSpi_CLK: Data clock line
SoftSpi_SDI_Direction: Direction of the Data in pin
SoftSpi_SDO_Direction: Direction of the Data out pin
SoftSpi_CLK_Direction: Direction of the Data clock pin

*/
void Soft_SPI_Init();
unsigned short Soft_SPI_Read(char sdata);
void Soft_SPI_Write(char sdata);

/*
Global variables
Mmc_Chip_Select: Chip Select line
Mmc_Chip_Select_Direction: Direction of the Chip Select pin

*/
unsigned char Mmc_Init();
unsigned short Mmc_Fat_Init();
unsigned char Mmc_Fat_QuickFormat(char *mmc_fat_label);
unsigned short Mmc_Fat_Assign(char *filename, char file_cre_attr);
void Mmc_Fat_Reset(unsigned long *size);
void Mmc_Fat_Read(unsigned short *bdata);
void Mmc_Fat_Rewrite();
void Mmc_Fat_Append();
char Mmc_Fat_Delete();
void Mmc_Fat_Write(char *fdata, unsigned data_len);


char Soft_UART_Init(Bits *port, char rx_pin, char tx_pin, unsigned long baud_rate, char inverted);
char Soft_UART_Read(char * error);
void Soft_UART_Write(char udata);
void Delay_ms(char);
void Delay_us(char);
int strcmp(char *s1, char *s2);
char *strncpy(char *to, char *from, int size);

unsigned short Button(Bits *port, unsigned short pin, unsigned short time, unsigned short active_state);


void Lcd_Init();
void Lcd_Out(char row, char column, char *text);
void Lcd_Out_Cp(char *text);
void Lcd_Chr(char row, char column, char out_char);
void Lcd_Chr_Cp(char out_char);
void Lcd_Cmd(char out_char);


#define _LCD_FIRST_ROW 0
#define _LCD_SECOND_ROW 0
#define _LCD_THIRD_ROW 0
#define _LCD_FOURTH_ROW 0
#define _LCD_CLEAR 0
#define _LCD_RETURN_HOME 0
#define _LCD_CURSOR_OFF 0
#define _LCD_UNDERLINE_ON 0
#define _LCD_BLINK_CURSOR_ON 0
#define _LCD_MOVE_CURSOR_LEFT 0
#define _LCD_MOVE_CURSOR_RIGHT 0
#define _LCD_TURN_ON 0
#define _LCD_TURN_OFF 0
#define _LCD_SHIFT_LEFT 0
#define _LCD_SHIFT_RIGHT 0
