#ifndef TIMER_H
#define TIMER_H

#include "seconds.h"

struct timer;

typedef void (*timer_callback)(struct timer *timer, void *arg);

struct timer_callbacks {
	timer_callback each_tick;
	timer_callback at_end;
};

struct timer {
	seconds_t seconds;
	seconds_t remaining;
	char *message;
	struct timer_callbacks callbacks;
};

void timer_start(struct timer *timer);

#endif /* TIMER_H */
