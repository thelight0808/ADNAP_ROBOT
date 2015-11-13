/**
 *	Raise your ARM 2015 sample code http://raiseyourarm.com/
 *	Author: Pay it forward club
 *	http://www.payitforward.edu.vn
 *	version 0.0.1
 */

/**
 * @file	speed control.c
 * @brief	speed control
 */

#include "../include.h"
#include "speed_control.h"

//* Private function prototype ----------------------------------------------*/
static void Config_PWM(void);
static void SetPWM(uint32_t ulBaseAddr, uint32_t ulTimer, uint32_t ulFrequency, int32_t ucDutyCycle);
static void speed_update_setpoint(void);
static void speed_control_runtimeout(uint32_t ms);
static void speed_control_stoptimeout(void);
//* Private variables -------------------------------------------------------*/
static int32_t SetPoint[2] = {0, 0};
static int32_t RealSpeedSet[2] = {0, 0};
static TIMER_ID speed_control_timID = INVALID_TIMER_ID;
int8_t i;
void speed_control_init(void)
{
	Config_PWM();
	SetPWM(PWM_MOTOR_LEFT, DEFAULT, 0);
	SetPWM(PWM_MOTOR_RIGHT, DEFAULT, 0);
}

/**
 * @brief Init battery sense
 * @note this function must call to calculate speed control
 */
void ProcessSpeedControl(void)
{
//	SetPoint = 250;
	int32_t Cycle[0];
	//Right motor
	if (RealSpeedSet[0] > 0)//CCW mode
	{
		ROM_GPIOPinWrite(DRV_ENABLE_RIGHT_CHN_PORT, DRV_ENABLE_RIGHT_CHN_PIN_IN_1, 0);
		ROM_GPIOPinWrite(DRV_ENABLE_RIGHT_CHN_PORT, DRV_ENABLE_RIGHT_CHN_PIN_IN_2, DRV_ENABLE_RIGHT_CHN_PIN_IN_2);
	}
	else if (RealSpeedSet[0] < 0)//CW mode
	{
		ROM_GPIOPinWrite(DRV_ENABLE_RIGHT_CHN_PORT, DRV_ENABLE_RIGHT_CHN_PIN_IN_1, DRV_ENABLE_RIGHT_CHN_PIN_IN_1);
		ROM_GPIOPinWrite(DRV_ENABLE_RIGHT_CHN_PORT, DRV_ENABLE_RIGHT_CHN_PIN_IN_2, 0);
	}
	//Left motor
	if (RealSpeedSet[1] > 0)//CCW mode
	{
		ROM_GPIOPinWrite(DRV_ENABLE_LEFT_CHN_PORT, DRV_ENABLE_LEFT_CHN_PIN_IN_1, 0);
		ROM_GPIOPinWrite(DRV_ENABLE_LEFT_CHN_PORT, DRV_ENABLE_LEFT_CHN_PIN_IN_2, DRV_ENABLE_LEFT_CHN_PIN_IN_2);
	}
	else if (RealSpeedSet[1] < 0)//CW mode
	{
		ROM_GPIOPinWrite(DRV_ENABLE_LEFT_CHN_PORT, DRV_ENABLE_LEFT_CHN_PIN_IN_1, DRV_ENABLE_LEFT_CHN_PIN_IN_1);
		ROM_GPIOPinWrite(DRV_ENABLE_LEFT_CHN_PORT, DRV_ENABLE_LEFT_CHN_PIN_IN_2, 0);
	}

	for (i = 0; i < 2; i++)
	{
		 Cycle[i] = RealSpeedSet[i]*100/DEFAULT;
		 SetPWM(PWM_MOTOR_RIGHT, DEFAULT, Cycle[i]);
	}
}

