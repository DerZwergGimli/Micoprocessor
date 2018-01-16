/* HomeWork Task 3
******************************/
#include <XC886CLM.H>
#include <XC886_lib.c>
#include <XC886_lib.h>
//******************************
//This is a modification of task2, I/O
//
//
//On the Trainingboard, Pin 2.0 is connected to a push button with a high level as default.
//This push button is to be used to control the action of the LEDs of Port 3. 
//By powering up the board, sequence 1 should be started. 
//Each time the button is pressed(= low level), the next sequence should be activated at Port 3.
//When the button is pressed after sequence 4, sequence 1 should start again.
//
//Sequence1:
//Running light, right to left: LED 3.0 is on, then 3.1 with 3.0 off and so on. After reaching 3.7, the sequence repeats again.
//
//Sequence2:
//All LEDs on Port 3.0 are blinking simultaneously
//
//Sequence3:
//Running light, left to right. As for 1, however, the direction has changed.
//
//Sequence4:
//Same as sequence 2
//
//
//
//
//*******Prototypes ***********
void init (void);
//******* Variables ***********
unsigned int ts = 10; //ms
unsigned int tp = 3750*(2) ;
//*****************************

void main (void)
{
	init();
	while(1)
	{
		;
	}
}



void init(void)
{
	//init Ports
	//PORT_PAGE = 0;
	//P2_DIR = 0;			//use Port2 as INPUT
	//P2_DATA = 0;
	//P3_DIR = 0xFF;	//use Port3 as OUTPUT
	//P3_DATA = 0;
	
	//init ISR
	//EX1 = 1; //enabele ext. interrupt
	//IT1 =1;	 //interrupt on falling-edge
	//EA =1;   //activate all ISR
	
	//lcd_init();				//actvate LCD
	//lcd_clr();				//clears the LCD
	//lcd_str("Sequence Number = "); //sets text for LCD
	
	//
	//Port Init
	PORT_PAGE =2;
	P3_ALTSEL0 = 0x80;
	P3_ALTSEL1 = 0x00;
	PORT_PAGE = 0;
	P3_DIR = 0x80;
	
	//TIMER INIT
	CCU6_PAGE = 1;
	CCU6_TCTR0H = 0x06;					//prescaler = 64
	CCU6_T13PRL = 0xa6;					//RELOAD after 3750 ticks => T13PR
	CCU6_PAGE = 2;
	CCU6_MODCTRH = 0x80;				//ENABLE COUNT63_0 on P3.7
	CCU6_PAGE = 0;
	CCU6_CC63SRL = 0x53;				//Compare after 1875 ticks
	CCU6_CC63SRH = 0x07;
	CCU6_TCTR4H = 0x42;					//run, shadow enable
	//
	
	
	
}





//void ISR_P20(void) interrupt 2
//{
	//do something when triggert
//	if (n == 4)
//		{
//			n = 1;
//		}
//	else 
//		{
//			n ++;
//		}
//	P2_DATA = 0;
//	IRCON0 = 0;	//clr ISR-Flag
//}

