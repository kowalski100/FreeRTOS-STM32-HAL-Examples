/*
@author:    Ijaz Ahmad


@descp:     This program demostrate how freeTask
            Yield API can be used to trigger
            context switch when Task completes
            its functionality before tick timer
            expires.
            

@warrenty:  void
*/

/*  
    Web Link: 
*/

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

void vTask(void * pvParams);


int main () {

  const int task_1_Param = 1;
  const int task_2_Param = 2;

  xTaskCreate (vTask, "T1", 150, (void *)&task_1_Param, 1, NULL);

  xTaskCreate (vTask, "T2", 150, (void *)&task_2_Param, 1, NULL);

  vTaskStartScheduler();
  
}

void vTask(void * pvParams) {

  volatile unsigned int i = 0;
  const int * tParam = (const int *)pvParams;
 
  for (;;) {
    
    /*
      Do what every Task is meant to do
    */
    printf("Task-%d Running.\n", *tParam);
    
    /*
      Now that task has completed its function,
      so let's yield so that other tasks are 
      waiting to be scheduled are allowed to 
      run.
    */
    printf("Task-%d Complete - Yielding Task.\n\n\n", *tParam);
    taskYIELD();
    
  }
}

