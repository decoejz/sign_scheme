#ifndef ECDSA_H
#define ECDSA_H

// https://docs.openssl.org/3.0/man7/EVP_SIGNATURE-ECDSA/

#ifdef __cplusplus
extern "C"
{
#endif

#include "utilsign.h"

#ifndef CONFIG_EMBEDDED
#include <stdio.h>
#endif
#include <stdlib.h>

#include "helper.h"
#include "io.h"

int key_gen_ecdsa(const char *secret_name, const char *public_name);
pki_t read_key_ecdsa(char load_type, const char *file_name);

int verify_ecdsa(uint8_t *msg_raw, uint8_t *msg_signed, int total_len, pki_t public_key);
int sign_ecdsa(uint8_t *msg_signed, uint8_t *msg_raw, unsigned int msg_len, pki_t secret_key);

#ifdef __cplusplus
}
#endif

#endif // ECDSA_H