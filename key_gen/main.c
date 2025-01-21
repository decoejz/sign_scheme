#include <stdio.h>
#include <string.h>

#include "sign_scheme.h"

void print_usage()
{
    printf("Error: Missing or invalid argument\n");
    printf("Usage: ./main <crypto_type> <key_name>\n");
    printf("Available crypto types:\n");
    printf("  - rsa\n");
    printf("  - ecdsa\n");
}

int main(int argc, char *argv[])
{
    // Check if we have exactly one argument (program name counts as one)
    if (argc != 3)
    {
        print_usage();
        return 1;
    }

    // Convert input to lowercase for case-insensitive comparison
    char *input = argv[1];
    for (int i = 0; input[i]; i++)
    {
        if (input[i] >= 'A' && input[i] <= 'Z')
        {
            input[i] += 32; // Convert to lowercase
        }
    }

    char *key_name = argv[2];
    char sk_name[1000], pk_name[1000]; // secret key, public key
    sprintf(sk_name, "secret_key_%s", key_name);
    sprintf(pk_name, "public_key_%s", key_name);

    // Process based on input
    if (strcmp(input, "rsa") == 0)
    {
        key_gen(RSA, sk_name, pk_name);
    }
    else if (strcmp(input, "ecdsa") == 0)
    {
        key_gen(ECDSA, sk_name, pk_name);
    }
    else
    {
        print_usage();
        return 1;
    }

    return 0;
}