//usr/bin/gcc solution_second_part.c -O3 -march=native && exec ./a.out

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
	size_t i = 0;
	while (!feof(fp)) {
		if (!fgets(line, sizeof line, fp))
			break;
		line[strlen(line) - 1] = '\0'; // remove newline

		parse_sensor(line, &sensors[i]);

		i++;
	}

	fclose(fp);
	struct pos tpos;
	uint8_t valid, is_beacon, found = 0;

	for (tpos.x = 0; tpos.x < 4000000 && !found; tpos.x++) {
		for (tpos.y = 0; tpos.y < 4000000; tpos.y++) {
			valid = 1;
			is_beacon = 0;

			for (size_t j = 0; j < DIM && !is_beacon; j++)
				if (tpos.x == sensors[j].beacon_pos.x && tpos.y == sensors[j].beacon_pos.y)
					is_beacon = 1;

			for (size_t j = 0; j < DIM && valid && !is_beacon; j++) {
				if (manhattan_distance(&tpos, &sensors[j].sensor_pos) <= sensors[j].distance) {
					valid = 0;
					if (tpos.y >= sensors[j].sensor_pos.y) {
						tpos.y += sensors[j].distance - manhattan_distance(&tpos, &sensors[j].sensor_pos);
					} else {
						tpos.y = 2 * sensors[j].sensor_pos.y - tpos.y; // good old simmetria
					}
				}
			}

			if (valid && !is_beacon) {
				found = 1;
				printf("Frequency: %lld\n", (uint64_t)(tpos.x) * 4000000 + (uint64_t)(tpos.y));
			}
		}
	}

	return 0;
}
