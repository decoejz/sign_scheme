// https://docs.openssl.org/3.2/man3/EVP_PKEY_new/#description#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <stdio.h>

#include "eddsa.h"

int key_gen_eddsa(const char *secret_name, const char *public_name)
{
    FILE *pk_file, *sk_file;
    EVP_PKEY *pkey = NULL;
    EVP_PKEY_CTX *pctx = NULL;

    pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_ED25519, NULL);
    if (!pctx)
    {
        fprintf(stderr, "EVP_PKEY_CTX_new_id failed\n");
        return -1;
    }

    if (EVP_PKEY_keygen_init(pctx) <= 0)
    {
        fprintf(stderr, "EVP_PKEY_keygen_init failed\n");
        return -1;
    }

    // Generate keys
    if (EVP_PKEY_keygen(pctx, &pkey) <= 0)
    {
        fprintf(stderr, "EVP_PKEY_keygen failed\n");
        return -1;
    }

    // Write secret key
    sk_file = fopen(secret_name, "w"); // secret key file
    if (sk_file != NULL)
    {
        PEM_write_PrivateKey(sk_file, pkey, NULL, NULL, 0, NULL, NULL);
    }
    else
    {
        perror("file error\n");
    }
    fclose(sk_file);

    // Write Public key
    pk_file = fopen(public_name, "w"); // public key file
    if (pk_file != NULL)
    {
        PEM_write_PUBKEY(pk_file, pkey);
    }
    else
    {
        perror("file error\n");
    }
    fclose(pk_file);

    // End program
    EVP_PKEY_free(pkey);
    EVP_PKEY_CTX_free(pctx);
    EVP_cleanup();

    return 1;
}