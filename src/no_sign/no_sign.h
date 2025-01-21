#ifndef NO_SIGN_H
#define NO_SIGN_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "sign_scheme_definitions.h"

#define NO_SIGN_SIGN_HEADER_SIZE 0 // int size
#define NO_SIGN_SIGN_MAX_LEN 0

    int key_gen_no_sign(const char *_secret_name, const char *_public_name);
    pki_t read_key_no_sign(char _load_type, const char *_file_name);

    int verify_no_sign(uint8_t *msg_raw, uint8_t *msg_signed, int total_len, pki_t _public_key);
    int sign_no_sign(uint8_t *msg_signed, uint8_t *msg_raw, unsigned int msg_len, pki_t _secret_key);

#ifdef __cplusplus
}
#endif


#endif // NO_SIGN_H