#ifndef EDDSA_H
#define EDDSA_H

// https://docs.openssl.org/3.0/man7/EVP_SIGNATURE-EDDSA/

#ifdef __cplusplus
extern "C"
{
#endif

#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <stdio.h>

#include "sign_scheme_definitions.h"
#include "../helper/helper.h"

#define EDDSA_SIGN_HEADER_SIZE 4 // int size
// #define EDDSA_SIGN_MAX_LEN 2048

    int key_gen_eddsa(const char *secret_name, const char *public_name);
    pki_t read_key_eddsa(char load_type, const char *file_name);

    int verify_eddsa(uint8_t *msg_raw, uint8_t *msg_signed, int total_len, pki_t public_key);
    int sign_eddsa(uint8_t *msg_signed, uint8_t *msg_raw, unsigned int msg_len, pki_t secret_key);

#ifdef __cplusplus
}
#endif

#endif // EDDSA_H