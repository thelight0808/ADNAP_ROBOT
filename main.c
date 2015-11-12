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

extern int16_t Distance = 0;
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
	bluetooth_init(115200);//baudrate = 11520
	while(1)
	{
		GetData();
	}
}
void Timer0IntUltrasound(void)
{
	// Clear the timer interrupt.
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	Distance = get_Distance();
	if(Distance < 200000)
	{
		if(Distance < 100000)
		{
			//stop motor
		}
		if (Distance && 0x8000 == 0)
		{
			//turn left
		}
		else if(Distance && 0x8000 == 0x8000)
		{
			//turn right
		}
	}
}
/******************************************************************************
 * 								END OF FILE
 *****************************************************************************/
