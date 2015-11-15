/*
 * ThanhxuanTool.c
 *
 *  Created on: Aug 27, 2015
 *      Author: ThanhXuan
 */
#include "Communication.h"
#include "../include.h"

uint8_t data[3],bt;
uint8_t i;
unsigned char Picture[504];
int16_t Distance;
extern char UltrasonicSensor;
void GetData(void)
{
	for(i=0;i<3;i++)
	{
		data[i] = 0;
	}
	HC05_GetRxData(data,3);
	switch(data[0])
	{
	case 'S'://servo
		//bluetooth_print("Servo   \n");
		bluetooth_print("%d %d\n",data[1],data[2]);
		setDutyCycle(10,50);
		delay_servo();
		break;
	case 'U'://Ultrasound
		bluetooth_print("Ultralsound\n");
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
		bluetooth_print("Distance: %d",Distance);
		break;
	case 'L'://LCD
		bluetooth_print("LCD\n");
		HC05_GetRxData(&Picture[0],504);
		LCDPicture(Picture);
		break;
	case 'T'://Temperature
		bluetooth_print("Temperature\n");
		bluetooth_print("%d",(uint8_t)halGetTemperature());
		break;
	case 'B'://Battery
		bluetooth_print("Battery\n");
		bluetooth_print("%d",get_pecent_Battery());
		break;
	case 'M'://Motor
		bluetooth_print("Motor\n");
		speed_set(MOTOR_RIGHT,data[1]);
		speed_set(MOTOR_LEFT,data[2]);
		ProcessSpeedControl();
		break;
	case 'H'://hibernate
		bluetooth_print("Hibernate\n");
		if (data[1])					//sleep mode
		{
			// write code effect sleep here
			setDutyCycle(0,0);
			Stop_Ultrasound();
			LCDClear();
			LCDBackLight_OFF();
			speed_Enable_Hbridge(false);
			bluetooth_print("AT+SLEEP");
		}
		else							//wake up ???????????
		{
			// write code effect wake up
		}
		break;

	}
}


