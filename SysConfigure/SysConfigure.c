/*
 * SysConfigure.c
 *
 *  Created on: Nov 1, 2015
 *      Author: ADNAP TEAM
 *     Version: 1.0.1
 *
 */
#include "../include.h"

#include "SysConfigure.h"
/************************************************************************
 * 						FUNCTION
 ***********************************************************************/

//***********************************************************************
//
//! Input		: 	None
//! Return		:	None
//! Decription	: 	Configures the Timer.
//
//***********************************************************************
void Captureinit()
{
  /*
    Set the timer 2_A to be periodic.
  */
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
  SysCtlDelay(3);
  TimerConfigure(TIMER2_BASE, TIMER_CFG_PERIODIC_UP);
  TimerEnable(TIMER2_BASE,TIMER_A);

  /*
     Set the timer 0_A to be periodic. Use for interrupt
   */
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
  SysCtlDelay(3);
  TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC_UP);
  TimerLoadSet(TIMER0_BASE, TIMER_A, 4000000);//50ms
  TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
  TimerEnable(TIMER0_BASE, TIMER_A);
}

//***********************************************************************
//
//! Input		: 	None
//! Return		:	None
//! Decription	: 	Configures the UART.
//
//***********************************************************************
void InitConsole(void)
{
    //
    // Enable GPIO port A which is used for UART0 pins.
    // TODO: change this to whichever GPIO port you are using.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlDelay(3);

    //
    // Configure the pin muxing for UART0 functions on port A0 and A1.
    // This step is not necessary if your part does not support pin muxing.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);

    //
    // Enable UART0 so that we can configure the clock.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    //
    // Select the alternate (UART) function for these pins.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, 16000000);

}

//***********************************************************************
//
//! Input		: 	None
//! Return		:	None
//! Decription	: 	Configures interrupt for Ultrasound
//
//***********************************************************************
void InterruptInit(void)
{
	//Configure Timer for Ultrasound
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
	extern void Timer0IntUltrasound(void);

	uint32_t ui32Period = (SysCtlClockGet() / 4000000);//50 ms????
	TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -1);
	//
	// Register the interrupt handler function for Timer0A.
	//
	IntRegister(INT_TIMER0A, Timer0IntUltrasound);

	//
	// Enable the interrupt for Timer0A.
	//
	IntEnable(INT_TIMER0A);

	//
	// Enable Timer0A .
	//
	IntMasterEnable();

	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

	TimerEnable(TIMER0_BASE, TIMER_A);
}

//***********************************************************************
//
//! Input		: 	None
//! Return		:	None
//! Decription	: 	Configures system : clock, timer, UART.
//
//***********************************************************************
void SysConfig(void)
{
 //Set system clock to 80Mhz
   SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
   //Configures the UART
   InitConsole();

   //Configures the timer
   Captureinit();

   //Configures interrupt.
   InterruptInit();
}
