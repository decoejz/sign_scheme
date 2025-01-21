#include <stdint.h>

int uc2int(uint8_t *res)
{
    return (res[0] << 24) | (res[1] << 16) | (res[2] << 8) | res[3];
}