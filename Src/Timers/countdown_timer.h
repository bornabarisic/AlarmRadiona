#ifndef __COUTDOWN_TIMER_H
#define __COUTDOWN_TIMER_H

#include <stdint.h>

extern uint8_t g_seconds_to_count;

extern void initialize_countdown_timer(void);
extern void start_countdown_timer(void);
extern void stop_countdown_timer(void);

#endif
