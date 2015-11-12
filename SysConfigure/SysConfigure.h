/*
 * SysConfigure.h
 *
 *  Created on: Nov 1, 2015
 *      Author: LENOVO
 */

#ifndef ADNAP_ROBOT_1_11_SYSCONFIGURE_SYSCONFIGURE_H_
#define ADNAP_ROBOT_1_11_SYSCONFIGURE_SYSCONFIGURE_H_
/******************************************************************************
 * 								PROTOTYPE
 *
 *****************************************************************************/
#define DRV_ENABLE_LEFT_CHN_PERIPHERAL		SYSCTL_PERIPH_GPIOB
#define DRV_ENABLE_RIGHT_CHN_PERIPHERAL		SYSCTL_PERIPH_GPIOB
#define DRV_ENABLE_LEFT_CHN_PORT			GPIO_PORTB_BASE
#define DRV_ENABLE_RIGHT_CHN_PORT			GPIO_PORTB_BASE
#define DRV_ENABLE_LEFT_CHN_PIN				GPIO_PIN_3
#define DRV_ENABLE_RIGHT_CHN_PIN			GPIO_PIN_7

void Captureinit();
void InitConsole(void);
extern void SysConfig(void);

#endif /* ADNAP_ROBOT_1_11_SYSCONFIGURE_SYSCONFIGURE_H_ */
