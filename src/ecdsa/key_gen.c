// https://docs.openssl.org/3.0/man7/EVP_PKEY-EC/

#include "ecdsa.h"

#ifdef CONFIG_EMBEDDED
int key_gen_ecdsa(const char *secret_name, const char *public_name)
{
    return 1;
}
#else
int key_gen_ecdsa(const char *secret_name, const char *public_name)
{
    FILE *pk_file, *sk_file;

    // Generate keys
    EVP_PKEY *pkey = EVP_EC_gen("P-256");
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
#endif