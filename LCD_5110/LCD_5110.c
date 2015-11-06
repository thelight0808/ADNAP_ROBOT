/*
 *
 * LCD_5110.c
 *
 *  Created on: Oct 30, 2015
 *      Author: ADNAP TEAM
 *     Version: 1.0.1
 *
 */
/**********************************************************************
* 						IMPORT
**********************************************************************/
#include "LCD_5110.h"

#include <stdint.h>
#include <stdbool.h>
#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"

/**********************************************************************
* 						FUNCTION
**********************************************************************/

//*********************************************************************
//
//! Input		: time
//! Return		: None
//! Decription	: to Delay system for a period of time, use when needed.
//
//*********************************************************************
void Delay(volatile long time)
{
	for(;time > 0; time--);
}

//*********************************************************************
//
//! Input		: txdata : 8 bits data will be sent
//! Return		: None
//! Decription	: Data will be shifted to Pin SDIN-to send data
//
//*********************************************************************
void shiftOut(unsigned char txdata)
{
	unsigned int i;
//	P1OUT clr SCK;
	GPIOPinWrite(GPIO_PORTE_BASE, SCK, 0);
//	P1OUT set SPIN;
	GPIOPinWrite(GPIO_PORTE_BASE, SPIN, 0xFF);
	for(i = 8; i > 0; i--)
	{
		if((txdata & 0x80) == 0x80)
//			P1OUT set SPIN;
			GPIOPinWrite(GPIO_PORTE_BASE, SPIN, 0xFF);
		else
//			P1OUT clr SPIN;
			GPIOPinWrite(GPIO_PORTE_BASE, SPIN, 0);

//		Rising Edge
//		P1OUT set SCK;
		GPIOPinWrite(GPIO_PORTE_BASE, SCK, 0xFF);
//		P1OUT clr SCK;
		GPIOPinWrite(GPIO_PORTE_BASE, SCK, 0);
		txdata <<= 1;
	}
}

//*********************************************************************
//
//! Input		: data_or_command: configure the data formats
//!				  				   between Data and Command
//!							0 : Command Mode
//!							1 : Data Mode
//! 			  data: Command byte
//! Return		: None
//! Decription	: to Write data on LCD with Data Mode
//!				  or configure LCD with Command Mode
//
//*********************************************************************
void LCDWrite(char data_or_command, unsigned char data)
{
	if(data_or_command)
//		P1OUT set DC;
		GPIOPinWrite(GPIO_PORTE_BASE, DC, 0xFF);	// Data Mode
	else
//		P1OUT clr DC;
		GPIOPinWrite(GPIO_PORTE_BASE, DC, 0);		// Command Mode
//	P1OUT clr SCE;
//	PIN SCE is still Status LOW (SCE = 0) until data byte is sent successfully
	GPIOPinWrite(GPIO_PORTE_BASE, SCE, 0);
 	shiftOut(data);									//  Send the data
//	P1OUT set SCE;
 	GPIOPinWrite(GPIO_PORTE_BASE, SCE, 0xFF);
}


//*********************************************************************
//
//! Input		: x : 84 columns (X Address)
//!						from X-Address 0 to X-Address 83
//! 			  y : 6 rows (Y Address)
//!						from Y-Address 0 to Y-Address 5
//! Return		: None
//! Decription	: Move pointer to expecting position on LCD
//
//*********************************************************************
void gotoXY(int x, int y)
{
	LCDWrite(0, 0x80 | x);						// Column.
	LCDWrite(0, 0x40 | y);						// Row.
}

//*********************************************************************
//
//! Input		: character Contain character you want to display on LCD
//! Return		: None
//! Decription	: Write a character on LCD
//
//*********************************************************************
void LCDCharacter(char character)
{
	unsigned int i;
	LCDWrite(LCD_DATA, 0x00);					//Blank vertical line padding
	for (i = 0 ; i < 5 ; i++)
		LCDWrite(LCD_DATA, ASCII[character - 0x20][i]);
	LCDWrite(LCD_DATA, 0x00);					//Blank vertical line padding
}

//*********************************************************************
//
//! Input		: *characters : point to fiRES element of string
//! Return		: None
//! Decription	: Given a string of characters,
//!				  one by one is passed on to the LCD
//
//*********************************************************************
void LCDString(char *characters)
{
	while (*characters)
    LCDCharacter(*characters++);
}

//*********************************************************************
//
//! Input		: None
//! Return		: None
//! Decription	: Given a file HEX,
//!				  one by one is passed on to the LCD
//
//*********************************************************************
/*void LCDPicture()
{
	LCDClear();
	unsigned int i;
	for (i = 0 ; i < 504 ; i++)	LCDWrite(LCD_DATA, Untitled[i]);
	Delay(700000);
}*/
//*********************************************************************
//
//! Input		: None
//! Return		: None
//! Decription	: Clears the LCD by writing zero on the entire screen
//
//*********************************************************************
void LCDClear()
{
	unsigned int i;
	for (i = 0 ; i < MAX_X*MAX_Y ; i++)	LCDWrite(LCD_DATA, 0x00);
	gotoXY(0, 0);//After we clear the display, return to the home position
}

//*********************************************************************
//
//! Input		: contrast : set voltage for VLCD
//!				  coefficent : control temperature to be in the
//!						  suitable range.There are 4 values:
//!							0 : temperature coefficient 0
//!							1 : temperature coefficient 1
//!							2 : temperature coefficient 2
//!							3 : temperature coefficient 3
//! 			  bias : set value of Bias Voltage Level
//!						  must be in the range of 0 to 7
//! Output		: None
//! Return		: None
//! Decription	: This sends the magical commands to the PCD8544
//
//*********************************************************************
void LCDInit(unsigned char contrast,unsigned char coefficent,unsigned char bias)
{
//	P1DIR set RES + SCE + DC + SCK + SPIN;	//Configure control pins
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, RES|SCE|DC|SCK|SPIN);
//	P1OUT clr RES;								//Reset the LCD to a known state
	GPIOPinWrite(GPIO_PORTE_BASE, RES, 0);
//	P1OUT set RES;
	GPIOPinWrite(GPIO_PORTE_BASE, RES, 0xFF);
	LCDWrite(LCD_CMD, 0x21); 					//Tell LCD that extended commands follow
	LCDWrite(LCD_CMD, 0x80 + contrast);			//Set contrast (max 127);
	LCDWrite(LCD_CMD, 0x04 + coefficent);		//Set Temp coefficent (max 4);
	LCDWrite(LCD_CMD, 0x10 + bias);				//LCD bias mode (max 15);
	LCDWrite(LCD_CMD, 0x20);					//We must send 0x20 before modifying the display control mode
	LCDWrite(LCD_CMD, 0x0C);					//Set display control, normal mode. 0x0D for inverse
}
