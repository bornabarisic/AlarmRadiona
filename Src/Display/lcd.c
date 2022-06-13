#include "utility.h"
#include "lcd.h"

#define DISPLAY_ADDRESS    0x4E
#define NIBBLE_BYTE_NUM    0x04
#define TIMEOUT            0x64

/* Global varables */
I2C_HandleTypeDef i2c_handle;

/**
 * @brief Initialize I2C peripheral for display
 */
static void init_i2c_peripheral(void)
{
   __HAL_RCC_I2C1_CLK_ENABLE();
   
   set_gpio_pin_alternate(GPIOB, GPIO_PIN_8|GPIO_PIN_9, GPIO_MODE_AF_OD, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_NOPULL, GPIO_AF4_I2C1);
   
   i2c_handle.Instance              = I2C1;
   i2c_handle.Init.ClockSpeed       = 100000;
   i2c_handle.Init.DutyCycle        = I2C_DUTYCYCLE_2;
   i2c_handle.Init.OwnAddress1      = 0x00;
   i2c_handle.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
   i2c_handle.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
   i2c_handle.Init.OwnAddress2      = 0x00;
   i2c_handle.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
   i2c_handle.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;
   i2c_handle.Mode                  = HAL_I2C_MODE_MASTER;
   
   if (HAL_I2C_Init(&i2c_handle) != HAL_OK)
   {
      while(1);
   }   
}

/**
 * @brief Send a command to display
 */
static void send_command(char cmd)
{
   char data_h;
   char data_l;
   uint8_t data_t[4];
   
   data_h = (cmd << 0) & 0xF0;
   data_l = (cmd << 4) & 0xF0;
   
   data_t[0] = data_h|0x0C;
   data_t[1] = data_h|0x08;
   data_t[2] = data_l|0x0C;
   data_t[3] = data_l|0x08;
   
   HAL_I2C_Master_Transmit(&i2c_handle, DISPLAY_ADDRESS, (uint8_t *)&data_t[0], NIBBLE_BYTE_NUM, TIMEOUT);
}

/**
 * @brief Send data to display
 */
void send_data(char data)
{
   char data_h;
   char data_l;
   uint8_t data_t[4];
   
   data_h = (data << 0) & 0xF0;
   data_l = (data << 4) & 0xF0;
   
   data_t[0] = data_h|0x0D;
   data_t[1] = data_h|0x09;
   data_t[2] = data_l|0x0D;
   data_t[3] = data_l|0x09;
   
   HAL_I2C_Master_Transmit(&i2c_handle, DISPLAY_ADDRESS, (uint8_t *)&data_t[0], NIBBLE_BYTE_NUM, TIMEOUT);
}

/**
 * @brief Send array of characters to display
 */
void send_string(char *str)
{
   while(*str)
   {
      send_data(*str++);
   }
}

/**
 * @brief Put a cursor to specific location on display
 */
void put_cursor(uint32_t row, uint32_t column)
{
   switch(row)
   {
      case 0:
         column |= 0x80;
         break;
      case 1:
         column |= 0xC0;
         break;
   }
   
   send_command(column);
}

/**
 * @brief Clear all data on display
 */
void lcd_clear(void)
{
   send_command(0x80);
   
   for(uint32_t i=0; i<70; i++)
   {
      send_data(' ');
   }
   
   put_cursor(0,0);
}

/**
 * @brief Configure display
 */
void configure_display(void)
{
   init_i2c_peripheral();
      
   osDelay(50);
   send_command(0x03);
   osDelay(5);
   send_command(0x03);
   osDelay(1);
   send_command(0x03);
   osDelay(10);
   send_command(0x02);
   osDelay(10);
   
   send_command(0x28);
   osDelay(1);
   send_command(0x08);
   osDelay(1);
   send_command(0x01);
   osDelay(2);
   send_command(0x06);
   osDelay(1);
   send_command(0x0C);
   
   lcd_clear();
   osDelay(5000);
   
   send_string("Initialization :");
   put_cursor(1,0);
   send_string("Done");
   
   osDelay(2000);   
   lcd_clear();
}
