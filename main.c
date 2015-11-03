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

#include "LCD_5110/LCD_5110.h"
#include "Ultrasound/Ultrasound.h"
#include "SysConfigure/SysConfigure.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/timer.h"
int16_t Distance = 0;
/******************************************************************************
 * 								MAIN
 *****************************************************************************/
int main(void)
{
	// Configures system : clock, timer, UART
	SysConfig();

	//Configure Ultrasound pin
	UltrasoundPinConfig();

	LCDInit(45,0,4);

	LCDClear();
	gotoXY(0,0);
	//		   012345678912
	LCDString("   TIVA C   ");
	gotoXY(0,1);
	LCDString("LM4F123GH6PM");
	gotoXY(0,3);
	LCDString("Distance(mm)");



	/*
      //Prints out the distance measured.
      UARTprintf("distance = %2dcm \n" , pulse);
		gotoXY(0,4);
		LCDCharacter((pulse/10000)%10 + 0x30);
		LCDCharacter((pulse/1000)%10 + 0x30);
		LCDCharacter((pulse/100)%10 + 0x30);
		LCDCharacter((pulse/10)%10 + 0x30);
		LCDCharacter(pulse%10 + 0x30);
		LCDString(" cm ");
      }
      //wait about 10ms until the next reading.
      SysCtlDelay(400000);
  */

}
void Timer0IntHandler(void)
{
	// Clear the timer interrupt.
	    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	Distance = get_Distance();
}
/******************************************************************************
 * 								END OF FILE
 *****************************************************************************/
