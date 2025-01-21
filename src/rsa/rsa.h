#ifndef RSA_H
#define RSA_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <stdio.h>

#include "sign_scheme_definitions.h"
#include "../helper/helper.h"

#define RSA_SIGN_HEADER_SIZE 4
#define RSA_SIGN_MAX_LEN 2048

    int key_gen_rsa(const char *secret_name, const char *public_name);
    pki_t read_key_rsa(char load_type, const char *file_name);

    int verify_rsa(uint8_t *msg_raw, uint8_t *msg_signed, int total_len, pki_t public_key);
    int sign_rsa(uint8_t *msg_signed, uint8_t *msg_raw, unsigned int msg_len, pki_t secret_key);

#ifdef __cplusplus
}
#endif

#endif // RSA_H