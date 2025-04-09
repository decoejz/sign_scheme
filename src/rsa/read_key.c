#include "rsa.h"

#ifdef CONFIG_EMBEDDED
pki_t read_key_rsa(char load_type, const char *file_name)
{   
    pki_t key_res = {0};
    FIL *file = open_file(file_name);
    if (file == NULL)
    {
        log_error("Read key error: Cannot open the file");
        return key_res;
    }
    
    char key_pem[RSA_SIGN_MAX_LEN];
    read_file(file, key_pem);  
    
    BIO *bio = NULL;
    bio = BIO_new_mem_buf(key_pem, -1);
    if (!bio) {
        log_error("BIO allocation failed");
        return key_res;
    }

    EVP_PKEY *key = NULL;
    if (load_type == PRIVATE_KEY) {
        key = PEM_read_bio_PrivateKey(bio, NULL, NULL, NULL);
    } else if (load_type == PUBLIC_KEY) {
        key = PEM_read_bio_PUBKEY(bio, NULL, NULL, NULL);
    } else {
        log_error("Read key error: Invalid key type");
    }

    BIO_free(bio);
    key_res.openssl_key = key;
    return key_res;
}
#else
pki_t read_key_rsa(char load_type, const char *file_name)
{
    pki_t key_res;
    EVP_PKEY *key = NULL;
    FILE *fp = open_file(file_name);
    if (fp == NULL)
    {
        log_error("Read key error: Cannot open the file");
        exit(EXIT_FAILURE);
    }

    if (load_type == PRIVATE_KEY)
    {

        key = PEM_read_PrivateKey(fp, NULL, NULL, NULL);
    }
    else if (load_type == PUBLIC_KEY)
    {
        key = PEM_read_PUBKEY(fp, NULL, NULL, NULL);
    }
    else
    {
        log_error("Read key error: Invalid input");
    }

    if (!key)
    {
        char buffer[124];
        sprintf(buffer, "Read key error: Unable to load key from file %s\n", file_name);
        log_error(buffer);
    }
    fclose(fp);

    key_res.openssl_key = key;
    return key_res;
}
#endif