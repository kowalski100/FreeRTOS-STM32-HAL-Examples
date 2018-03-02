/*
 * @author:         Ijaz Ahmad
 *
 * @Warrenty:       void
 *
 * @description:    This project demostrate FreeRTOS and STM HAL 
 *                  integration. FreeRTOS is used to schadule
 *                  two simple tasks. Each task is responsible
 *                  to glow one on-board LED. For more details,
 *                  visit the following weblink.
 *                  
 */

/*
 * @WebLink: 
 */

#include "stm32f4xx_hal.h"

/*
    Global Variables
*/
TIM_HandleTypeDef TIM_InitStruct;


/*
    Configure onboard LEDs as output
*/
void configureLEDs(void);

/*
    Simple tasks to blink LEDs.
*/
static void myTask1( void *pvParameters );
static void myTask2( void *pvParameters );

/*
    For Tick Timer Configuration
*/
void vPortSetupTimerInterrupt (void);
extern void xPortSysTickHandler( void );

int main () {
    
    GPIO_InitTypeDef GPIO_InitStruct;
    
    /*
        initialize HAL Library. This step is mandatory
        for HAL libraries to work properly. It configures
        SysTick etc. for its internal configurations.
    */
    HAL_Init();
    
    NVIC_SetPriorityGrouping(0U);

    /*
        After HAL_Init(), System is running on default HSI 
        clock (16MHz). PLL is disabled.
    */
    
    /* Enable clock to GPIO-D */
    __HAL_RCC_GPIOD_CLK_ENABLE();
    
    /* Set GPIOD Pin#15 Parameters */
    GPIO_InitStruct.Pin     = GPIO_PIN_15;
    GPIO_InitStruct.Mode    = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull    = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed   = GPIO_SPEED_FREQ_LOW;
    /* Init GPIOD Pin#15 */
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    
    for (;;) {
        
        /* Toggle GPIOD Pin#15 --> BLUE LED on STM32F407-Discovery */
        HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
        
        /* 100ms second delay */
        HAL_Delay(100);
        
    }
    
}

/*********************************************
    
    From FreeRTOSConfig.h
    configTICK_RATE_HZ = 1000
    So Timer-2 needs to be configured to
    generate 1msec delay that matches
    configTICK_RATE_HZ rate.
 *********************************************/
void vPortSetupTimerInterrupt (void) {

    /* 
        Enable clock to Timer-2 
        
        NOTE: This is lagacy Macro. The better approach is the
        use of HAL_TIM_ConfigClockSource function.
    */
    __HAL_RCC_TIM2_CLK_ENABLE();
    
    /*
        From STM32F407 datasheet, Timer2 is clocked from
        APB1 bus (42Mhz max). In default configuration
        Timer-2 is receiving 16Mhz (HSI) bus clock.
    */        
    
    /***************************************************
                   Timer-2 Configuration:
    ****************************************************/
    
    /* Select Timer-2 for further configuration */
    TIM_InitStruct.Instance = TIM2;
    
    /*
        Divide the timer-2 input frequency (16Mhz)
        by a factor of 1000 (16,000,000/1,000 = 16,000 = 16Khz) 
    */
    TIM_InitStruct.Init.Prescaler   = 1000;
    
    #if (UP_COUNTER)
     /* Up-Counter mode*/
     TIM_InitStruct.Init.CounterMode = TIM_COUNTERMODE_UP;
    #else 
      TIM_InitStruct.Init.CounterMode = TIM_COUNTERMODE_DOWN;        
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
    TIM_InitStruct.Init.Period = 16;
        
    /*
        Finally initialize Timer-2
    */
    while (HAL_TIM_Base_Init(&TIM_InitStruct)!= HAL_OK);

    /*
        Enable timer-2 IRQ interrupt
    */
    HAL_TIM_Base_Start_IT(&TIM_InitStruct);

    /* Enable interrupt at IRQ-Level */
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
        
    /* Tick timer should have least priority */
    NVIC_SetPriority(TIM2_IRQn,0xff);    
    
    /*
        Start the timer
    */
    HAL_TIM_Base_Start(&TIM_InitStruct);
}

