/*
 * Temperature.c
 *
 *  Created on: Nov 14, 2015
 *      Author: LENOVO
 */

#include "../include.h"
#include "Temperature.h"

void TemperatureSensorInit()
{
    /** Enable the ADC */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    /* Enable sample sequence 3 with a processor signal trigger.
     * Sequence 3 will do a single sample when the processor sends a signal to start the conversion. */
    ADCSequenceConfigure(ADC0_BASE, ADC_SEQUENCE, ADC_TRIGGER_PROCESSOR, 0);

    /* Configure step 0 on sequence 3:
     * - Sample the temperature sensor (ADC_CTL_TS) in single-ended mode (default)
     * - Configure the interrupt flag (ADC_CTL_IE) to be set when the sample is done
     * - Tell the ADC logic that this is the last conversion on sequence 3 (ADC_CTL_END). */
    ADCSequenceStepConfigure(ADC0_BASE, ADC_SEQUENCE, ADC_STEP, ADC_CTL_TS | ADC_CTL_IE | ADC_CTL_END);

    /* Enable the ADC Sequence we're using */
    ADCSequenceEnable(ADC0_BASE, ADC_SEQUENCE);

    /* Clear the interrupt status flag. This is done to make sure the interrupt flag is cleared before we sample. */
    ADCIntClear(ADC0_BASE, ADC_SEQUENCE);
}


/**
 * Read the processor's internal temperature sensor. Accuracy is +/-5C
 * @pre halTemperatureSensorInit() was called to initialize the ADC
 * @return temperature of processor in degrees Celsius
 * @note to convert: VTSENS = 2.7 - ((TEMP + 55) / 75)
 */
int32_t halGetTemperature()
{
    /* This array is used for storing the data read from the ADC FIFO. It must be as large as the
    FIFO for the sequencer in use.  This example uses sequence 3 which has a FIFO depth of 1. */
    uint32_t ulADC0_Value[1];

    /* Our output value */
    int32_t temperatureInDegreesC;

    /* Manually Trigger the ADC conversion */
    ADCProcessorTrigger(ADC0_BASE, 3);

    /* Wait for conversion to be completed */
    while(!ADCIntStatus(ADC0_BASE, 3, false))
    {
    }

    /* Clear the ADC interrupt flag. */
    ADCIntClear(ADC0_BASE, 3);

    /* Read ADC Value. */
    ADCSequenceDataGet(ADC0_BASE, 3, ulADC0_Value);

    //UARTprintf("ADC Value = %u\n", ulADC0_Value[0]);

    /* Reference voltage of 3.3V, in mV.  */
#define REFERENCE_VOLTAGE_MV                (3300l)

    /* ADC is 12 bit resolution */
#define NUMBER_OF_STEPS_12_BIT_RESOLUTION   (4096l)

    /* The voltage from the ADC, in millivolts */
    uint32_t adcMv = (uint32_t) ((ulADC0_Value[0] * REFERENCE_VOLTAGE_MV) / NUMBER_OF_STEPS_12_BIT_RESOLUTION);

    /* Use non-calibrated conversion provided in the data sheet. Divide last to avoid dropout. */
    temperatureInDegreesC = (147500l - 75l * adcMv) / 1000;

    return temperatureInDegreesC;
}


/**
 * Utility function to convert a celsius temperature to fahrenheit.
 * @param temperatureInDegreesC the temp in C
 * @return the temp in F
 */
int32_t halConvertTemperatureFromCtoF(int32_t temperatureInDegreesC)
{
    return ((temperatureInDegreesC * 9) + 160) / 5;
}
