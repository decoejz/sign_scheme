#ifndef SIGN_SCHEME_H
#define SIGN_SCHEME_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdlib.h>
#include <stdint.h>
#include "sign_scheme_definitions.h"

// TODO: Improve to get only the biggest
#define MAX_SIGN_HEADER_SIZE (ECDSA_SIGN_HEADER_SIZE + NO_SIGN_SIGN_HEADER_SIZE + RSA_SIGN_HEADER_SIZE)
#define MAX_SIGN_MAX_LEN (ECDSA_SIGN_MAX_LEN + NO_SIGN_SIGN_MAX_LEN + RSA_SIGN_MAX_LEN)

int key_gen(const char *secret_name, const char *public_name);
pki_t read_key(char load_type);
int sign(uint8_t *msg_signed, uint8_t *msg_raw, unsigned int msg_len, pki_t secret_key);
int verify(uint8_t *msg_raw, uint8_t *msg_signed, int total_len, pki_t public_key);
void sign_log(const char *msg);

#ifdef __cplusplus
}
#endif

#endif // SIGN_SCHEME_H