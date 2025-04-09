
#ifndef UTILSIGN_H
#define UTILSIGN_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <sign_scheme_definitions.h>
#include <openssl/ossl_typ.h>

struct pki
{
    char *char_key;
    EVP_PKEY *openssl_key;
} /* pki_t */;

#define ECDSA_SIGN_HEADER_SIZE 4 // int size
#define ECDSA_SIGN_MAX_LEN 2048

#define EDDSA_SIGN_HEADER_SIZE 4 // int size
#define EDDSA_SIGN_MAX_LEN 2048 // TODO CHECH

#define NO_SIGN_SIGN_HEADER_SIZE 0 // int size
#define NO_SIGN_SIGN_MAX_LEN 0

#define RSA_SIGN_HEADER_SIZE 4
#define RSA_SIGN_MAX_LEN 2048

#ifdef __cplusplus
}
#endif

#endif // UTILSIGN_H