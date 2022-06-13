#ifndef __UTILITY_H
#define __UTILITY_H

#include "stm32f446xx.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"

extern void add_thread(void *p_thread, osPriority_t thread_priority, void *p_tread_stack_addres, uint32_t tread_stack_size);
extern void gpio_init(void);
extern void set_gpio_pin(GPIO_TypeDef *GPIO, uint32_t pin, uint32_t mode, uint32_t speed, uint32_t pull);
extern void set_gpio_pin_alternate(GPIO_TypeDef *GPIO, uint32_t pin, uint32_t mode, uint32_t speed, uint32_t pull, uint32_t alternate);

#endif
