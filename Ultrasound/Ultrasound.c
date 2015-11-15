/*
 * Ultrasound.c
 *
 *  Created on: Oct 31, 2015
 *      Author: ADNAP TEAM
 *     Version: 1.0.1
 *
 */
/************************************************************************
 *						 IMPORT
 ***********************************************************************/
#include "../include.h"
#include "Ultrasound.h"


/************************************************************************
 * 					GLOBAL VARIABLE
 ***********************************************************************/
 /*
  * Create a constant to avoid doing math every time we do a reading. The
  * clock is at 80MHz so each counter value is equal to
  * 1/80Mhz = 0.0125us. The speed of sound in air is 340m/s = 0.034cm/us
  * The value is divided with 2 because the ultrasound is sent out and
  * after that, the sensor receives an echo.
  */
 const double Coefficient = ((1.0/80.0) * 0.034) / 2;

 /*
  * Store pulse length
  */
 double Pulse = 0;

 /*
  * Show which sensor is being read
  */
 extern char UltrasonicSensor = 0;

 /*
  * Tells the main code if the a pulse is being read at the moment
  */
 volatile uint8_t echowait=0;

 bool CompleteRead;

 static TIMER_ID Ultrasound_TimerID = INVALID_TIMER_ID;

 /************************************************************************
  *						 CONFIGURATION
  ***********************************************************************/

 //***********************************************************************
 //
 //! Input		: 	None
 //! Return		:	None
 //! Decription	: 	Configure Trigger pin and Echo pin
 //
 //***********************************************************************
 void UltrasoundPinConfig()
 {
	 /*
	  * Configure Trigger pin
	  */
	 SysCtlPeripheralEnable(TRIGGER_PORT);
	 GPIOPinTypeGPIOOutput(TRIGGER_PORT, TRIGGER_PIN_1 | TRIGGER_PIN_2);

	 /*
	  * Configure Echo pin
	  */
	 SysCtlPeripheralEnable(ECHO_PORT);
	 GPIOIntRegister(ECHO_PORT, Echo);			///Problem ????@@@@!!!!!
     GPIOPinTypeGPIOInput(ECHO_PORT, ECHO_PIN_1 | ECHO_PIN_2);
     GPIOIntTypeSet(ECHO_PORT, ECHO_PIN_1 | ECHO_PIN_2 , GPIO_BOTH_EDGES);
     GPIOIntEnable(ECHO_PORT, ECHO_PIN_1 | ECHO_PIN_2 );

     /*
      * run timeout
      */
     Ultrasound_Runtimeout(2000);
 }

 /************************************************************************
  *					 TRIGGER AND ECHO
  ************************************************************************/

 //***********************************************************************
 //
 //! Input		: 	None
 //! Return		:	None
 //! Decription	: 	Send out a 10us pulse on trigger pin.
 //
 //***********************************************************************
void Trigger()
{
	  if(READING_SRF05_1)
	  {
		 GPIOPinWrite(TRIGGER_PORT, TRIGGER_PIN_1, TRIGGER_PIN_1);
		 SysCtlDelay((SysCtlClockGet() / 1000000) *10);
		 GPIOPinWrite(TRIGGER_PORT, TRIGGER_PIN_1, ~TRIGGER_PIN_1);
	  }
	  else if(READING_SRF05_2)
	  {
		  GPIOPinWrite(TRIGGER_PORT, TRIGGER_PIN_2, TRIGGER_PIN_2);
		  SysCtlDelay((SysCtlClockGet() / 1000000) *10);
		  GPIOPinWrite(TRIGGER_PORT, TRIGGER_PIN_2, ~TRIGGER_PIN_2);
	  }
}

