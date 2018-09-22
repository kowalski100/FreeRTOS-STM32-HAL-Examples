/*
@author:    Ijaz Ahmad

@descp:     This program creates two simple freeRTOS
            tasks. after 20 context switching,
            task-1 is deleted and only task-2 runs
            onward.
            

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
int flag = 0;

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

    /*
      if flag is set, it means the Task-1 is deleted
      so give some time to idle task to reclaim memory
    */
    if (flag == 1) {
      vTaskDelay(1);
      flag  = 0;
    }
    
    printf("Task-%d Running.\n", *tParam);
 
    /*
      prevents useless counting and redeletion of deleted task on overflow.
    */
    
    if (counter <=20) {
      counter++;
    }
    
    if (counter == 20) {      
      printf("Deleting Task-1....\n");
      flag = 1;
      vTaskDelete(hTask1);
    }
    
    /*Dummy Delay - Lazy work */
    for (i =0; i < 500000; ++i);
  }
}

