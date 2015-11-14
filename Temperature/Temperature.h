/*
 * Temperature.h
 *
 *  Created on: Nov 14, 2015
 *      Author: LENOVO
 */

#ifndef TEMPERATURE_TEMPERATURE_H_
#define TEMPERATURE_TEMPERATURE_H_

#define ADC_SEQUENCE    3
#define ADC_STEP        0

extern void TemperatureSensorInit();
extern int32_t halGetTemperature();

#endif /* TEMPERATURE_TEMPERATURE_H_ */
