/*
@author:    Ijaz Ahmad

@descp:     This program send messages to Queue
            from Push Button ISR on STM32f4-Discovery.
            A receiving Task receive the message
            and display it to serial window.
            
            
@warrenty:  void
*/

/*  
    Web Link: 
*/
#include <stdint.h>
#include "stm32f4xx.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define       __setbit(___reg, ___bit)      ((___reg) |= (1U << (___bit)))
#define       __clearbit(___reg, ___bit)    ((___reg) &= (~(1U << (___bit))))
#define       __togglebit(___reg, ___bit)   ((___reg) ^= (1U << (___bit)))
#define       __getbit(___reg, ___bit)      (((___reg) & (1U << (___bit))) >> (___bit))



QueueHandle_t qHandle;

/*
  string literals
*/
const char *t1_Msg = "Message received From Button ISR...";

void vRxTask(void * pvParams);

static void configInputPin (void);


int main () {
    
    volatile unsigned int i = 0 ;
    
    configInputPin();
    
    /*
        Configure EXTI.0 for PA.0
    */
    
    /*
        PA.0 is connected on EXTI0 which in turn 
        connected to IRQ No.6
    */
    
    /*
        enable clock to syscfg for pin selection.
    */
    __setbit(RCC->APB2ENR, 14);
    
    /*
        connect PA.0 to EXTI.0
        EXTICR[3-0] = 0b0000
    */
    __clearbit(SYSCFG->EXTICR[0],0);
    __clearbit(SYSCFG->EXTICR[0],1);
    __clearbit(SYSCFG->EXTICR[0],2);
    __clearbit(SYSCFG->EXTICR[0],3);
    
   /*
        Enable Interrupt on EXTI0 line
   */ 
   __setbit(EXTI->IMR, 0);
   
   
   /*
       trigger interrupt on rising edge
   */
   __setbit(EXTI->RTSR, 0);

   NVIC_SetPriority(EXTI0_IRQn, 6U);

   /*
       Now EXTI has been configured, finally
       enable IRQn.6 to accept interrupt.
   */
   
   NVIC_EnableIRQ(EXTI0_IRQn);


  qHandle = xQueueCreate(2, sizeof(char *));
  
  if (qHandle != NULL) {
    
    xTaskCreate (vRxTask, "RxTask", 200, NULL, 2, NULL);
    
    vTaskStartScheduler();  
    
  }else {
    
    printf ("Failed to create Queue! :-(\n");
    while (1);
    
  }

}

static void configInputPin (void) {
    
     // enable clock to GPIOA
    __setbit(RCC->AHB1ENR, 0);
        
    /*
        configure PA.0 as digital input
    */
    __clearbit(GPIOA->MODER, 0);
    __clearbit(GPIOA->MODER, 1);
    
    /*
        GPIOs output type: Push-Pull
        so that it can be pulled high (logic-1) and pulled down (logic-0)
    */
    __clearbit(GPIOA->OTYPER, 0);
    
    /*
        Lets keep GPIO speed: Medium
    */
    __setbit(GPIOA->OSPEEDR, 0);
    __clearbit(GPIOA->OSPEEDR, 1);
    
    /*
       Initial Level: Logic High --> No Pull-up/down
    */
    __clearbit(GPIOA->PUPDR, 0);
    __setbit(GPIOA->PUPDR, 1);
    
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


#ifdef   __cplusplus
    extern  "C" {        
#endif
void EXTI0_IRQHandler (void) {

    int txStatus = 0;
    BaseType_t xHigherPriorityTaskWoken;
    /*
        Clear the pending interrupt
    */
    __setbit (EXTI->PR, 0);

      printf("Button Pressed, Sending message to Queue!\n");
  
    txStatus = xQueueSendToBackFromISR(qHandle, &t1_Msg, &xHigherPriorityTaskWoken);

    if (0 == txStatus) {
      
      printf("Sending failed Task-1!\n");
    
    } else {

      portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
    
    }
}

#ifdef   __cplusplus
    extern  "C" {
    }
#endif

