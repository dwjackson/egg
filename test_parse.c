#include "parse.h"
#include <assert.h>

int main()
{
	int seconds;

	seconds = parse_time("35");
	assert(seconds == 35);

	seconds = parse_time("1m");
	assert(seconds == 60);

	seconds = parse_time("1m30s");
	assert(seconds == 90);

	seconds = parse_time("1m30z");
	assert(seconds == -1);

	seconds = parse_time("2h");
	assert(seconds == 7200);

	seconds = parse_time("2h30m45s");
	assert(seconds == 9045);

	seconds = parse_time("49s");
	assert(seconds == 49);

	return 0;
}
