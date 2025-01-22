#include <openssl/evp.h>
#include <openssl/pem.h>
#include <stdio.h>
#include <string.h>

void read_sigma(unsigned char *sig, size_t sig_len)
{
    printf("Signature (length: %zu bytes):\n", sig_len);
    for (size_t i = 0; i < sig_len; i++)
    {
        printf("%02x", sig[i]);
    }
    printf("\n");
}

int verify_message(const unsigned char *msg, size_t msg_len,
                   const unsigned char *sig, size_t sig_len)
{
    EVP_PKEY *pkey = NULL;
    EVP_MD_CTX *md_ctx = NULL;
    FILE *pubkey_file;
    int ret = -1;

    pubkey_file = fopen("public.pem", "rb");
    if (!pubkey_file)
    {
        fprintf(stderr, "Failed to open public key\n");
        return -1;
    }
    pkey = PEM_read_PUBKEY(pubkey_file, NULL, NULL, NULL);
    fclose(pubkey_file);

    if (!pkey)
    {
        fprintf(stderr, "Failed to read public key\n");
        return -1;
    }

    md_ctx = EVP_MD_CTX_new();
    if (!md_ctx)
    {
        goto cleanup;
    }

    if (EVP_DigestVerifyInit(md_ctx, NULL, NULL, NULL, pkey) != 1)
    {
        goto cleanup;
    }

    ret = EVP_DigestVerify(md_ctx, sig, sig_len, msg, msg_len);

cleanup:
    EVP_MD_CTX_free(md_ctx);
    EVP_PKEY_free(pkey);
    return ret;
}

int sign_message(const unsigned char *msg, size_t msg_len, unsigned char **sig, size_t *sig_len)
{
    EVP_PKEY *pkey = NULL;
    EVP_MD_CTX *md_ctx = NULL;
    FILE *privkey_file;
    int ret = -1;

    privkey_file = fopen("private.pem", "rb");
    if (!privkey_file)
    {
        fprintf(stderr, "Failed to open private key\n");
        return -1;
    }
    pkey = PEM_read_PrivateKey(privkey_file, NULL, NULL, NULL);
    fclose(privkey_file);

    if (!pkey)
    {
        fprintf(stderr, "Failed to read private key\n");
        return -1;
    }

    md_ctx = EVP_MD_CTX_new();
    if (!md_ctx)
    {
        goto cleanup;
    }

    if (EVP_DigestSignInit(md_ctx, NULL, NULL, NULL, pkey) != 1)
    {
        goto cleanup;
    }

    if (EVP_DigestSign(md_ctx, NULL, sig_len, msg, msg_len) != 1)
    {
        goto cleanup;
    }

    *sig = OPENSSL_malloc(*sig_len);
    if (!*sig)
    {
        goto cleanup;
    }

    if (EVP_DigestSign(md_ctx, *sig, sig_len, msg, msg_len) != 1)
    {
        OPENSSL_free(*sig);
        *sig = NULL;
        goto cleanup;
    }

    read_sigma(*sig, *sig_len);
    ret = 0;

cleanup:
    EVP_MD_CTX_free(md_ctx);
    EVP_PKEY_free(pkey);
    return ret;
}

int main()
{
    const char *message = "Hello, World!";
    unsigned char *signature = NULL;
    size_t sig_len;

    if (sign_message((unsigned char *)message, strlen(message), &signature, &sig_len) == 0)
    {
        int verify_result = verify_message((unsigned char *)message, strlen(message),
                                           signature, sig_len);
        printf("Verification result: %s\n", verify_result == 1 ? "Success" : "Failed");
        OPENSSL_free(signature);
    }

    return 0;
}