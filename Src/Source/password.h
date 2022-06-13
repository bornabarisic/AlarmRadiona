#ifndef __PASSWORD_H
#define __PASSWORD_H

#include <stdint.h>
#include <stdbool.h>

#define PASSWORD_LENGTH    4

extern void reset_password_input(void);
extern void enter_password(char *ch, uint8_t *password_index);
extern bool validate_password(void);

#endif
