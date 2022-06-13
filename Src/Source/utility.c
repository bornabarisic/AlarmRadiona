#include <stdint.h>

#include "utility.h"

/**
 * @brief Add new thread to the scheduler
 */
void add_thread(void *p_thread, osPriority_t thread_priority, void *p_tread_stack_addres, uint32_t tread_stack_size)
{       
   const osThreadAttr_t attributes =
   {
      .priority     = thread_priority,
      .stack_mem    = p_tread_stack_addres,
      .stack_size   = tread_stack_size
   };
   
   osThreadNew((osThreadFunc_t)p_thread, NULL, &attributes);
}


/**
 * @brief Initialize clock of used GPIO ports
 */
void gpio_init(void)
{
   __HAL_RCC_GPIOA_CLK_ENABLE();
   __HAL_RCC_GPIOB_CLK_ENABLE();
   __HAL_RCC_GPIOC_CLK_ENABLE();
}

/**
 * @brief Configure GPIO common function
 */
void set_gpio_pin(GPIO_TypeDef *GPIO, uint32_t pin, uint32_t mode, uint32_t speed, uint32_t pull)
{   
   GPIO_InitTypeDef  GpioStruct = {0};
   
   GpioStruct.Pin    = pin;
   GpioStruct.Mode   = mode;
   GpioStruct.Speed  = speed;
   GpioStruct.Pull   = pull;
   
   HAL_GPIO_Init(GPIO, &GpioStruct);
}

/**
 * @brief Configure GPIO alternate function
 */
void set_gpio_pin_alternate(GPIO_TypeDef *GPIO, uint32_t pin, uint32_t mode, uint32_t speed, uint32_t pull, uint32_t alternate)
{
   GPIO_InitTypeDef  GpioStruct = {0};
   
   GpioStruct.Pin    = pin;
   GpioStruct.Mode   = mode;
   GpioStruct.Speed  = speed;
   GpioStruct.Pull   = pull;
   GpioStruct.Alternate = alternate;
   
   HAL_GPIO_Init(GPIO, &GpioStruct);
}
