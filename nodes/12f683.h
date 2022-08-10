//unsigned char w_REG;
#define CLRWDT ("")

static Bits GPIO, OPTION_REG, TRISIO, INTCON, TMR0, IOC, CMCON0, ADCON0, ANSEL, WDTCON;
static Bits TMR1H, TMR1L, T1CON, CMCON1, PIE1, PIR1;
static Bits TMR2, T2CON, PR2;
//char GPIO, OPTION_REG, TRISIO, INTCON, TMR0, CMCON0, ANSEL, ADCON0;




#define TOGGLE_GP0 (GPIO.F0 = (GPIO.F0 == 0)? 1 : 0 )
#define ON_GP0 (GPIO.F0 = 1)
#define OFF_GP0 (GPIO.F0 = 0)
#define IS_GP0_ON (GPIO.F0 == 1)

#define TOGGLE_GP1 (GPIO.F1 = (GPIO.F1 == 0)? 1 : 0 )
#define ON_GP1 (GPIO.F1 = 1)
#define OFF_GP1 (GPIO.F1 = 0)
#define IS_GP1_ON (GPIO.F1 == 1)

#define TOGGLE_GP2 (GPIO.F2 = (GPIO.F2 == 0)? 1 : 0 )
#define ON_GP2 (GPIO.F2 = 1)
#define OFF_GP2 (GPIO.F2 = 0)
#define IS_GP2_ON (GPIO.F2 == 1)

#define TOGGLE_GP3 (GPIO.F3 = (GPIO.F3 == 0)? 1 : 0 )
#define ON_GP3 (GPIO.F3 = 1)
#define OFF_GP3 (GPIO.F3 = 0)
#define IS_GP3_ON (GPIO.F3 == 1)

#define TOGGLE_GP4 (GPIO.F4 = (GPIO.F4 == 0)? 1 : 0 )
#define ON_GP4 (GPIO.F4 = 1)
#define OFF_GP4 (GPIO.F4 = 0)
#define IS_GP4_ON (GPIO.F4 == 1)

#define TOGGLE_GP5 (GPIO.F5 = (GPIO.F5 == 0)? 1 : 0 )
#define ON_GP5 (GPIO.F5 = 1)
#define OFF_GP5 (GPIO.F5 = 0)
#define IS_GP5_ON (GPIO.F5 == 1)




#define MAKE_GP0_INPUT (TRISIO.F0 = 1)
#define MAKE_GP0_OUTPUT (TRISIO.F0 = 0)
#define MAKE_GP1_INPUT (TRISIO.F1 = 1)
#define MAKE_GP1_OUTPUT (TRISIO.F1 = 0)
#define MAKE_GP2_INPUT (TRISIO.F2 = 1)
#define MAKE_GP2_OUTPUT (TRISIO.F2 = 0)
#define MAKE_GP3_INPUT (TRISIO.F3 = 1)
#define MAKE_GP3_OUTPUT (TRISIO.F3 = 0)
#define MAKE_GP4_INPUT (TRISIO.F4 = 1)
#define MAKE_GP4_OUTPUT (TRISIO.F4 = 0)
#define MAKE_GP5_INPUT (TRISIO.F5 = 1)
#define MAKE_GP5_OUTPUT (TRISIO.F5 = 0)


#define DISABLE_INTERRUPT (INTCON.F7 = 0)
#define ENABLE_INTERRUPT  (INTCON.F7 = 1)
#define DISABLE_PERIPHERAL_INTERRUPT (INTCON.F6 = 0)
#define ENABLE_PERIPHERAL_INTERRUPT  (INTCON.F6 = 1)
#define DISABLE_TMR0_INTERRUPT (INTCON.F5 = 0)
#define ENABLE_TMR0_INTERRUPT  (INTCON.F5 = 1)
#define CLEAR_TMR0_INTERRUPT (INTCON.F2 = 0)
#define CLEAR_EXTERNAL_INTERRUPT (INTCON.F1 = 0)
#define CLEAR_GPIO_CHANGE_INTERRUPT (INTCON.F0 = 0)
#define TMR0_INTERRUPT (INTCON.F2 == 1)
#define EXTERNAL_INTERRUPT (INTCON.F1 == 1)
#define GPIO_CHANGE_INTERRUPT (INTCON.F0 == 1)
#define TMR0_CLOCK_SET_INTERNAL (OPTION_REG.F5 = 0)
#define TMR0_CLOCK_SET_EXTERNAL (OPTION_REG.F5 = 1)


#define WDT_PRESCALER_1_1 0b00001000
#define WDT_PRESCALER_1_2 0b00001001
#define WDT_PRESCALER_1_4 0b00001010
#define WDT_PRESCALER_1_8 0b00001011
#define WDT_PRESCALER_1_16 0b00001100
#define WDT_PRESCALER_1_32 0b00001101
#define WDT_PRESCALER_1_64 0b00001110
#define WDT_PRESCALER_1_128 0b00001111

#define TMR0_PRESCALER_1_1 0b00000000
#define TMR0_PRESCALER_1_2 0b00000001
#define TMR0_PRESCALER_1_4 0b00000010
#define TMR0_PRESCALER_1_8 0b00000011
#define TMR0_PRESCALER_1_16 0b00000100
#define TMR0_PRESCALER_1_32 0b00000101
#define TMR0_PRESCALER_1_64 0b00000110
#define TMR0_PRESCALER_1_128 0b00000111

#define APPLY_PRESCALER_TO_TMR0 (OPTION_REG.F3 = 0)
#define APPLY_PRESCALER_TO_WDT (OPTION_REG.F3 = 1)

#define DISABLE_TMR1 (T1CON.F0 = 0)
#define ENABLE_TMR1  (T1CON.F0 = 1)
#define TMR1_CLOCK_SET_INTERNAL (T1CON.F1 = 0)
#define TMR1_CLOCK_SET_EXTERNAL (T1CON.F1 = 1)

#define TMR1_PRESCALER_1_1 0b00000000
#define TMR1_PRESCALER_1_2 0b00010000
#define TMR1_PRESCALER_1_4 0b00100000
#define TMR1_PRESCALER_1_8 0b00110000
#define DISABLE_TMR1_INTERRUPT (PIE1.F0 = 0)
#define ENABLE_TMR1_INTERRUPT  (PIE1.F0 = 1)
#define CLEAR_TMR1_INTERRUPT (PIR1.F0 = 0)
