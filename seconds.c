#include "seconds.h"
#include <stdio.h>

#define SECONDS_PER_HOUR 3600
#define SECONDS_PER_MINUTE 60
#define TIME_BUFSIZE 9

static int format_seconds(char str[], seconds_t seconds);

void print_seconds(seconds_t seconds)
{
	char buf[TIME_BUFSIZE];
	if (format_seconds(buf, seconds) == 0) {
		printf("%-8s", buf);
	}
}

static int format_seconds(char str[], seconds_t seconds)
{
	struct seconds_parts parts;
	parts_from_seconds(&parts, seconds);
	seconds_t h, m, s;
	h = parts.sp_hours;
	m = parts.sp_minutes;
	s = parts.sp_seconds;
	if (h > 99 || m > 99 || s > 99) {
		str[0] = '\0';
		return -1;
	}

	int bw; /* bytes written */
	if (h > 0) {
		bw = snprintf(str, TIME_BUFSIZE, "%02ld:%02ld:%02ld", h, m, s);
	} else {
		bw = snprintf(str, TIME_BUFSIZE, "%02ld:%02ld", m, s);
	}
	str[TIME_BUFSIZE - 1] = '\0';
	if (bw >= TIME_BUFSIZE) {
		return -1;
	}

	return 0;
}

void parts_from_seconds(struct seconds_parts *parts, seconds_t seconds)
{
	seconds_t h, m, s;

	h = seconds / SECONDS_PER_HOUR;
	m = (seconds - h * SECONDS_PER_HOUR) / SECONDS_PER_MINUTE;
	s = seconds - h * SECONDS_PER_HOUR - m * SECONDS_PER_MINUTE;

	parts->sp_hours = h;
	parts->sp_minutes = m;
	parts->sp_seconds = s;
}
