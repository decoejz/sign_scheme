#ifndef HELPER_H
#define HELPER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

int int2uc(uint8_t *res, int num);
int uc2int(uint8_t *res);


#ifdef __cplusplus
}
#endif

#endif // HELPER_H