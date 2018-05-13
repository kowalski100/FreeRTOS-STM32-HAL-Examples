/*
@author:    Ijaz Ahmad

@descp:     This program creates two simple freeRTOS
            tasks. after each 10 context switching,
            prioirty of one task is changed.
            

@warrenty:  void
*/

/*  
    Web Link: 
*/

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

void vTask(void * pvParams);

TaskHandle_t hTask1;
int counter = 0;

int main () {

  const int task_1_Param = 1;
  const int task_2_Param = 2;

  xTaskCreate (vTask, "T1", 150, (void *)&task_1_Param, 1, &hTask1);

  xTaskCreate (vTask, "T2", 150, (void *)&task_2_Param, 1, NULL);

  vTaskStartScheduler();
  
}

void vTask(void * pvParams) {

  volatile unsigned int i = 0;
  const int * tParam = (const int *)pvParams;
 
  for (;;) {
    
    printf("Task-%d Running.\n", *tParam);  
    
    counter++;
    if (counter == 10) {
      
      printf("Raising Task-1 Priority....\n"); 
      vTaskPrioritySet(hTask1, uxTaskPriorityGet(hTask1)+1);
    
    }else if (counter == 20) {
      
      printf("Lowering Task-1 Priority....\n");
      vTaskPrioritySet(hTask1, uxTaskPriorityGet(hTask1)-1);
      counter = 0;
    }
    
    /*Dummy Delay - Lazy work */
    for (i =0; i < 500000; ++i);
  }
}

