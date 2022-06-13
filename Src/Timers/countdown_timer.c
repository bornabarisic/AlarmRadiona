#include "stm32f4xx_hal.h"

#include "countdown_timer.h"

/* Countdown timer configuration  */
#define COUNTDOWN_TIMER_PRESCALLER     (90000  - 1)
#define COUNTDOWN_TIMER_PERIOD         ( 1000  - 1)
#define COUNTDOWN_SECONDS              30

/** 
 *    TIM2 
 *    clock          = 90 MHz
 *    prescaller     = 90000
 *    period         = 1000
 *    update event   = 1 sec
 *    Zelim da timer generira UPDATE event svku sekundu kako bi dekremnetirao sekunde
 */

/* Global variables */
uint8_t g_seconds_to_count;

/* Timer handler struct */
static TIM_HandleTypeDef timer_handler;

void TIM2_IRQHandler(void)
{
   __HAL_TIM_CLEAR_FLAG(&timer_handler, TIM_IT_UPDATE);
   
   if (g_seconds_to_count > 0)
   {
      g_seconds_to_count--;
   }
   else
   {
      stop_countdown_timer();
      
      /* Alarm se aktivira !!! */
   }
}

void initialize_countdown_timer(void)
{
   __HAL_RCC_TIM2_CLK_ENABLE();
   
   timer_handler.Instance                 = TIM2;
   
   __HAL_TIM_DISABLE(&timer_handler);
   
   timer_handler.Init.Prescaler           = COUNTDOWN_TIMER_PRESCALLER;
   timer_handler.Init.Period              = COUNTDOWN_TIMER_PERIOD;
   timer_handler.Init.ClockDivision       = TIM_CLOCKDIVISION_DIV1;
   timer_handler.Init.CounterMode         = TIM_COUNTERMODE_UP;
   timer_handler.Init.RepetitionCounter   = 0x00;
   timer_handler.Init.AutoReloadPreload   = TIM_AUTORELOAD_PRELOAD_DISABLE;
   
   if(HAL_TIM_Base_Init(&timer_handler) != HAL_OK)
   {
      while(1);
   }
   
   __HAL_TIM_CLEAR_FLAG(&timer_handler, TIM_IT_UPDATE);
   __HAL_TIM_ENABLE_IT(&timer_handler, TIM_IT_UPDATE);
   
   HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
   HAL_NVIC_EnableIRQ(TIM2_IRQn);
   
   g_seconds_to_count = COUNTDOWN_SECONDS;
}

void start_countdown_timer(void)
{
   HAL_TIM_Base_Start(&timer_handler);
}

void stop_countdown_timer(void)
{
   HAL_TIM_Base_Start(&timer_handler);
}
