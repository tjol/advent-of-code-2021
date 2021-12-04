#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define BITS (8 * ((int)sizeof(unsigned)))

int parse(FILE *stream, unsigned **numbersp, size_t *sizep)
{
    // Create array
    size_t size = 1000;
    unsigned *numbers = malloc(size * sizeof(unsigned));

    size_t k = 0;
    int nbits = 0;
    
    // set up
    size_t strbuf_size = 256;
    char *strbuf = malloc(strbuf_size);

    ssize_t line_len = 0;
    while ((line_len = getline(&strbuf, &strbuf_size, stream)) > 0) {
        numbers[k] = 0;
        for (int i = 0; i < line_len && i < BITS; ++i) {
            int bit = strbuf[i] == '1';
            if (strbuf[i] == '1' || strbuf[i] == '0') {
                numbers[k] <<= 1;
                if ((i+1) > nbits) ++nbits;
                numbers[k] |= bit;
            }
        }
        if ((++k) >= size) {
            size *= 2;
            numbers = realloc(numbers, size * sizeof(unsigned));
        }
    }
    free(strbuf);

    // Shrink the array
    size = k;
    numbers = realloc(numbers, size * sizeof(unsigned));

    *numbersp = numbers;
    *sizep = size;
    return nbits;
}

unsigned *partition_by_bit(unsigned *begin, unsigned *end, unsigned bitmask)
{
    --end;
    for (;;) {
        while (*begin & bitmask) ++begin;
        while (!(*end & bitmask)) --end;
        if (begin >= end) return begin;
        unsigned tmp = *begin;
        *begin = *end;
        *end = tmp;
    }
}

unsigned rating(const unsigned numbers[], size_t size, int nbits, bool o2_mode)
{
    // clone the array
    unsigned *array = malloc(size * sizeof(unsigned));
    memcpy(array, numbers, size * sizeof(unsigned));

    unsigned *begin = array;
    unsigned *end = array + size;
    
    for (int bit = nbits - 1; bit >= 0; --bit) {
        unsigned *mid = partition_by_bit(begin, end, 1 << bit);
        int ones = mid - begin;
        int zeros = end - mid;
        if ((o2_mode && ones >= zeros) || (!o2_mode && ones < zeros) || (zeros == 0)) {
            // keep the ones
            end = mid;
            if (ones == 1) break; // done
        } else {
            // keep the zeros
            begin = mid;
            if (zeros == 1) break; // done
        }
    }

    unsigned rating = *begin;

    free(array);
    return rating;
}

int main()
{
    unsigned *numbers;
    size_t size;
    int nbits = parse(stdin, &numbers, &size);

    unsigned o2_rating = rating(numbers, size, nbits, true);
    unsigned co2_rating = rating(numbers, size, nbits, false);

    // printf("O2: %u, CO2: %u\n", o2_rating, co2_rating);
    printf("%u\n", o2_rating * co2_rating);

    return 0;
}
