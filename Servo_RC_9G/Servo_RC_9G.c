/*
 * Servo_RC_9G.c
 *
 *  Created on: Nov 4, 2015
 *      Author: ADNAP TEAM
 *     Version: 1.0.1
 *
 */
#include "../include.h"
#include "Servo_RC_9G.h"
#include "driverlib/pwm.h"
unsigned long ulPeriod;//Period of PWM

void Servo_Config()
{
	//Configure PWM clock to match system
		SysCtlPWMClockSet(SYSCTL_PWMDIV_64);//PWM clock is 80M/64 = 1,25Mhz

	//Enable the peripherals used by this program.
		SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);//Tiva Launchpad has 2 modules (0 and 1) and module 1 covers led pins
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);


	//Configure PF1 pins as PWM
		GPIOPinConfigure(GPIO_PE4_M0PWM4|GPIO_PE5_M0PWM5);
		GPIOPinTypePWM(SERVO_PORT, SERVO_0_PIN|SERVO_1_PIN);

	//
	// Configure the PWM generator for count down mode with immediate updates
	// to the parameters.
	//
		PWMGenConfigure(PWM0_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);

	//
	// Set the period. For a 50 Hz frequency, the period = 1/50, or 20
	// miliseconds. For a 1,25 MHz clock, this translates to 25,000 clock ticks.
	// Use this value to set the period.
	//
		ulPeriod = 25000;
		PWMGenPeriodSet(PWM0_BASE, PWM_GEN_2, ulPeriod);
}



void delay_servo()
{
	SysCtlDelay((SysCtlClockGet() / 10));
}

void setDutyCycle(unsigned long ulDutyCycle_0,unsigned long ulDutyCycle_1)
{
	PWMGenDisable(PWM0_BASE, PWM_GEN_2);
	//
	// Set the pulse width of PWM1_2
	//
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_4, ulPeriod*ulDutyCycle_0/100);
	//
	// Set the pulse width of PWM1_3
	//
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_5, ulPeriod*ulDutyCycle_1/100);
	//
	// Enable the outputs.
	//
	PWMOutputState(PWM0_BASE, PWM_OUT_4_BIT|PWM_OUT_5_BIT, true);
	//
	//Enable the PWM generator
	//
	PWMGenEnable(PWM0_BASE, PWM_GEN_2);
}



