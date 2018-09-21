/*
@author:    Ijaz Ahmad

@descp:     This program creates a simple freeRTOS
            tasks which exectues after 1-sec. The purpose
            of this tutorial is to demonstrate the 
            functionality of vTaskDelay() API.
            

@warrenty:  void
*/

/*  
    Web Link: 
*/

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

void vTask(void * pvParams);

int counter = 0;

int main () {


  xTaskCreate (vTask, "T1", 150, NULL, 1, NULL);

  vTaskStartScheduler();
  
}

void vTask(void * pvParams) {
  
  TickType_t xLastWakeTime;
 
  /*
    1000-msec in terms of tick
  */
  const TickType_t xFrequency = 1000/ portTICK_PERIOD_MS;

  /*
    stamp current time... It is only required once.
    it will be updated automatically with every call
    to vTaskDelayUntil() API.
  */
  xLastWakeTime = xTaskGetTickCount();
  
  printf("Clock Task Start Running...\n");
 
  for (;;) {
    
    printf("Seconds Count: %d\n", counter++);
 
    /*
       create 1-second (1000-msec) delay.
    */
    vTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
  
}

