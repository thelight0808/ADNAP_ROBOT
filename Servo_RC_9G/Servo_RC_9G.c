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
		SysCtlPWMClockSet(SYSCTL_PWMDIV_1);

	//Enable the peripherals used by this program.
		SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);//Tiva Launchpad has 2 modules (0 and 1) and module 1 covers led pins
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);


	//Configure PF1 pins as PWM
		GPIOPinConfigure(GPIO_PA6_M1PWM2|GPIO_PA7_M1PWM3);
		GPIOPinTypePWM(SERVO_PORT, SERVO_0_PIN|SERVO_1_PIN);

	//
	// Configure the PWM generator for count down mode with immediate updates
	// to the parameters.
	//
		PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);

	//
	// Set the period. For a 200 Hz frequency, the period = 1/200, or 5
	// miliseconds. For a 80 MHz clock, this translates to 400,000 clock ticks.
	// Use this value to set the period.
	//
		ulPeriod = 400000;
		PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, ulPeriod);

}

void delay_servo()
{
	SysCtlDelay((SysCtlClockGet() / 10000));
}

void setDutyCycle(unsigned long ulDutyCycle_0,unsigned long ulDutyCycle_1)
{
	PWMGenDisable(PWM1_BASE, PWM_GEN_2);
//
// Set the pulse width of PWM0 for a 25% duty cycle.
//
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_2, ulPeriod*ulDutyCycle_0/100);
//
// Set the pulse width of PWM1 for a 75% duty cycle.
//
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_3, ulPeriod*ulDutyCycle_1/100);

//
// Enable the outputs.
//
PWMOutputState(PWM1_BASE, PWM_OUT_2_BIT|PWM_OUT_3_BIT, true);
//
// Start the timers in generator 0.
//
	//Enable the PWM generator
		PWMGenEnable(PWM1_BASE, PWM_GEN_2);
}



