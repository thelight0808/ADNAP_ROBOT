/*
 * main.c
 *
 *  Created on: Oct 30, 2015
 *      Author: ADNAP TEAM
 *     Version: 1.0.1
 *
 */

/******************************************************************************
 * 								IMPORT
 *****************************************************************************/
#include "include.h"

/******************************************************************************
 * 							MAIN
 *****************************************************************************/
int main(void)
{
	// Configures system
	Timer_Init();
	speed_control_init();
	HostCommInit();
	BattSense_init();
	SysConfig();
	UltrasoundPinConfig();
	LCDInit(50,0,5);
	Servo_Config();
	TemperatureSensorInit();
	bluetooth_init(115200);//baudrate = 11520
	while(1)
	{
		GetData();
	}
}

/******************************************************************************
 * 								END OF FILE
 *****************************************************************************/
