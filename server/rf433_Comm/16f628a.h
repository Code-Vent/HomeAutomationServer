#include"defs.h"
#define CLRWDT ("")
#define SLEEP ("")

#define RB7_bit 7;
#define RB6_bit 6;
#define RB5_bit 5;
#define RB4_bit 4;
#define RB3_bit 3;
#define RB2_bit 2;
#define RB1_bit 1;
#define RB0_bit 0;
// Lcd pinout settings
#define RA5_bit 5;
#define RA4_bit 4;
#define RA6_bit 6;
#define RA3_bit 3;
#define RA2_bit 2;
#define RA1_bit 1;
#define RA0_bit 0;

// Pin direction
#define TRISA4_bit 4;
#define TRISA6_bit 6;
#define TRISA3_bit 3;
#define TRISA2_bit 2;
#define TRISA1_bit 1;
#define TRISA0_bit 0;

char TRISA, TRISB, CMCON, VRCON, T1CON, T2CON, CCP1CON, TXSTA, RCSTA;

static Bits PORTA, PORTB, OPTION_REG, INTCON;




#define TOGGLE_RA0 ( PORTA.B0 = (PORTA.B0 == 0)? 1 : 0 )
#define ON_RA0 (PORTA.B0 = 1)
#define OFF_RA0 (PORTA.B0 = 0)
#define IS_RA0_ON (PORTA.B0 == 1)

#define TOGGLE_RA1 ( PORTA.B1 = (PORTA.B1 == 0)? 1 : 0 )
#define ON_RA1 (PORTA.B1 = 1)
#define OFF_RA1 (PORTA.B1 = 0)
#define IS_RA1_ON (PORTA.B1 == 1)

#define TOGGLE_RA2 ( PORTA.B2 = (PORTA.B2 == 0)? 1 : 0 )
#define ON_RA2 (PORTA.B2 = 1)
#define OFF_RA2 (PORTA.B2 = 0)
#define IS_RA2_ON (PORTA.B2 == 1)

#define TOGGLE_RA3 ( PORTA.B3 = (PORTA.B3 == 0)? 1 : 0 )
#define ON_RA3 (PORTA.B3 = 1)
#define OFF_RA3 (PORTA.B3 = 0)
#define IS_RA3_ON (PORTA.B3 == 1)

#define TOGGLE_RA4 ( PORTA.B4 = (PORTA.B4 == 0)? 1 : 0 )
#define ON_RA4 (PORTA.B4 = 1)
#define OFF_RA4 (PORTA.B4 = 0)
#define IS_RA4_ON (PORTA.B4 == 1)

#define TOGGLE_RA5 ( PORTA.B5 = (PORTA.B5 == 0)? 1 : 0 )
#define ON_RA5 (PORTA.B5 = 1)
#define OFF_RA5 (PORTA.B5 = 0)
#define IS_RA5_ON (PORTA.B5 == 1)

#define TOGGLE_RA6 ( PORTA.B6 = (PORTA.B6 == 0)? 1 : 0 )
#define ON_RA6 (PORTA.B6 = 1)
#define OFF_RA6 (PORTA.B6 = 0)
#define IS_RA6_ON (PORTA.B6 == 1)

#define TOGGLE_RA7 ( PORTA.B7 = (PORTA.B7 == 0)? 1 : 0 )
#define ON_RA7 (PORTA.B7 = 1)
#define OFF_RA7 (PORTA.B7 = 0)
#define IS_RA7_ON (PORTA.B7 == 1)

#define TOGGLE_RB0 ( PORTB.B0 = (PORTB.B0 == 0)? 1 : 0 )
#define ON_RB0 (PORTB.B0 = 1)
#define OFF_RB0 (PORTB.B0 = 0)
#define IS_RB0_ON (PORTB.B0 == 1)

#define TOGGLE_RB1 ( PORTB.B1 = (PORTB.B1 == 0)? 1 : 0 )
#define ON_RB1 (PORTB.B1 = 1)
#define OFF_RB1 (PORTB.B1 = 0)
#define IS_RB1_ON (PORTB.B1 == 1)

#define TOGGLE_RB2 ( PORTB.B2 = (PORTB.B2 == 0)? 1 : 0 )
#define ON_RB2 (PORTB.B2 = 1)
#define OFF_RB2 (PORTB.B2 = 0)
#define IS_RB2_ON (PORTB.B2 == 1)

#define TOGGLE_RB3 ( PORTB.B3 = (PORTB.B3 == 0)? 1 : 0 )
#define ON_RB3 (PORTB.B3 = 1)
#define OFF_RB3 (PORTB.B3 = 0)
#define IS_RB3_ON (PORTB.B3 == 1)

