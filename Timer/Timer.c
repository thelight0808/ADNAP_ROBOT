/**
 *	Raise your ARM 2015 sample code http://raiseyourarm.com/
 *	Author: Pay it forward club
 *	http://www.payitforward.edu.vn
 *  version 0.0.1
 */

/**
 * @file	timer.c
 * @brief	timer event managment
 */

#include "../include.h"
#include "Timer.h"

#define TIMER_PERIOD_MS 1
#define MAX_TIMEOUT_EVT 10	// maximmum interrupt timer

typedef struct
{
  TIMER_CALLBACK_FUNC callback;//function call Interrupt
  unsigned long period_cnt;//timeout
}TIMEOUT_EVT;

//* Private function prototype ----------------------------------------------*/
void TIMER_ISR(void);
//* Private variables -------------------------------------------------------*/
static TIMEOUT_EVT timer_event_list[MAX_TIMEOUT_EVT];

void Timer_Init(void)
{
	//Enable Timer 4
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER4);

	//Configured Full-width periodic timer
	ROM_TimerConfigure(TIMER4_BASE, TIMER_CFG_PERIODIC);

	//Set the count time for the timer (TimerA).
	ROM_TimerLoadSet(TIMER4_BASE, TIMER_A, ROM_SysCtlClockGet() * TIMER_PERIOD_MS/ 1000);	//Interval: TIMER_PERIOD_MS(ms)

	//Registers an interrupt handler for the timer interrupt (TIMER_ISR)
	TimerIntRegister(TIMER4_BASE, TIMER_A, &TIMER_ISR);//

	//Enables an interrupt.
	ROM_IntEnable(INT_TIMER4A);

	//Enables individual timer interrupt source
	ROM_TimerIntEnable(TIMER4_BASE, TIMER_TIMA_TIMEOUT);

	//Clears timer interrupt sources
	ROM_TimerIntClear(TIMER4_BASE, TIMER_TIMA_TIMEOUT);

	//the timer keeps running while in debug mode.
	ROM_TimerControlStall(TIMER4_BASE, TIMER_A, false);

	//Enables the timerA.
	ROM_TimerEnable(TIMER4_BASE, TIMER_A);
}

/**
 * @brief Register event
 * @param callback function name
 * @param ms event timeout
 * @return timer ID
 * register Interrupt Event for function "callback" with "ms" time, function "callback" will be call after "ms" periodic
 *
 */
TIMER_ID TIMER_RegisterEvent(TIMER_CALLBACK_FUNC callback, unsigned long ms)
{
    int i;

    // find event NULL in event list to add new event
    for(i=0; i< MAX_TIMEOUT_EVT; i++)
    {
      if((timer_event_list[i].period_cnt == 0) && (timer_event_list[i].callback == NULL)) break;
    }

    //if don't have event NULL return invalid
    if(i == MAX_TIMEOUT_EVT)
    	return INVALID_TIMER_ID;

    //save new event need process
    timer_event_list[i].period_cnt = (unsigned long)(ms/TIMER_PERIOD_MS);
    timer_event_list[i].callback = callback;

    //return identify of event
    return (TIMER_ID)i;
}

//input: timer_id :identify of event
// clear register Event
//return true if event had clear otherwise return false

bool TIMER_UnregisterEvent(TIMER_ID timer_id)
{
	bool ret = false;
    if(timer_id < MAX_TIMEOUT_EVT)
    {
        timer_event_list[timer_id].period_cnt = 0;
        timer_event_list[timer_id].callback = NULL;
        ret = true;
    }
    return ret;
}

void TIMER_ISR(void)
{
    int i;

    //Clears timer interrupt sources
	ROM_TimerIntClear(TIMER4_BASE, TIMER_TIMA_TIMEOUT);

	for(i=0; i<MAX_TIMEOUT_EVT; i++)
	{
		if(timer_event_list[i].period_cnt > 0)
		{
			timer_event_list[i].period_cnt--;
			if(timer_event_list[i].period_cnt == 0 && timer_event_list[i].callback != NULL)
			{
				(timer_event_list[i].callback)();//run function interrupt event
				/*
				 * Only clear timeout callback when period equal to 0
				 * Another callback could be register in current timeout callback
				 */
				if (timer_event_list[i].period_cnt == 0)
					timer_event_list[i].callback = NULL;//clear event
			}
		}
	}

}