//***********************************************************************
//
//! Input		: 	None
//! Output		:	Pulse : Store pulse length
//! Return		:	None
//! Decription	: 	Record value pulse on echo pin.
//
//************************************************************************
void Echo()
{
	/*
	 * Clear interrupt flag. Since we only enabled on this is enough
	 */
  GPIOIntClear(ECHO_PORT, ECHO_PIN_1 | ECHO_PIN_2);

  /*
     If it's a rising edge then set he timer to 0
     It's in periodic mode so it was in some random value
   */
  if(WAITING_FOR_ECHO)
  {
	  /*
	   * Loads value 0 into the timer
	   */
	  HWREG(TIMER2_BASE + TIMER_O_TAV) = 0;

	  /*
	   * Start timer
	   */
	  TimerEnable(TIMER2_BASE,TIMER_A);
	  echowait=1;
  }
  /*
     If it's a falling edge that was detected, then get the value of the counter
   */
  else if(RECEIVE_ECHO)
  {
	  /*
	   * Record value
	   */
	  Pulse = TimerValueGet(TIMER2_BASE,TIMER_A);

	  /*
	   * Stop timer
	   */
	  TimerDisable(TIMER2_BASE,TIMER_A);
	  echowait=0;
  }
}

//***********************************************************************
//
//! Input		: UltrasonicSensor 	: Show which sensor is being read
//! 			  Pulse				: Store pulse length
//! Return		: Distance
//! Decription	: This function returns a 16-bits value. The first MSB
//!				  bits indicate which ultrasonic sensor is read.
//! 			  The fifteenth bit tells the MCU that if distance
//! 			  is in range or not
//! 				 This bit is 0 if obstacle is in range
//! 				 This bit is 1 if obstacle is out of range.
//! 			  If the fifteenth bit is 1, then the
//! 				 fourteenth bit is 0 if obstacle is too far
//! 				 fourteenth bit is 1 if obstacle is too close.
//! 			  The entire 13 bits return the value of distance in mm.
//! 			  If obstacle is out of range,
//! 			  this value doesn't make sense.
//
//***********************************************************************
int CalculateDistance(char UltrasonicSensor, double Pulse)
{
  int16_t Distance = 0;
  Distance = Pulse * Coefficient;

  //Create sisteenth bit-Show which sensor is being read
  Distance += ((int)UltrasonicSensor << 15);
  if(IN_RANGE)
  {
	  Distance &= 0xBFFF;//Clear fifteenth bit
  }
  else if(TOO_FAR)
  {
	  Distance |= 0x4000;//Set fifteenth bit
	  Distance &= 0xDFFF;//clear fourteen bit
	  Distance |= 0x3FFF;//set distance is extremely
  }
  else
  {
	  Distance |= 0x6000;//Set fifteenth and fourteen bit
	  Distance &= 0xC000;//set distance is zero
  }
  return Distance;
}

//***********************************************************************
//
//! Input		: None
//! Return		: Distance
//! Decription	: Send out pulse on trigger pin then waiting for pulse
//!				  on Echo pin and Calculate Distance
//
//***********************************************************************
int get_Distance(void)
{
	int16_t Distance = 0;
	Trigger();
	Echo();
	Distance = CalculateDistance(UltrasonicSensor,Pulse);
	if(UltrasonicSensor == 0)
	{
		UltrasonicSensor = 1;
	}
	else
	{
		UltrasonicSensor = 0;
	}
	return Distance;
}

void Timer0IntUltrasound(void)
{
	// Clear the timer interrupt.
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

	int16_t Distance = 0;
	Distance = get_Distance();
	if(Distance < 200000)
	{
		if(Distance < 100000)
		{
			//stop motor
			speed_set(MOTOR_RIGHT,0);
			speed_set(MOTOR_LEFT,0);
		}
		if (Distance && 0x8000 == 0)
		{
			//turn left
			speed_set(MOTOR_RIGHT,50);
			speed_set(MOTOR_LEFT,-50);
		}
		else if(Distance && 0x8000 == 0x8000)
		{
			//turn right
			speed_set(MOTOR_RIGHT,-50);
			speed_set(MOTOR_LEFT,50);
		}
	}
}


static void Ultrasound_Stoptimeout(void)
{
	if (Ultrasound_TimerID != INVALID_TIMER_ID)
		TIMER_UnregisterEvent(Ultrasound_TimerID);
	Ultrasound_TimerID = INVALID_TIMER_ID;
}

static void Ultrasound_Runtimeout(uint32_t msTime)
{
	Ultrasound_Stoptimeout();
	Ultrasound_TimerID = TIMER_RegisterEvent(Timer0IntUltrasound, msTime);
}


void Stop_Ultrasound(void)
{
	Ultrasound_Stoptimeout();
}

/************************************************************************
 *					 END OF FILE
 ************************************************************************/
