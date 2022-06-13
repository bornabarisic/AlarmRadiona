#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "utility.h"
#include "lcd.h"
#include "keyboard.h"
#include "password.h"
#include "countdown_timer.h"
#include "display.h"

/* User defines */
#define NUM_OF_CHARACTERS  16

/* Thread for display stack parameters */
#define DISPLAY_STATE_THREAD_STACK_SIZE   256
uint64_t display_thread_stack[DISPLAY_STATE_THREAD_STACK_SIZE/8];

/* Global varables */
char *g_alarm_state;
char display_buffer1[NUM_OF_CHARACTERS];

enum
{
   HOME_PAGE         = 1,
   
   ALARM_STATUS,
   ALARM_COUNTDOWN,
   
   PASSWORD_ENTER,
   PASSWORD_VALIDATE,
   
   SENSORS_STATE,
   BATTERY_VOLTAGE
}g_display_state;

/**
 * @brief Show home page to display and checks user keypad input
 */
static void home_page(void)
{
   switch (g_pressed_key)
   {
      case '#':
         g_display_state = BATTERY_VOLTAGE;
         
         lcd_clear();
         break;
      
      case '*':
         g_display_state = SENSORS_STATE;
         
         lcd_clear();
         break;
      
      case '0':
         g_display_state = PASSWORD_ENTER;
         
         lcd_clear();
         break;
      
      default:         
         sprintf(display_buffer1, "Home screen     ");
         send_string(display_buffer1);
         put_cursor(1,0);
         sprintf(display_buffer1, "Alarm -> %s", g_alarm_state);
         send_string(display_buffer1);
         put_cursor(0,0);
         break;
   }
   
   g_pressed_key = NULL;
}

/**
 * @brief Show and change alarm status
 */
static void alarm_status(void)
{  
   switch (g_pressed_key)
   {
      case '#':
         g_alarm_state = "ACTIVE";
         break;
      
      case '*':
         g_alarm_state = "IDLE  ";
         break;
      
      case '0':
         g_display_state = HOME_PAGE;
         lcd_clear();
         break;
      
      default:
         send_string("Set alarm state ");
         put_cursor(1,0);
         send_string(g_alarm_state);
         
         put_cursor(0,0);
         break;
   }
   
   g_pressed_key = NULL;
}

/**
 * @brief Show timer cuntdown seconds
 */
static void alarm_countdown(void)
{
   switch (g_pressed_key)
   {
      case '#':
         /* postaviti da se unese password za ukidanje alarma */
         break;
      
      case '*':
         /* mislim da nije potrebno nista */
         break;     
      
      default:
         /* Napraviti prikaz sekundi */
         sprintf(display_buffer1, "Countdown: %02d s", g_seconds_to_count);
         send_string(display_buffer1);
         put_cursor(0,0);
         break;
   }
   
   g_pressed_key = NULL;
}

/**
 * @brief Show enter passeword screen
 */
static void password_input(void)
{
   static uint8_t character_index = 0; 
   
   switch (g_pressed_key)
   {
      case '#':
         g_display_state = HOME_PAGE;         
         put_cursor(0,0);
         send_string("Password cancel ");
         osDelay(1000);
         character_index = 0;
         lcd_clear();
         break;
      
      case '*':
         g_display_state = PASSWORD_ENTER;
         put_cursor(0,0);
         send_string("Password clear  ");
         osDelay(1000);
         character_index = 0;
         reset_password_input();
         lcd_clear();
         break;
      
      case NULL:
         send_string("Enter password  ");
         put_cursor(0,0);
         break;
      
      default:
         send_string("Enter password  ");
         enter_password(&g_pressed_key, &character_index);
         
         put_cursor(1, character_index);
         send_data(g_pressed_key);
      
         if (character_index >= PASSWORD_LENGTH)
         {
            character_index = 0;
            g_display_state = PASSWORD_VALIDATE;
            
            osDelay(1000);            
            lcd_clear();
         }
         
         put_cursor(0,0);
         break;
   }
   
   g_pressed_key = NULL;
}

/**
 * @brief Show validation of password
 */
static void is_password_valid(void)
{
   if (validate_password())
   {
      send_string("Password OK     ");
      g_display_state = ALARM_STATUS;
   }
   else
   {
      send_string("Password ERROR  ");
      g_display_state = PASSWORD_ENTER;
   }   
   
   osDelay(1000);   
   lcd_clear();
   g_pressed_key = NULL;
}

/**
 * @brief Show sensor states
 */
static void sensor_states(void)
{
   switch (g_pressed_key)
   {
      case '#':
         g_display_state = HOME_PAGE;
         
         lcd_clear();
         break;
      
      case '*':
         g_display_state = BATTERY_VOLTAGE;
         
         lcd_clear();
         break;
      
      default:
         send_string("S1  S2  S3  S4  ");
         
         /* Tu cu ispisivati stvarna stanje digitalnih ulaza (senzora)*/
         char sensor_state[4] = {1,0,1,0};
         char state;
         
         put_cursor(1,1);
         sprintf(&state, "%d", sensor_state[0]);
         send_string(&state); 
         
         put_cursor(1,5);
         sprintf(&state, "%d", sensor_state[1]);
         send_string(&state);
         
         put_cursor(1,9);
         sprintf(&state, "%d", sensor_state[2]);
         send_string(&state);
         
         put_cursor(1,13);
         sprintf(&state, "%d", sensor_state[3]);
         send_string(&state);
         
         put_cursor(0,0);
         break;
   }
   
   g_pressed_key = NULL;
}

/**
 * @brief Show battery voltage
 */
static void battery_voltage(void)
{
   switch (g_pressed_key)
   {
      case '#':
         g_display_state = SENSORS_STATE;
         
         lcd_clear();
         break;
      
      case '*':
         g_display_state = HOME_PAGE;
      
         lcd_clear();
         break;
      
      default:
         send_string("Battery voltage:");
         put_cursor(1,0);
      
         /* Tu cu ispisivati vrijednost od ADC-a, za sada je hardkodirana */
         char voltage = 5;
         char volts;
         sprintf(&volts, "%d V", voltage);
         put_cursor(1,0);
         send_string(&volts);
         
         put_cursor(0,0);
         break;
   }
   
   g_pressed_key = NULL;
}

/**
 * @brief Thread for display state
 */
void display_state_thread(void *arguments)
{
   while(1)
   {
      switch (g_display_state)
      {
         case HOME_PAGE:
            home_page();
            break;
         
         case ALARM_STATUS:
            alarm_status();
            break;
                  
         case ALARM_COUNTDOWN:
            alarm_countdown();
            break;
         
         case PASSWORD_ENTER:
            password_input();
            break;
         
         case PASSWORD_VALIDATE:
            is_password_valid();
            break;
         
         case SENSORS_STATE:
            sensor_states();
            break;
         
         case BATTERY_VOLTAGE:
            battery_voltage();
            break;
         
         default:  
            break;
      }
      
      osDelay(500);
   }
}

/**
 * @brief Initialize display peripheral 
 */
void initialize_display(void)
{
   /* Initialize gloabal varables */   
   g_display_state   = HOME_PAGE;
   g_alarm_state     = "ACTIVE";
   
   /* Configure display */
   configure_display();
   
   /* Add thread in scheduler */
   add_thread(display_state_thread, osPriorityNormal, &display_thread_stack[0], sizeof(display_thread_stack));
}
