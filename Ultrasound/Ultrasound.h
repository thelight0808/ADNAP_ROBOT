/*
 * Ultrasound.h
 *
 *  Created on: Oct 31, 2015
 *      Author: ADNAP TEAM
 *     Version: 1.0.1
 *
 */

#ifndef ADNAP_ROBOT_1_11_ULTRASOUND_ULTRASOUND_H_
#define ADNAP_ROBOT_1_11_ULTRASOUND_ULTRASOUND_H_

/******************************************************************************
 * 								DEFINE
 *****************************************************************************/

#define SRF05_1 			0
#define SRF05_2 			1


#define READING_SRF05_1		UltrasonicSensor == 0
#define READING_SRF05_2		UltrasonicSensor == 1


#define ECHO_PORT			GPIO_PORTA_BASE
#define ECHO_PIN_1			GPIO_PIN_4	//PA4
#define ECHO_PIN_2			GPIO_PIN_5	//PA5

#define TRIGGER_PORT		GPIO_PORTA_BASE
#define TRIGGER_PIN_1		GPIO_PIN_2	//PA2
#define TRIGGER_PIN_2		GPIO_PIN_3	//PA3


#define WAITING_FOR_ECHO    (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_4) == GPIO_PIN_4 || \
							 GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_5) == GPIO_PIN_5 )

#define RECEIVE_ECHO        (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_4) == 0 || \
							 GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_5) == 0 )
#define TOO_FAR				(Pulse > ((SysCtlClockGet() / 1000) * 30))
#define IN_RANGE			(Pulse > ((SysCtlClockGet() / 1000000) * 20) && \
							Pulse <= ((SysCtlClockGet() / 1000) * 30))

/******************************************************************************
 * 								PROTOTYPE
 *****************************************************************************/

 extern void UltrasoundPinConfig();
 extern void Echo();
 extern void Trigger();
 extern int CalculateDistance(char UltrasonicSensor, double Pulse);
 extern int get_Distance(void);
 static void Ultrasound_Runtimeout(uint32_t msTime);
 extern void Stop_Ultrasound(void);

/******************************************************************************
 * 								END OF FILE
 *****************************************************************************/
#endif /* ADNAP_ROBOT_1_11_ULTRASOUND_ULTRASOUND_H_ */
