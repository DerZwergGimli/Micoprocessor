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
unsigned int value=1;
unsigned int n=1;				//current sequence
unsigned int blink =0;

unsigned int i1;
unsigned int i2;
//*****************************

void main (void)
{
	init();
	while (1)
	{
		switch(n)
		{
			lcd_curs(18);
			lcd_int(n);
			//Check STATE
			case 1: 
				{						
							value++;
							P3_DATA = value;
							if(value == 255){value = 0;}
							break;
				}
			case 2:
				{
					if(blink == 0){
						P3_DATA = 0;
						blink++;}
					else {
						P3_DATA = 0xFF;
						blink = 0;}
					
						break;
				}
			case 3:
				{
					value--;
					P3_DATA = value;
					if(value == 0){value = 255;}
					break;
				}
			case 4:
				{
						if(blink == 0){
						P3_DATA = 0;
						blink++;}
					else {
						P3_DATA = 0xFF;
						blink = 0;}
					
						break;
				}
			
		}
	
		
		for(i1=0; i1<4095; i1++)
		{
		for(i2=0; i2<150; i2++){;}
		}
}
	}


void sequence1(void)
{ //evlt. loop here
	value++;
	P3_DATA = 0;
	if(value == 255){value = 0;}
}

void init(void)
{
	//init Ports
	PORT_PAGE = 0;
	P2_DIR = 0;			//use Port2 as INPUT
	P2_DATA = 0;
	P3_DIR = 0xFF;	//use Port3 as OUTPUT
	P3_DATA = 0;
	
	//init ISR
	EX1 = 1; //enabele ext. interrupt
	IT1 =1;	 //interrupt on falling-edge
	EA =1;   //activate all ISR
	
	lcd_init();				//actvate LCD
	lcd_clr();				//clears the LCD
	lcd_str("Sequence Number = "); //sets text for LCD
}





void ISR_P20(void) interrupt 2
{
	//do something when triggert
	if (n == 4)
		{
			n = 1;
		}
	else 
		{
			n ++;
		}
	P2_DATA = 0;
	IRCON0 = 0;	//clr ISR-Flag
}

