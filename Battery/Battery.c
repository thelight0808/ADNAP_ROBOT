/**
 *	Raise your ARM 2015 sample code http://raiseyourarm.com/
 *	Author: Pay it forward club
 *	http://www.payitforward.edu.vn
 *	version 0.0.1
 */

/**
 * @file	Battery.c
 * @brief	Battery sense
 */
#include "../include.h"
#include "Battery.h"

//* Private function prototype ----------------------------------------------*/
void BattSenseTimerTimeout(void);
void BattSenseISR(void);
static void battery_Stoptimeout(void);
static void battery_Runtimeout(TIMER_CALLBACK_FUNC TimeoutCallback, uint32_t msTime);

//* Public variables -------------------------------------------------------*/
volatile float BatteryVoltage = 0; //<! Battery voltage
//* Private variables ------------------------------------------------------*/
static TIMER_ID battery_TimerID = INVALID_TIMER_ID;

//* Function declaration ---------------------------------------------------*/
/**
 * @brief Init battery sense
 */
void BattSense_init(void)
{
	// Enable the GPIO and the ADC.
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);

	//Configures an ADC input pin
	ROM_GPIOPinTypeADC(GPIO_PORTB_BASE, GPIO_PIN_4);

	//Configures the hardware oversampling factor (x64) of the ADC
	//Averaging of 64 sample
	ROM_ADCHardwareOversampleConfigure(ADC1_BASE, 64);

	// Enable the first sample sequencer to capture the value of channel 1 when
	// the processor trigger occurs.
	ROM_ADCSequenceConfigure(ADC1_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
	ROM_ADCSequenceStepConfigure(ADC1_BASE, 3, 0, ADC_CTL_END | ADC_CTL_CH10 | ADC_CTL_IE);
	ROM_ADCSequenceEnable(ADC1_BASE, 3);

	//Registers an interrupt handler for an ADC interrupt.
 	ADCIntRegister(ADC1_BASE, 3, &BattSenseISR);

	//Enables a sample sequence interrupt
 	ROM_ADCIntEnable(ADC1_BASE, 3);

 	// Register schedule for battery measurement
 	battery_Runtimeout(&BattSenseTimerTimeout, 10000);
}

/**
 * @brief Battery measurement trigger
 */
void BattSenseTimerTimeout(void)
{
	battery_TimerID = INVALID_TIMER_ID;

	// Trigger the sample sequence.
	ROM_ADCProcessorTrigger(ADC1_BASE, 3);
}

/**
 * @brief Battery measurement convertion done callback
 */
void BattSenseISR(void)
{
	uint32_t ADCResult;

	//Clears sample sequence interrupt source.
	ROM_ADCIntClear(ADC1_BASE, 3);


	battery_Runtimeout(&BattSenseTimerTimeout, 10000);

	// Read the value from the ADC.
	ROM_ADCSequenceDataGet(ADC1_BASE, 3, (uint32_t *)&ADCResult);

	//Measure voltage
	BatteryVoltage = ((float)ADCResult) / 4096 * 3.3 * (100 + 470) / 100 + 0.3;

	if (BatteryVoltage > (float)7.6)
		{
			//shutdown robot here to protect battery
		}
		else if (BatteryVoltage < (float)7.4)
		{
			//Notify user to shutdown robot
			bluetooth_print("Low Battery !");
		}
}

static void battery_Stoptimeout(void)
{
	if (battery_TimerID != INVALID_TIMER_ID)
		TIMER_UnregisterEvent(battery_TimerID);
	battery_TimerID = INVALID_TIMER_ID;
}

static void battery_Runtimeout(TIMER_CALLBACK_FUNC TimeoutCallback, uint32_t msTime)
{
	battery_Stoptimeout();
	battery_TimerID = TIMER_RegisterEvent(TimeoutCallback, msTime);
}

float get_Battery(void)
{
	return BatteryVoltage;
}

uint8_t get_pecent_Battery(void)
{
	return (uint8_t)(BatteryVoltage*100/7.4);
}
