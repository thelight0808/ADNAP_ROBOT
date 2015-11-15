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
void Captureinit();
void InitConsole(void);
extern void SysConfig(void);
extern void Config_System(void);
extern uint32_t u32_UsrSystemClockGet();
#endif /* ADNAP_ROBOT_1_11_SYSCONFIGURE_SYSCONFIGURE_H_ */
