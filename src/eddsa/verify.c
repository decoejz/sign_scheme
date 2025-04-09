#include "eddsa.h"

#ifdef CONFIG_EMBEDDED
int verify_eddsa(uint8_t *msg_raw, uint8_t *msg_signed, int total_len, pki_t public_key)
{
    if (public_key.openssl_key == NULL)
    {
        log_error("Validation ERROR - key is NULL\n");
        return -1;
    }

    int sigma_len = uc2int(msg_signed);
    int msg_len = total_len - EDDSA_SIGN_HEADER_SIZE - sigma_len;
    unsigned char sigma[sigma_len];

    // (siglen (int) + msg + sigma)
    memcpy(msg_raw, msg_signed + EDDSA_SIGN_HEADER_SIZE, msg_len);
    memcpy(sigma, msg_signed + EDDSA_SIGN_HEADER_SIZE + msg_len, sigma_len);

    EVP_MD_CTX *md_ctx = EVP_MD_CTX_new();
    if (!md_ctx)
    {
        log_error("Validation ERROR - Failed to create EVP_MD_CTX_new\n");
        return -1;
    }

    if (EVP_DigestVerifyInit(md_ctx, NULL, NULL, NULL, public_key.openssl_key) != 1)
    {
        log_error("Validation ERROR - Failed to initialize EVP_PKEY_CTX\n");
        EVP_MD_CTX_free(md_ctx);
        return -1;
    }

    int res = EVP_DigestVerify(md_ctx, sigma, sigma_len, (unsigned char *)msg_raw, msg_len);
    EVP_MD_CTX_free(md_ctx);
    if (res == 1)
    {
        return msg_len;
    }
    else
    {
        char buffer[64];
        sprintf(buffer, "Validation ERROR - code %d\n", res);
        log_error(buffer);
        return res;
    }
}
#else
// RES:
// <0 => Invalid Signature | Error
// msg_size => Signature Correct
// else => Error
int verify_eddsa(uint8_t *msg_raw, uint8_t *msg_signed, int total_len, pki_t public_key)
{
    if (public_key.openssl_key == NULL)
    {
        log_error("Validation ERROR - key is NULL\n");
        return -1;
    }

    int sigma_len = uc2int(msg_signed);
    int msg_len = total_len - EDDSA_SIGN_HEADER_SIZE - sigma_len;
    unsigned char sigma[sigma_len];

    // (siglen (int) + msg + sigma)
    memcpy(msg_raw, msg_signed + EDDSA_SIGN_HEADER_SIZE, msg_len);
    memcpy(sigma, msg_signed + EDDSA_SIGN_HEADER_SIZE + msg_len, sigma_len);

    EVP_MD_CTX *md_ctx = EVP_MD_CTX_new();
    if (!md_ctx)
    {
        log_error("Validation ERROR - Failed to create EVP_MD_CTX_new\n");
        return -1;
    }

    if (EVP_DigestVerifyInit(md_ctx, NULL, NULL, NULL, public_key.openssl_key) != 1)
    {
        log_error("Validation ERROR - Failed to initialize EVP_PKEY_CTX\n");
        EVP_MD_CTX_free(md_ctx);
        return -1;
    }

    int res = EVP_DigestVerify(md_ctx, sigma, sigma_len, (unsigned char *)msg_raw, msg_len);
    EVP_MD_CTX_free(md_ctx);
    if (res == 1)
    {
        return msg_len;
    }
    else
    {
        char buffer[64];
        sprintf(buffer, "Validation ERROR - code %d\n", res);
        log_error(buffer);
        return res;
    }
}
#endif