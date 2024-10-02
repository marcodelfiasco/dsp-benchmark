#ifndef TIMER_H_
#define TIMER_H_

void timer_init(void);
void timer_setup(void (*callback)(int user_arg), int user_arg, int period_usec);
void timer_start(void);
void timer_stop(void);

#endif // TIMER_H_
