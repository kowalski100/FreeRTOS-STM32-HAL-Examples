/*
@author:    Ijaz Ahmad

@descp:     This program creates three freeRTOS
            tasks. The first two tasks sends 
            string messages (msg pointer) via  
            queue to the third task (vRxTask).
            
            
@warrenty:  void
*/

/*  
    Web Link: 
*/

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

void vTxTask1(void * pvParams);
void vTxTask2(void * pvParams);
void vRxTask(void * pvParams);

QueueHandle_t qHandle;

/*
  string literals
*/
const char *t1_Msg = "Task-1 Message...";
const char *t2_Msg = "Task-2 Message...";


int main () {
  
  qHandle = xQueueCreate(2, sizeof(char *));
  
  if (qHandle != NULL) {

    xTaskCreate (vTxTask1, "Tx1", 150, NULL, 1, NULL);

    xTaskCreate (vTxTask2, "Tx2", 150, NULL, 1, NULL);
    
    xTaskCreate (vRxTask, "Rx1", 200, NULL, 2, NULL);
    
    vTaskStartScheduler();  
    
  }else {
    
    printf ("Failed to create Queue! :-(\n");
    while (1);
    
  }
  
}

void vTxTask1(void * pvParams) {

  volatile int i = 0;
  
  int txStatus = 0;
  
  for (;;) {

    printf("Sending Msg-Task-1\n");

    txStatus = xQueueSendToBack(qHandle, &t1_Msg, 3);

    if (0 == txStatus) {
      
      printf("Sending failed Task-1!\n");
    
    }    
    /*
      This will prevent display printf messages too fast.
      also task yelding not required
    */
    for (i = 0; i < 500000; i++);
  }
}

void vTxTask2(void * pvParams) {

  volatile int i = 0;
  
  int txStatus = 0;
  
  for (;;) {

    printf("Sending Msg-Task-2\n");

    txStatus = xQueueSendToBack(qHandle, &t2_Msg, 3);

    if (0 == txStatus) {
    
      printf("Sending failed Task-2\n");    

    }

    /*
      This will prevent display printf messages too fast.
      also task yelding not required
    */
    for (i = 0; i < 500000; i++);
  }
}

void vRxTask(void * pvParams) {
  volatile unsigned int i = 0;
  char * msgPtr;
  
  int rxStatus = 0;
  
  for (;;) {
    
    rxStatus = xQueueReceive(qHandle, &msgPtr, 500);

    if (0 == rxStatus) {
      printf ("Awaiting Message...\n");
    }else {
      printf ("Rx Msg: %s\n", msgPtr);    
    }
  }
}

