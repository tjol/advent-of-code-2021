#include <stdio.h>
#include <stdlib.h>

#define BITS (8 * sizeof(unsigned))

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

int most_common_bit(int bitidx, const unsigned numbers[], size_t size)
{
    int ones = 0;
    int zeros = 0;
    unsigned bitmask = 1 << bitidx;

    for (size_t n = 0; n < size; ++n) {
        if (numbers[n] & bitmask) {
            ++ones;
        } else {
            ++zeros;
        }
    }
    return (ones >= zeros);
}

int main()
{
    unsigned *numbers;
    size_t size;
    int nbits = parse(stdin, &numbers, &size);

    unsigned int gamma = 0;
    unsigned int epsilon = 0;
    for (int i = nbits - 1;i >= 0; --i) {
        gamma <<= 1;
        epsilon <<= 1;
        if (most_common_bit(i, numbers, size)) {
            gamma |= 1;
        } else {
            epsilon |= 1;
        }
    }

    // printf("gamma = %u, epsilon = %u\n", gamma, epsilon);
    printf("%u\n", gamma * epsilon);

    return 0;
}