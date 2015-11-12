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
#include "../include.h"
#include "LCD_5110.h"
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
//	P1OUT clr Clk;
	GPIOPinWrite(LCD_PORT, Clk, 0);
//	P1OUT set Din;
	GPIOPinWrite(LCD_PORT, Din, 0xFF);
	for(i = 8; i > 0; i--)
	{
		if((txdata & 0x80) == 0x80)
//			P1OUT set Din;
			GPIOPinWrite(LCD_PORT, Din, 0xFF);
		else
//			P1OUT clr Din;
			GPIOPinWrite(LCD_PORT, Din, 0);

//		Rising Edge
//		P1OUT set Clk;
		GPIOPinWrite(LCD_PORT, Clk, 0xFF);
//		P1OUT clr Clk;
		GPIOPinWrite(LCD_PORT, Clk, 0);
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
		GPIOPinWrite(LCD_PORT, DC, 0xFF);	// Data Mode
	else
//		P1OUT clr DC;
		GPIOPinWrite(LCD_PORT, DC, 0);		// Command Mode
//	P1OUT clr CE;
//	PIN CE is still Status LOW (CE = 0) until data byte is sent successfully
	GPIOPinWrite(LCD_PORT, CE, 0);
 	shiftOut(data);									//  Send the data
//	P1OUT set CE;
 	GPIOPinWrite(LCD_PORT, CE, 0xFF);
}


//*********************************************************************
//
//! Input		: x : 84 columns (X AddRSTs)
//!						from X-AddRSTs 0 to X-AddRSTs 83
//! 			  y : 6 rows (Y AddRSTs)
//!						from Y-AddRSTs 0 to Y-AddRSTs 5
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
//! Input		: *characters : point to fiRST element of string
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
//! Decription	: Turn on BackLight LCD
//
//*********************************************************************
void LCDBackLight_ON()
{
	GPIOPinWrite(LCD_PORT, BL, 0xFF);
}
//*********************************************************************
//
//! Input		: None
//! Return		: None
//! Decription	: Turn off BackLight LCD
//
//*********************************************************************
void LCDBackLight_OFF()
{
	GPIOPinWrite(LCD_PORT, BL, 0);
}
//*********************************************************************
//
//! Input		: None
//! Return		: None
//! Decription	: Given a file HEX,
//!				  one by one is passed on to the LCD
//
//*********************************************************************
void LCDPicture(unsigned char Picture[MAX_X*MAX_Y])
{
	LCDClear();
	unsigned int i;
	for (i = 0 ; i < MAX_X*MAX_Y ; i++)	LCDWrite(LCD_DATA, Picture[i]);
	Delay(700000);
}
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
//	P1DIR set RST + CE + DC + Clk + Din;	//Configure control pins
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	GPIOPinTypeGPIOOutput(LCD_PORT, RST|CE|DC|Clk|Din|BL);
//	P1OUT clr RST;								//RSTet the LCD to a known state
	GPIOPinWrite(LCD_PORT, RST, 0);
//	P1OUT set RST;
	GPIOPinWrite(LCD_PORT, RST, 0xFF);
	LCDWrite(LCD_CMD, 0x21); 					//Tell LCD that extended commands follow
	LCDWrite(LCD_CMD, 0x80 + contrast);			//Set contrast (max 127);
	LCDWrite(LCD_CMD, 0x04 + coefficent);		//Set Temp coefficent (max 4);
	LCDWrite(LCD_CMD, 0x10 + bias);				//LCD bias mode (max 15);
	LCDWrite(LCD_CMD, 0x20);					//We must send 0x20 before modifying the display control mode
	LCDWrite(LCD_CMD, 0x0C);					//Set display control, normal mode. 0x0D for inverse
	LCDClear();
}
