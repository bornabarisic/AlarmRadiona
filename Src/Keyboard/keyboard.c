#include <stdbool.h>

#include "utility.h"
#include "display.h"
#include "keyboard.h"

/* User define */
#define ROWS   4
#define COLS   3

/* Keyboard thread stack parameters */
#define KEYBOARD_THREAD_STACK_SIZE 256
uint64_t keyboard_thread_stack[KEYBOARD_THREAD_STACK_SIZE/8];

/* Global variables */
char g_pressed_key;

/* Local variables */
static const uint16_t   column[COLS]         = {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_4};
static const uint16_t   row   [ROWS]         = {GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_8};
static const uint8_t    keys  [ROWS][COLS]   = {{'#', '0', '*'}, {'9', '8', '7'}, {'6', '5', '4'}, {'3', '2', '1'}};

/**
 * @brief Scan keyboard thread
 */
void keyboard_thread(void *arguments)
{ 
   /* Moram provjeriti ako ce odmah nakon sto se dogodi event otici u drugu dretvu ili moram staviti yield */
   
   while(1)
   {
      bool is_key_pressed = false;
      
      for (uint32_t i=0; i<COLS; i++)
      {
         for (uint32_t j=0; j<ROWS; j++)
         {
            HAL_GPIO_WritePin(GPIOA, column[i], GPIO_PIN_SET);
            is_key_pressed = HAL_GPIO_ReadPin(GPIOA, row[j]);
            
            if (is_key_pressed)
            {
               g_pressed_key = keys[j][i];               
            }
                        
            HAL_GPIO_WritePin(GPIOA, column[i], GPIO_PIN_RESET);
         }
      }
      
      osDelay(50);
   }
}

/**
 * @brief Initialize keaboard peripheral
 */
void initialize_keyboard(void)
{
   set_gpio_pin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4,             GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_PULLDOWN);
   set_gpio_pin(GPIOA, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8,  GPIO_MODE_INPUT,     GPIO_SPEED_FREQ_VERY_HIGH, GPIO_PULLDOWN);
   
   g_pressed_key = NULL;
   
   add_thread(keyboard_thread, osPriorityNormal, &keyboard_thread_stack[0], sizeof(keyboard_thread_stack));   
}
