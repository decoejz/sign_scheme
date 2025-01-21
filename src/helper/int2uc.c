#include "helper.h"

int int2uc(uint8_t *res, int num)
{
    res[0] = num >> 24;
    res[1] = num >> 16;
    res[2] = num >> 8;
    res[3] = num;
    return 0;
}