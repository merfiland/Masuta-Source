#define _GNU_SOURCE

#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include "includes.h"
#include "rand.h"
#include "util.h"

static uint32_t x, y, z, w;

void rand_init(void)
{
    x = time(NULL);
    y = getpid() ^ getppid();
    z = clock();
    w = z ^ y;
}

uint32_t rand_next(void)
{
    uint32_t t = x;
    t ^= t << 11;
    t ^= t >> 8;
    x = y; y = z; z = w;
    w ^= w >> 19;
    w ^= t;
    return w;
}

void rand_alpha_str(uint8_t *str, int len)
{
    char alpha_set[] = "g1abc4dmo35hnp2lie0kjf";

    while(len--)
        *str++ = alpha_set[rand_next() % util_strlen(alpha_set)];
}
