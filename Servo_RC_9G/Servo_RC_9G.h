/*
 * Servo_RC_9G.h
 *
 *  Created on: Nov 4, 2015
 *      Author: ADNAP TEAM
 *     Version: 1.0.1
 *
 */

#ifndef SERVO_RC_9G_SERVO_RC_9G_H_
#define SERVO_RC_9G_SERVO_RC_9G_H_

#define	SERVO_PORT 	GPIO_PORTA_BASE
#define SERVO_0_PIN	GPIO_PIN_6
#define SERVO_1_PIN	GPIO_PIN_7

extern void Servo_Config();
extern void setDutyCycle(unsigned long ulDutyCycle_0,unsigned long ulDutyCycle_1);
extern void delay_servo();

#endif /* SERVO_RC_9G_SERVO_RC_9G_H_ */
