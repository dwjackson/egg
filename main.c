#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

#include "parse.h"
#include "timer.h"

#define USAGE_FMT "Usage: %s [OPTIONS...] [TIME]\n"
#define MAX_SECONDS (99 * 3600 + 99 * 60 + 99)
#define MAX_PART 99

static void timer_complete(time_t start_time);
static void print_elapsed(seconds_t elapsed);
static void play_chime();
static int seconds_are_valid(seconds_t seconds);
static void show_help(const char* progname);

/* Timer callbacks */
static void each_tick(struct timer *timer, void *arg);
static void at_end(struct timer *timer, void *arg);

/* Globals */
int end_after_seconds = -1;
bool display_as_seconds = false;
bool is_quiet = false;

int main(int argc, char *argv[])
{
	extern char *optarg;
	extern int optind;
	int opt;
	struct timer timer;

	timer.message = NULL;
	timer.callbacks.each_tick = each_tick;
	timer.callbacks.at_end = at_end;

	while ((opt = getopt(argc, argv, "e:hm:sq")) != -1) {
		if (opt == 'm') {
			timer.message = optarg;
		} else if (opt == 'e') {
			end_after_seconds = atoi(optarg);
			if (end_after_seconds < 0) {
				printf("Bad argument: Stop-after seconds");
				exit(EXIT_FAILURE);
			}
		} else if (opt == 'h') {
			show_help(argv[0]);
			exit(EXIT_SUCCESS);
		} else if (opt == 's') {
			display_as_seconds = true;
		} else if (opt == 'q') {
			is_quiet = true;
		}
	}

	if (optind >= argc) {
		printf(USAGE_FMT, argv[0]);
		exit(EXIT_FAILURE);
	}

	timer.seconds = parse_time(argv[optind]);
	if (!seconds_are_valid(timer.seconds)) {
		printf("Invalid time\n");
		exit(EXIT_FAILURE);
	}

	timer_start(&timer);

	return 0;
} 

static void each_tick(struct timer *timer, void *arg)
{
	(void) arg;

	if (is_quiet) {
		/* Quiet mode, do not print anything */
		return;
	}
	printf("\r");
	if (display_as_seconds) {
		printf("%ld", timer->remaining);
	} else {
		print_seconds(timer->remaining);
	}
	fflush(stdout);
}

static void timer_complete(time_t start_time)
{
	struct tm start_tm;
	struct tm end_tm;
	time_t end_time;
	char start_str[9];
	char end_str[9];
	int i;

	if (is_quiet) {
		/* Quiet mode, print nothing & make no noise */
		return;
	}

	end_time = time(NULL);
	localtime_r(&start_time, &start_tm);
	localtime_r(&end_time, &end_tm);
	strftime(start_str, 9, "%T", &start_tm);
	strftime(end_str, 9, "%T", &end_tm);

	printf("\r");
	printf("Timer complete, use CTRL+C to stop chime\n");
	printf("Timer ran from %s to %s\n", start_str, end_str);
	seconds_t elapsed = 0;
	if (end_after_seconds == 0) {
		play_chime();
	} else if (end_after_seconds > 0) {
		for (i = 0; i <= end_after_seconds; i++, elapsed++) {
			play_chime();
			print_elapsed(elapsed);
		}
		printf("\n");
	} else {
		while (1) {
			play_chime();
			print_elapsed(elapsed);
			elapsed++;
		}
	}
}

static void print_elapsed(seconds_t elapsed)
{
	printf("\r");
	if (display_as_seconds) {
		printf("%ld", elapsed);
	} else {
		print_seconds(elapsed);
	}
	fflush(stdout);
}

static void play_chime()
{
	printf("\a");
	sleep(1);
}

static int seconds_are_valid(seconds_t seconds)
{
	struct seconds_parts parts;
	parts_from_seconds(&parts, seconds);
	return seconds >= 0 && seconds <= MAX_SECONDS
		&& parts.sp_hours <= MAX_PART && parts.sp_minutes <= MAX_PART
		&& parts.sp_seconds <= MAX_PART;
}

static void at_end(struct timer *timer, void *arg)
{
	(void) timer;

	time_t *ptr = (time_t *) arg;
	time_t start_time = *ptr;

	if (timer->message != NULL) {
		puts(timer->message);
	}

	timer_complete(start_time);
}

static void show_help(const char *progname)
{
	printf(USAGE_FMT, progname);

	printf("Options:\n");
	printf("\t-e [SECONDS] - stop the alarm chime after SECONDS seconds\n");
	printf("\t-h - display this help message\n");
	printf("\t-m [MESSAGE] - display the given message when the timer ends\n");
	printf("\t-s - display remaining time in seconds\n");
	printf("\t-q - do not display remaining time\n");

	printf("Time Strings:\n");
	printf("\tThe strings are in the format [HOURS]h[MINUTES]m[SECONDS]s\n");
	printf("\tIf a part of the string is omitted it is assumed to be zero\n");
}