static void Config_PWM(void)
{
	// Enable the GPIO B
	//Configures pin PB6 (timer 0) and PB2 (timer 3) for use by the Timer peripheral.
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	ROM_GPIOPinConfigure(GPIO_PB6_T0CCP0);
	ROM_GPIOPinConfigure(GPIO_PB2_T3CCP0);
	ROM_GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_6);

	//
	// Configure timer
	//

	// Enable the Timer 3 and Timer 0
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);

	//
	//Timer 0 A
	//

	//Two half-width timers, Half-width PWM output
	ROM_TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM);
	//Set the count time for the timer (TimerA).
	ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, DEFAULT);
	//Sets the timer match value.(PWM mode)
	ROM_TimerMatchSet(TIMER0_BASE, TIMER_A, DEFAULT); // PWM A
	//Enables the timerA.
	ROM_TimerEnable(TIMER0_BASE, TIMER_A);

	//
	//Timer 3 A
	//
	ROM_TimerConfigure(TIMER3_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM);
	ROM_TimerLoadSet(TIMER3_BASE, TIMER_A, DEFAULT);
	ROM_TimerMatchSet(TIMER3_BASE, TIMER_A, DEFAULT); // PWM B
	//Controls the output level is made active low
	ROM_TimerControlLevel(TIMER3_BASE, TIMER_A, true);
	ROM_TimerEnable(TIMER3_BASE, TIMER_A);

	//
	//Configure pin control function
	//
	ROM_SysCtlPeripheralEnable(DRV_ENABLE_LEFT_CHN_PERIPHERAL);
	ROM_SysCtlPeripheralEnable(DRV_ENABLE_RIGHT_CHN_PERIPHERAL);
	ROM_GPIOPinTypeGPIOOutput(DRV_ENABLE_LEFT_CHN_PORT, DRV_ENABLE_LEFT_CHN_PIN);
	ROM_GPIOPinTypeGPIOOutput(DRV_ENABLE_RIGHT_CHN_PORT, DRV_ENABLE_RIGHT_CHN_PIN);
	ROM_GPIOPinWrite(DRV_ENABLE_LEFT_CHN_PORT, DRV_ENABLE_LEFT_CHN_PIN, 0);
	ROM_GPIOPinWrite(DRV_ENABLE_RIGHT_CHN_PORT, DRV_ENABLE_RIGHT_CHN_PIN, 0);
	ROM_GPIOPinWrite(DRV_ENABLE_RIGHT_CHN_PORT, DRV_ENABLE_STBY, DRV_ENABLE_STBY);
}

/**
 * @brief Control Hbridge
 */
void speed_Enable_Hbridge(bool Enable)
{
	if (Enable)
	{
		ROM_GPIOPinWrite(DRV_ENABLE_LEFT_CHN_PORT, DRV_ENABLE_LEFT_CHN_PIN, DRV_ENABLE_LEFT_CHN_PIN);
		ROM_GPIOPinWrite(DRV_ENABLE_RIGHT_CHN_PORT, DRV_ENABLE_RIGHT_CHN_PIN, DRV_ENABLE_RIGHT_CHN_PIN);
	}
	else
	{
		ROM_GPIOPinWrite(DRV_ENABLE_LEFT_CHN_PORT, DRV_ENABLE_LEFT_CHN_PIN, 0);
		ROM_GPIOPinWrite(DRV_ENABLE_RIGHT_CHN_PORT, DRV_ENABLE_RIGHT_CHN_PIN, 0);
	}
}


void SetPWM(uint32_t ulBaseAddr, uint32_t ulTimer, uint32_t ulFrequency, int32_t ucDutyCycle)
{
	uint32_t ulPeriod;
	ulPeriod = ROM_SysCtlClockGet() / ulFrequency;
	ROM_TimerLoadSet(ulBaseAddr, ulTimer, ulPeriod);
	if (ucDutyCycle > 90)
		ucDutyCycle = 90;
	else if (ucDutyCycle < -90)
		ucDutyCycle = -90;
	ROM_TimerMatchSet(ulBaseAddr, ulTimer, (100 + ucDutyCycle) * ulPeriod / 200 - 1);
}

/**
 * @brief Control speed
 * @param select motor select
 * @param speed motor speed (encoder pulse / 20ms)
 */
void speed_set(MOTOR_SELECT Select, int32_t speed)
{
	speed_Enable_Hbridge(true);
	if (Select == MOTOR_RIGHT)
	{
		if (SetPoint[0] != speed)
		{
			SetPoint[0] = speed;
			speed_control_runtimeout(15);
		}
	}
	else if (Select == MOTOR_LEFT)
	{
		if (SetPoint[1] != speed)
		{
			SetPoint[1] = speed;
			speed_control_runtimeout(15);
		}
	}
	if (SetPoint[0]==0 && SetPoint[1]==0 )
	{
		speed_Enable_Hbridge(false);
	}
}


static void speed_update_setpoint(void)
{
	int i;
	speed_control_timID = INVALID_TIMER_ID;

	for (i = 0; i < 2; i++)
	{
		if (RealSpeedSet[i] + 20 < SetPoint[i])
			RealSpeedSet[i] += 20;
		else if (RealSpeedSet[i] > SetPoint[i] + 20)
			RealSpeedSet[i] -= 20;
		else
			RealSpeedSet[i] = SetPoint[i];
	}

	speed_control_runtimeout(20);
}

static void speed_control_runtimeout(uint32_t ms)
{
	speed_control_stoptimeout();
	speed_control_timID = TIMER_RegisterEvent(&speed_update_setpoint, ms);
}

static void speed_control_stoptimeout(void)
{
	if (speed_control_timID != INVALID_TIMER_ID)
		TIMER_UnregisterEvent(speed_control_timID);
	speed_control_timID = INVALID_TIMER_ID;
}
