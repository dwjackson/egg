#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "parse.h"

static seconds_t parse_number(const char *str, char **endptr);
static seconds_t parse_minutes(const char *str, char **endptr, seconds_t n);

seconds_t parse_time(const char *str)
{
	long int seconds;
	long int number;
	char *next;
	number = parse_number(str, &next);
	
	seconds = 0;
	if (*next == '\0' || *next == 's') {
		seconds = number;
	} else if (*next == 'h') {
		seconds += number * 60 * 60;
		next++;
		if (*next == '\0') {
			return seconds;
		}
		number = parse_number(next, &next);
		number = parse_minutes(next, &next, number);
		if (number > 0) {
			seconds += number;
		} else {
			seconds = -1;
		}
	} else if (*next == 'm') {
		next++;
		number = parse_minutes(next, &next, number);
		if (number > 0) {
			seconds = number;
		} else {
			seconds = -1;
		}
	} else {
		seconds = -1;
	}

	return seconds;
}

static seconds_t parse_number(const char *str, char **endptr)
{
	long int num = strtol(str, endptr, 10);
	return num;
}

static seconds_t parse_minutes(const char *str, char **endptr, seconds_t n)
{
	size_t seconds = 0;
	seconds += n * 60;
	if (*str == '\0') {
		return seconds;
	}
	n = parse_number(str, endptr);
	seconds += n;
	if (**endptr == 'm') {
		(*endptr)++;
		n = parse_number(*endptr, endptr);
		if (n < 0) {
			return -1;
		} else {
			seconds += n;
		}
	} else if (!(**endptr == 's' || **endptr == '\0')) {
		seconds = -1;
	}
	return seconds;
}
