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

void vTask1(void * pvParams);
void vTask2(void * pvParams);

int main () {
  
  xTaskCreate (vTask1, "T1", 150, NULL, 1, NULL);

  xTaskCreate (vTask2, "T2", 150, NULL, 1, NULL);

  vTaskStartScheduler();
  
}

void vTask1(void * pvParams) {

  volatile unsigned int i = 0;

  for (;;) {
    printf("Task-1 Running.\n");
    
    /*Dummy Delay - Lazy work */
    for (i =0; i < 50000; ++i);
  }
}

void vTask2(void * pvParams) {

  volatile unsigned int i = 0;
 
  for (;;) {
    printf("Task-2 Running.\n");  

    /*Dummy Delay - Lazy work */
    for (i =0; i < 50000; ++i);
  }
}

