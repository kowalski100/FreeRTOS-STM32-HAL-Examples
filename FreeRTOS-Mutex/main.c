/*
@author:    Ijaz Ahmad

@descp:     This program uses Mutex to restrict
            access to stdout. only one task will
            be able to access printf/stdout until
            its finished writing to stdout.

@warrenty:  void
*/

/*  
    Web Link: 
*/

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/*
  Mutex Handle
*/
SemaphoreHandle_t mutexHandle;


void vTask1(void * pvParams);
void vTask2(void * pvParams);

int main () {
  
  mutexHandle = xSemaphoreCreateMutex();
  
  if (mutexHandle != NULL ) {

    xTaskCreate (vTask1, "T1", 200, NULL, 1, NULL);

    xTaskCreate (vTask2, "T2", 200, NULL, 1, NULL);

    vTaskStartScheduler();
    
  }else {
    
    printf ("failed to create Mutex!\n");
    while (1);
  }
  
}

void vTask1(void * pvParams) {

  volatile unsigned int i = 0;

  for (;;) {
    
    xSemaphoreTake(mutexHandle, portMAX_DELAY); 
    
    printf("Message From Task-1: FreeRTOS Task-1 Running...\n");

    xSemaphoreGive( mutexHandle );    
    
    /*Dummy Delay - Lazy work */
    for (i =0; i < 200000; ++i);
  }
}

void vTask2(void * pvParams) {

  volatile unsigned int i = 0;
 
  for (;;) {

    xSemaphoreTake(mutexHandle, portMAX_DELAY);
    
    printf("Message From Task-2: FreeRTOS Task-2 Running...\n");

    xSemaphoreGive( mutexHandle );    

    /*Dummy Delay - Lazy work */
    for (i =0; i < 200000; ++i);
  }
}

