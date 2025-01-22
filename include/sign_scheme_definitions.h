#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <openssl/pem.h>

// For the use of read_key function
#define PRIVATE_KEY 0
#define PUBLIC_KEY 1

// #######################
#define NO_SIGN 0
#define RSA 1
#define ECDSA 2
#define EDDSA 3

#define QGC 0
#define PX4 1

#define SIGN 0
#define VERIFY 1

#define BEFORE 0
#define AFTER 1

#define INVALID 0
#define VALID 1
#define NA 2
// #######################

    typedef union pki
    {
        char *char_key;
        EVP_PKEY *openssl_key;
    } pki_t;

#ifdef __cplusplus
}
#endif

#endif // DEFINITIONS_H