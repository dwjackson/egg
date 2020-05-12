#include "timer.h"
#include <time.h>
#include <unistd.h>

static time_t run_timer(struct timer *timer);

void timer_start(struct timer *timer)
{
	time_t start_time = run_timer(timer);
	if (timer->callbacks.at_end != NULL) {
		timer->callbacks.at_end(timer, &start_time);
	}
}

static time_t run_timer(struct timer *timer)
{
	time_t start_time = time(NULL);
	timer->remaining = timer->seconds;
	int i;
	for (i = 0; i < timer->seconds; i++, (timer->remaining)--) {
		if (timer->callbacks.each_tick != NULL) {
			timer->callbacks.each_tick(timer, NULL);
		}
		sleep(1);
	}
	return start_time;
}
