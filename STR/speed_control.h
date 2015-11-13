/**
 *	Raise your ARM 2015 sample code http://raiseyourarm.com/
 *	Author: Pay it forward club
 *	http://www.payitforward.edu.vn
 *	version 0.0.1
 */

/**
 * @file	speed_control.h
 * @brief	speed control
 */


#ifndef GOOGLECAR_18_9_14H_STR_SPEED_CONTROL_H_
#define GOOGLECAR_18_9_14H_STR_SPEED_CONTROL_H_

#define PWM_MOTOR_LEFT			TIMER0_BASE, TIMER_A//PB6
#define PWM_MOTOR_RIGHT			TIMER3_BASE, TIMER_A//PB2

#define DRV_ENABLE_LEFT_CHN_PERIPHERAL		SYSCTL_PERIPH_GPIOB
#define DRV_ENABLE_RIGHT_CHN_PERIPHERAL		SYSCTL_PERIPH_GPIOB

#define DRV_ENABLE_LEFT_CHN_PORT			GPIO_PORTB_BASE
#define DRV_ENABLE_RIGHT_CHN_PORT			GPIO_PORTB_BASE

#define DRV_ENABLE_LEFT_CHN_PIN				GPIO_PIN_0|GPIO_PIN_1 //AIN1,AIN2
#define DRV_ENABLE_RIGHT_CHN_PIN			GPIO_PIN_3|GPIO_PIN_4 //BIN1,BIN2

#define DRV_ENABLE_STBY						GPIO_PIN_5//Stand by

#define DRV_ENABLE_LEFT_CHN_PIN_IN_1		GPIO_PIN_0//AIN1
#define DRV_ENABLE_LEFT_CHN_PIN_IN_2		GPIO_PIN_1//AIN2
#define DRV_ENABLE_RIGHT_CHN_PIN_IN_1		GPIO_PIN_3//BIN1
#define DRV_ENABLE_RIGHT_CHN_PIN_IN_2		GPIO_PIN_4//BIN2

#define Uht			(double)12.0

#define DEFAULT		100000	//H-Bridge Freq (Hz)

typedef enum
{
	MOTOR_LEFT = 0,
	MOTOR_RIGHT
} MOTOR_SELECT;

extern void speed_control_init(void);
extern void ProcessSpeedControl(void);
extern void speed_set(MOTOR_SELECT Select, int32_t speed);
extern void speed_Enable_Hbridge(bool Enable);

#endif /* GOOGLECAR_18_9_14H_STR_SPEED_CONTROL_H_ */
