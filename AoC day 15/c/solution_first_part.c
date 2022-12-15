//usr/bin/gcc solution_first_part.c && exec ./a.out

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define DIM 27

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

struct pos {
	int32_t x, y;
};

struct sensor {
	struct pos sensor_pos;
	struct pos beacon_pos;
	uint32_t distance;
};

uint32_t manhattan_distance(struct pos *a, struct pos *b)
{
	return abs(a->x - b->x) + abs(a->y - b->y);
}

void parse_sensor(char *line, struct sensor *sensor)
{
	int32_t x, y, bx, by;
	sscanf(line, "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d", &x, &y, &bx, &by);
	sensor->sensor_pos.x = x;
	sensor->sensor_pos.y = y;
	sensor->beacon_pos.x = bx;
	sensor->beacon_pos.y = by;
	sensor->distance = manhattan_distance(&sensor->sensor_pos, &sensor->beacon_pos);
}


int main(int argc, char** argv) {
	FILE *fp = fopen("input", "r");

	if (fp == NULL)
		return 1;

	struct sensor sensors[DIM];

	char line[256];
	ssize_t i = 0;
	while (!feof(fp)) {
		if (!fgets(line, sizeof line, fp))
			break;
		line[strlen(line) - 1] = '\0'; // remove newline

		parse_sensor(line, &sensors[i]);

		i++;
	}

	fclose(fp);

	int32_t minx = INT32_MAX, maxx = INT32_MIN;

	for (i = 0; i < DIM; i++) {
		minx = min(minx, sensors[i].sensor_pos.x);
		minx = min(minx, sensors[i].beacon_pos.x);
		maxx = max(maxx, sensors[i].sensor_pos.x);
		maxx = max(maxx, sensors[i].beacon_pos.x);
	}

	uint32_t max_distance = 0;
	for (i = 0; i < DIM; i++)
		max_distance = max(max_distance, sensors[i].distance);

	struct pos tpos = {0, 0};
	tpos.y = 2000000;
	uint8_t valid, is_beacon;
	uint32_t count = 0;

	minx -= max_distance;
	maxx += max_distance;

	for (i = minx; i < maxx; i++) {
		tpos.x = i;
		valid = 1;
		is_beacon = 0;

		for (size_t j = 0; j < DIM && !is_beacon; j++)
			if (tpos.x == sensors[j].beacon_pos.x && tpos.y == sensors[j].beacon_pos.y)
				is_beacon = 1;

		for (size_t j = 0; j < DIM && valid && !is_beacon; j++)
			if (manhattan_distance(&tpos, &sensors[j].sensor_pos) <= sensors[j].distance)
				valid = 0;

		if (!valid && !is_beacon)
			count++;
	}

	printf("Total: %u\n", count);

	return 0;
}
