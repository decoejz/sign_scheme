#include "no_sign.h"

// RES:
// total_len as it is always the same as msg_len
int verify_no_sign(uint8_t *msg_raw, uint8_t *msg_signed, int total_len, pki_t _public_key)
{
    // TODO: Improve this function to eliminate the memmove
    memmove(msg_raw, msg_signed, total_len);
    return total_len;
}