#define TOGGLE_RB4 ( PORTB.B4 = (PORTB.B4 == 0)? 1 : 0 )
#define ON_RB4 (PORTB.B4 = 1)
#define OFF_RB4 (PORTB.B4 = 0)
#define IS_RB4_ON (PORTB.B4 == 1)

#define TOGGLE_RB5 ( PORTB.B5 = (PORTB.B5 == 0)? 1 : 0 )
#define ON_RB5 (PORTB.B5 = 1)
#define OFF_RB5 (PORTB.B5 = 0)
#define IS_RB5_ON (PORTB.B5 == 1)

#define TOGGLE_RB6 ( PORTB.B6 = (PORTB.B6 == 0)? 1 : 0 )
#define ON_RB6 (PORTB.B6 = 1)
#define OFF_RB6 (PORTB.B6 = 0)
#define IS_RB6_ON (PORTB.B6 == 1)

#define TOGGLE_RB7 ( PORTB.B7 = (PORTB.B7 == 0)? 1 : 0 )
#define ON_RB7 (PORTB.B7 = 1)
#define OFF_RB7 (PORTB.B7 = 0)
#define IS_RB7_ON (PORTB.B7 == 1)


#define MAKE_RA0_INPUT (TRISIA.B0 = 1)
#define MAKE_RA0_OUTPUT (TRISIA.B0 = 0)
#define MAKE_RA1_INPUT (TRISIA.B1 = 1)
#define MAKE_RA1_OUTPUT (TRISIA.B1 = 0)
#define MAKE_RA2_INPUT (TRISIA.B2 = 1)
#define MAKE_RA2_OUTPUT (TRISIA.B2 = 0)
#define MAKE_RA3_INPUT (TRISIA.B3 = 1)
#define MAKE_RA3_OUTPUT (TRISIA.B3 = 0)
#define MAKE_RA4_INPUT (TRISIA.B4 = 1)
#define MAKE_RA4_OUTPUT (TRISIA.B4 = 0)

#define MAKE_RB0_INPUT (TRISB.B0 = 1)
#define MAKE_RB0_OUTPUT (TRISB.B0 = 0)
#define MAKE_RB1_INPUT (TRISB.B1 = 1)
#define MAKE_RB1_OUTPUT (TRISB.B1 = 0)
#define MAKE_RB2_INPUT (TRISB.B2 = 1)
#define MAKE_RB2_OUTPUT (TRISB.B2 = 0)
#define MAKE_RB3_INPUT (TRISB.B3 = 1)
#define MAKE_RB3_OUTPUT (TRISB.B3 = 0)
#define MAKE_RB4_INPUT (TRISB.B4 = 1)
#define MAKE_RB4_OUTPUT (TRISB.B4 = 0)
#define MAKE_RB5_INPUT (TRISB.B5 = 1)
#define MAKE_RB5_OUTPUT (TRISB.B5 = 0)
#define MAKE_RB6_INPUT (TRISB.B6 = 1)
#define MAKE_RB6_OUTPUT (TRISB.B6 = 0)
#define MAKE_RB7_INPUT (TRISB.B7 = 1)
#define MAKE_RB7_OUTPUT (TRISB.B7 = 0)


#define DISABLE_INTERRUPT (INTCON.F7 = 0)
#define ENABLE_INTERRUPT  (INTCON.F7 = 1)
#define DISABLE_TMR0_INTERRUPT (INTCON.F5 = 0)
#define ENABLE_TMR0_INTERRUPT  (INTCON.F5 = 1)
#define ENABLE_EXTERNAL_INTERRUPT (INTCON.F4 = 1)
#define DISABLE_EXTERNAL_INTERRUPT (INTCON.F4 = 0)
#define ENABLE_PORTB_CHANGE_INTERRUPT (INTCON.F3 = 1)
#define DISABLE_PORTB_CHANGE_INTERRUPT (INTCON.F3 = 0)
#define CLEAR_TMR0_INTERRUPT (INTCON.F2 = 0)
#define CLEAR_EXTERNAL_INTERRUPT (INTCON.F1 = 0)
#define CLEAR_PORTB_CHANGE_INTERRUPT (INTCON.F0 = 0)
#define TMR0_INTERRUPT (INTCON.F2 == 1)
#define EXTERNAL_INTERRUPT (INTCON.F1 == 1)
#define PORTB_CHANGE_INTERRUPT (INTCON.F0 == 1)

#define APPLY_PRESCALER_TO_TMR0 (OPTION_REG.F3 = 0)
#define APPLY_PRESCALER_TO_WDT (OPTION_REG.F3 = 1)