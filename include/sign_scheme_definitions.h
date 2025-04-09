#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#ifdef __cplusplus
extern "C"
{
#endif

// For the use of read_key function
#define PRIVATE_KEY 0
#define PUBLIC_KEY 1

// #######################
#define SIGN_SCHEME_NONE 0
#define SIGN_SCHEME_RSA 1
#define SIGN_SCHEME_ECDSA 2
#define SIGN_SCHEME_EDDSA 3

#define GroundControl 0
#define Autopilot 1

#define SIGN_PKT 0
#define VERIFY_PKT 1

#define BEFORE 0
#define AFTER 1

#define INVALID 0
#define VALID 1
#define NA 2
// #######################
typedef struct pki pki_t;
#ifdef __cplusplus
}
#endif

#endif // DEFINITIONS_H