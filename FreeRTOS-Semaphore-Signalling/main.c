/*
@author:    Ijaz Ahmad

@descp:     This program creates a binary semaphore
            to synchronize a task with an ISR. The 
            Task remains in block state and as soon
            as the ISR is called on button press, 
            the Task is unblocked to print a button
            pressed message to the debugger window.
            
            
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
#include "semphr.h"

#define       __setbit(___reg, ___bit)      ((___reg) |= (1U << (___bit)))
#define       __clearbit(___reg, ___bit)    ((___reg) &= (~(1U << (___bit))))
#define       __togglebit(___reg, ___bit)   ((___reg) ^= (1U << (___bit)))
#define       __getbit(___reg, ___bit)      (((___reg) & (1U << (___bit))) >> (___bit))



SemaphoreHandle_t smphrHandle;

void vTask (void *);

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
   
   smphrHandle = xSemaphoreCreateBinary();
  
  if (smphrHandle != NULL) {
    
    xTaskCreate (vTask, "Task", 200, NULL, 1, NULL);
    
    vTaskStartScheduler();  
    
  }else {
    
    printf ("Failed to create Semaphore! :-(\n");
    
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

void vTask(void * pvParams) {

  for (;;) {
    /* Lock Task here until unlocked by ISR*/
    xSemaphoreTake(smphrHandle, portMAX_DELAY);
    
    printf("Button Pressed, Task Unlocked!.\n");
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

    txStatus = xSemaphoreGiveFromISR(smphrHandle, &xHigherPriorityTaskWoken);

    if (pdPASS == txStatus) {
      portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
    }
}

#ifdef   __cplusplus
    extern  "C" {
    }
#endif

