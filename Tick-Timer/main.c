/*
@author:    Ijaz Ahmad

@descp:     This Program configures timer-2 as tick 
            timer for FreeRTOS, creates two simple
            tasks each blinks on of onboard LEDs on
            STM32F4-Discovery board.

@warrenty:  void
*/

/*  
    Web Link: 
*/

#include <stdint.h>
#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"

#define       __setbit(___reg, ___bit)      ((___reg) |= (1U << (___bit)))
#define       __clearbit(___reg, ___bit)    ((___reg) &= (~(1U << (___bit))))
#define       __togglebit(___reg, ___bit)   ((___reg) ^= (1U << (___bit)))
#define       __getbit(___reg, ___bit)      (((___reg) & (1U << (___bit))) >> (___bit))

/*
    Configure onboard LEDs as output
*/
void configureLEDs(void);

/*
    Simple tasks to blink LEDs.
*/
static void myTask1( void *pvParameters );
static void myTask2( void *pvParameters );

void vPortSetupTimerInterrupt (void);
extern void xPortSysTickHandler( void );

int main () {
    
    NVIC_SetPriorityGrouping(0U);
    
    /* Tick timer should have least priority */
    NVIC_SetPriority(TIM2_IRQn,0xff);
    
    /*
        Configure PD.14, PD.15 for onboard LEDs
    */
    configureLEDs();
    
    xTaskCreate( myTask1, "Task1", 50, NULL, 1, NULL);
    xTaskCreate( myTask2, "Task2", 50, NULL, 1, NULL);
    
    vTaskStartScheduler();
    
    while (1) {}

}

/*********************************************
    
    From FreeRTOSConfig.h
    configTICK_RATE_HZ = 1000
    So Timer-2 needs to be configured to
    generate 1msec delay that matches
    configTICK_RATE_HZ rate.

 *********************************************/
void vPortSetupTimerInterrupt (void) {
        
    /*****************************************
        Timers Configuration
     *****************************************/
    
    /* 
        From STM32F407 datasheet, Timer2 is clocked from
        APB1 bus (42Mhz max). In default configuration
        Timer-2 is receiving 16Mhz (HSI) bus clock.
    */
    
    /* Enable clock to Timer-2 on AHB-1 Bus */
    __setbit(RCC->APB1ENR, 0U);

    /*
        Divide the timer-2 input frequency (16Mhz)
        by a factor of 1000 (16,000,000/1,000 = 16,000 = 16Khz) 
    */
    TIM2->PSC = 1000;
    
    #if (UP_COUNTER)
     /* Up-Counter mode*/
     __clearbit(TIM2->CR1, 4U);
    #else
     /* Down Counter*/
     __clearbit(TIM2->CR1, 4U);
    #endif

    /*
        As configTICK_RATE_HZ = 1000, so tick timer
        need to generate interrupt at the rate of 1000/sec (1msec delay).
        As the input frequency is 16khz so the total
        counts required for 1msec delay:
        
        total counts = 1msec * f
                     =  0.001 * 16,000
                     = 16
    */
    TIM2->ARR = 16;

    /*
        Enable timer-2 Update interrupt to
        generate interrupt on counter overflow/underflow
    */
    __setbit(TIM2->DIER, 0U);


    /*
        Timer-2 interrupt is received on IRQ-6 on NVIC
        so enable IRQ-6 on NVIC for interrupt detection
    */
     NVIC_EnableIRQ(TIM2_IRQn);
     
     /* Start Timer-2 */
     __setbit(TIM2->CR1, 0U);

}


void TIM2_IRQHandler (void) {
    
    /* clear timer interrupt */
    __clearbit(TIM2->SR, 0U);
    
    /* call the FreeRTOS kernel for a tick update*/
    xPortSysTickHandler();
}

static void myTask1( void *pvParameters ) {
    volatile unsigned int i = 0 ;

   while (1) {
    __togglebit(GPIOD->ODR, 14);
 
    for ( i = 0; i < 500000; ++i);
   }
   
}

static void myTask2( void *pvParameters ) {
    volatile unsigned int i = 0 ;
   
   while (1) {

    __togglebit(GPIOD->ODR, 15); 
       for ( i = 0; i < 250000; ++i);
   }
    
}


void configureLEDs(void) {
    
        // enable clock to GPIOD
    __setbit(RCC->AHB1ENR, 3);
    
    /*
        On STM32F4-Discovery
        BLUE-LED     -->   PD.15  
    */
    
    /*
        configure PD.15 as general purpose output
    */
    __setbit(GPIOD->MODER, 30);
    __clearbit(GPIOD->MODER, 31);
    
    /*
        GPIOs output type: Push-Pull
        so that it can be pulled high (logic-1) and pulled down (logic-0)
    */
    __clearbit(GPIOD->OTYPER, 15);
    
    /*
        Lets keep GPIO speed: Medium
    */
    __setbit(GPIOD->OSPEEDR, 30);
    __clearbit(GPIOD->OSPEEDR, 31);
    
    /*
    Initial Level: Logic High --> No Pull-up/down
    */
    __clearbit(GPIOD->PUPDR, 30);
    __clearbit(GPIOD->PUPDR, 31);
    


    /* For GPIOD-14 */
       
    /*
        configure PD.14 as general purpose output
    */
    __setbit(GPIOD->MODER, 28);
    __clearbit(GPIOD->MODER, 29);
    
    /*
        GPIOs output type: Push-Pull
        so that it can be pulled high (logic-1) and pulled down (logic-0)
    */
    __clearbit(GPIOD->OTYPER, 14);
    
    /*
        Lets keep GPIO speed: Medium
    */
    __setbit(GPIOD->OSPEEDR, 28);
    __clearbit(GPIOD->OSPEEDR, 29);
    
    /*
    Initial Level: Logic High --> No Pull-up/down
    */
    __clearbit(GPIOD->PUPDR, 28);
    __clearbit(GPIOD->PUPDR, 29);
    
}
