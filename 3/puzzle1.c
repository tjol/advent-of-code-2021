#include <stdio.h>
#include <stdlib.h>

#define BITS 64

struct bitcount {
	int zeros;
	int ones;
};

struct bitcount counters[BITS];

void parse(FILE *stream)
{
	// reset
	for (int i=0; i<BITS; ++i) counters[i] = (struct bitcount){0, 0};
	
	// set up
	size_t len = 256;
	char *buffer = malloc(len);

	ssize_t line_len = 0;
	while ((line_len = getline(&buffer, &len, stream)) > 0) {
		for (int i = 0; i < line_len && i < BITS; ++i) {
			switch(buffer[i]) {
			case '1':
				counters[i].ones++;
				break;
			case '0':
				counters[i].zeros++;
				break;
			}
		}
	}
	free(buffer);
}

int main()
{
	parse(stdin);
	unsigned int gamma = 0;
	unsigned int epsilon = 0;
	for (int i = 0;counters[i].ones || counters[i].zeros;++i) {
		gamma <<= 1;
		epsilon <<= 1;
		if (counters[i].ones > counters[i].zeros) {
			gamma |= 1;
		} else {
			epsilon |= 1;
		}
	}

	//printf("gamma = %u, epsilon = %u\n", gamma, epsilon);
	printf("%u\n", gamma * epsilon);

	return 0;
}