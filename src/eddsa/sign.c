#include "eddsa.h"

void read_sigma(uint8_t *sig, size_t sig_len) {
    printf("Signature (length: %zu bytes):\n", sig_len);
    for (size_t i = 0; i < sig_len; i++) {
        printf("%02x", sig[i]);
    }
    printf("\n");
}

// Res:
// 0 => ERROR
// else => Success (msg total len)
int sign_eddsa(uint8_t *msg_signed, uint8_t *msg_raw, unsigned int msg_len, pki_t secret_key)
{
    size_t siglen;
    uint8_t *sigma;

    // Create a context
    EVP_MD_CTX *md_ctx = EVP_MD_CTX_new();
    if (!md_ctx) {
        printf("sign error (ctx): %s\n", strerror(errno));
        return 0;
    }

    if (EVP_DigestSignInit(md_ctx, NULL, NULL, NULL, secret_key.openssl_key) != 1) {
        printf("sign error (init): %s\n", strerror(errno));
        return 0;
    }

    if (EVP_DigestSign(md_ctx, NULL, &siglen, msg_raw, msg_len) != 1) {
        printf("sign error (digest): %s\n", strerror(errno));
        return 0;
    }

    sigma = OPENSSL_malloc(siglen);
    if (!sigma) {
        printf("sign error (malloc): %s\n", strerror(errno));
        return 0;
    }

    if (EVP_DigestSign(md_ctx, sigma, &siglen, msg_raw, msg_len) != 1) {
        printf("sign error (signing): %s\n", strerror(errno));
        OPENSSL_free(sigma);
        EVP_MD_CTX_free(md_ctx);
        return 0;
    }

    // * Concatenate sign with message to send (siglen (int) + msg + sigma)
    uint8_t siglen_size[4];
    int2uc(siglen_size, (int)siglen);

    memmove(msg_signed, siglen_size, EDDSA_SIGN_HEADER_SIZE);
    memmove(msg_signed + EDDSA_SIGN_HEADER_SIZE, msg_raw, msg_len);
    memmove(msg_signed + EDDSA_SIGN_HEADER_SIZE + msg_len, (uint8_t *)sigma, siglen);

    return EDDSA_SIGN_HEADER_SIZE + msg_len + siglen;
}
