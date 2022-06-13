#include <string.h>

#include "password.h"

#define CORRECT_PASSWORD      "1234"

char g_password[PASSWORD_LENGTH];

void reset_password_input(void)
{
   memset((void *)&g_password[0], NULL, sizeof(g_password));
}

void enter_password(char *ch, uint8_t *password_index)
{   
   g_password[*password_index] = *ch;
   
   *password_index += 1;
}

bool validate_password(void)
{
   if (strncmp(g_password, CORRECT_PASSWORD, PASSWORD_LENGTH) != 0)
   {
      return false;
   }
   else
   {
      return true;
   }   
}
