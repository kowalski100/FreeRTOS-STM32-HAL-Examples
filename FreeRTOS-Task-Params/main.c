/*
@author:    Ijaz Ahmad

@descp:     This program creates two simple freeRTOS
            tasks each printing its running status. 
            The output string are printed to debug
            serial window.

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
  
  const int task_1_Num = 1;
  const int task_2_Num = 2;

  xTaskCreate (vTask, "T1", 150, (void *)&task_1_Num, 1, NULL);

  xTaskCreate (vTask, "T2", 150, (void *)&task_2_Num, 1, NULL);

  vTaskStartScheduler();
  
}

void vTask(void * pvParams) {

  volatile unsigned int i = 0;
  int *param = (int *)pvParams;

  for (;;) {
    printf("Task-%d Running\n", *param );
    
    /*Dummy Delay - Lazy work */
    for (i =0; i < 50000; ++i);
  }
}

