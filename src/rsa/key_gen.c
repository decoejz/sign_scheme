// https://stackoverflow.com/questions/68102808/how-to-use-openssl-3-0-rsa-in-c
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <stdio.h>

#include "rsa.h"

int key_gen_rsa(const char *secret_name, const char *public_name)
{
    int size = 2048;

    FILE *pk_file, *sk_file;

    // Generate keys
    EVP_PKEY *pkey = EVP_RSA_gen(size);
    if (pkey == NULL)
    {
        return 0;
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

    return 1;
}