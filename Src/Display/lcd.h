#ifndef __LCD_H
#define __LCD_H

#include <stdint.h>

extern void configure_display (void);
extern void send_data         (char data);
extern void send_string       (char *str);
extern void lcd_clear         (void);
extern void put_cursor        (uint32_t row, uint32_t column);

#endif
