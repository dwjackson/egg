#ifndef SECONDS_H
#define SECONDS_H

typedef long int seconds_t;

struct seconds_parts {
	seconds_t sp_hours;
	seconds_t sp_minutes;
	seconds_t sp_seconds;
};

void print_seconds(seconds_t seconds);
void parts_from_seconds(struct seconds_parts *parts, seconds_t seconds);

#endif /* SECONDS_H */
