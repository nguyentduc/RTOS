/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*					WIN32 PORT & LINUX PORT
*                          (c) Copyright 2004, Werner.Zimmermann@fht-esslingen.de
*                 (Similar to Example 1 of the 80x86 Real Mode port by Jean J. Labrosse)
*                                           All Rights Reserved
** *****************************************************************************************************
*		Further modified by mikael.jakas@puv.fi, jukka.matila@vamk.fi
* *****************************************************************************************************
*                                               EXAMPLE #1
*********************************************************************************************************
*/

#include "includes.h"

/*
*********************************************************************************************************
*                                               CONSTANTS
*********************************************************************************************************
*/

#define  TASK_STK_SIZE                 512       /* Size of each task's stacks (# of WORDs)            */
#define  N_TASKS                        2       /* Number of identical tasks                          */

/*
*********************************************************************************************************
*                                               VARIABLES
*********************************************************************************************************
*/

OS_STK        TaskStk[N_TASKS][TASK_STK_SIZE];        /* Tasks stacks                                  */
OS_STK        TaskStartStk[TASK_STK_SIZE];
OS_EVENT		*DispSem;
OS_EVENT		*PrintSem;

/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void TaskStart(void *data);        /* Function prototype of Startup task           */
void Random_Print(void *data);   /* Function prototypes of Print_fixedPos Task     */
void Random_Print_Y(void *data);   /* Function prototypes of Print_fixedPos Task     */


/*
*********************************************************************************************************
*                                                MAIN
*********************************************************************************************************
*/

int  main (void)
{
    PC_DispClrScr(DISP_FGND_WHITE + DISP_BGND_BLACK);      /* Clear the screen                         */

    OSInit();                                              /* Initialize uC/OS-II                      */

    OSTaskCreate(TaskStart, (void *)0, &TaskStartStk[TASK_STK_SIZE - 1], 0);
	DispSem = OSSemCreate(0);								//Create display semaphore
	PrintSem = OSSemCreate(0);
    OSStart();                                             /* Start multitasking                       */

    return 0;
}


/*
*********************************************************************************************************
*                                              STARTUP TASK
*********************************************************************************************************
*/
void  TaskStart (void *pdata)
{
    INT16U key;
    INT8U symbol;
	INT8U i;
	INT8U err;

    pdata = pdata;                                         /* Prevent compiler warning                 */
    symbol = 'X';

    //for(i = 0; i < 10;  )
	OSTaskCreate(Random_Print,(void *)&symbol, (void *)&TaskStk[0][TASK_STK_SIZE - 1], 1);

	OSSemPend(DispSem, 0, &err);
	for (;;) {

		if (PC_GetKey(&key) == TRUE) {                     /* See if key has been pressed              */
			if (key == 0x1B) {                             /* Yes, see if it's the ESCAPE key          */
				exit(0);  	                           /* End program                              */
			}
		}
		OSTimeDlyHMSM(0, 0, 1, 0);                         /* Wait one second                          */
	}
	
}

/*
*********************************************************************************************************
*                                                  TASKS
*********************************************************************************************************
*/

/*void Random_Print (void *data)	//question 2
{
    INT8U  x;
    INT8U  y;
	INT8U i;

    srand(GetCurrentThreadId());
	for (;;) {
		for (i = 0; i < 3; i++) {

			x = rand() % 80;	// x from 0 to 80                        
			y = rand() % 25;	// y from 0 to 25

			PC_DispChar(x, y, *((INT8U *)data), DISP_FGND_BLACK + DISP_BGND_GRAY);

			OSTimeDlyHMSM(0, 0, 1, 0);                            // Wait one second                      

			PC_DispChar(x, y, ' ', DISP_FGND_BLACK);

		}
		OSTaskDel(1);
	}
}*///end of Random_Print


void Random_Print(void *data)
{
	INT8U  x;
	INT8U  y;
	INT8U i;
	INT8U err;

	OSTaskCreate(Random_Print_Y, (void *)0, (void *)&TaskStk[1][TASK_STK_SIZE - 1], 2);
	srand(GetCurrentThreadId());
	OSSemPend(PrintSem, 0, &err);
	for (;;) {
		for (i = 0; i < 10; i++) {

			x = rand() % 80;	// x from 0 to 80                        
			y = rand() % 25;	// y from 0 to 25

			PC_DispChar(x, y, *((INT8U *)data), DISP_FGND_BLACK + DISP_BGND_GRAY);

			OSTimeDlyHMSM(0, 0, 1, 0);                            // Wait one second                      

			PC_DispChar(x, y, ' ', DISP_FGND_BLACK);
		}
		err = OSSemPost(DispSem);
		OSTaskDel(1);
	}
}
void Random_Print_Y(void *data)
{
	INT8U  x;
	INT8U  y;
	INT8U i;
	INT8U err;

	data = data;
	srand(GetCurrentThreadId());

	for (;;) {
		for (i = 0; i < 10; i++) {

			x = rand() % 80;	// x from 0 to 80                        
			y = rand() % 25;	// y from 0 to 25

			PC_DispChar(x, y, 'Y', DISP_FGND_BLACK + DISP_BGND_GRAY);

			OSTimeDlyHMSM(0, 0, 1, 0);                            // Wait one second                      

			PC_DispChar(x, y, ' ', DISP_FGND_BLACK);
		}
		err = OSSemPost(PrintSem);
		OSTaskDel(2);
	}
}
/*
*********************************************************************************************************
*                                      NON-TASK FUNCTIONS 
*********************************************************************************************************
*/
