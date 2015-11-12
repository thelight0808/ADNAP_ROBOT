/*
 * ThanhxuanTool.c
 *
 *  Created on: Aug 27, 2015
 *      Author: ThanhXuan
 */
#include <stdlib.h>
#include "Communication.h"
#include "../Battery/Battery.h"
#include "../include.h"

uint8_t data[3];
uint8_t i;
unsigned char Picture[504];
char str[3];
extern int16_t Distance;
extern char UltrasonicSensor;
char C_Distance[50];
void GetData(void)
{
	HC05_GetRxData(&data[0],3);
	switch(data[0])
	{
	case 'S'://servo
		setDutyCycle(data[1],data[2]);
		break;
	case 'U'://Ultrasound
		if (data[1])					//left sensor
		{
			UltrasonicSensor = 0;
			Distance = get_Distance();
		}
		else							//right sensor
		{
			UltrasonicSensor = 1;
			Distance = get_Distance();
		}
		Distance &= 0x3FFF;
		//itoa(Distance,C_Distance,10);??????????
		//bluetooth_print(C_Distance);
		break;
	case 'L'://LCD
		HC05_GetRxData(&Picture[0],504);
		LCDPicture(Picture);
		break;
	case 'T'://Temperature
		break;
	case 'B'://Battery
		//sprintf(str, "%.4g", get_Battery());//convert float to string????
		bluetooth_print(str);
		break;
	case 'M'://Motor
		speed_set(MOTOR_RIGHT,data[1]);
		speed_set(MOTOR_LEFT,data[2]);
		ProcessSpeedControl();
		break;
	case 'H'://hibernate
		if (data[1])					//sleep mode
		{
			// write code effect sleep here
			bluetooth_print("AT+SLEEP");
		}
		else							//wake up ???????????
		{
			for(i=0;i<8;i++)
				bluetooth_print("ADNAP_ROBOT");
			// write code effect wake up
		}
		break;

	}
	for(i=0;i<3;i++)
	data[i] = 0;
}


