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
    printf("  - eddsa\n");
}

int main(int argc, char *argv[])
{
    // Check if we have exactly one argument (program name counts as one)
    if (argc != 3)
    {
        print_usage();
        return 1;
    }

    // Define the arrya of schemes to search if valid.
    char *schemes[] = {"RSA", "EDDSA", "ECDSA"};
    int schemes_size = sizeof(schemes) / sizeof(schemes[0]);
    int valid = 0;

    // Convert input to lowercase for case-insensitive comparison
    char *input = argv[1];
    for (int i = 0; input[i]; i++)
    {
        if (input[i] >= 'a' && input[i] <= 'z')
        {
            input[i] -= 32; // Convert to lowercase
        }
    }

    char *key_name = argv[2];
    char sk_name[1000], pk_name[1000]; // secret key, public key
    sprintf(sk_name, "secret_key_%s", key_name);
    sprintf(pk_name, "public_key_%s", key_name);

    setenv("SIGN_SCHEME", input, 0);

    for (int i = 0; i < schemes_size; i++)
    {
        if (strcmp(schemes[i], input) == 0)
        {
            key_gen(sk_name, pk_name);
            valid = 1;
            break;
        }
    }

    // Process based on input
    if (!valid)
    {
        print_usage();
        return 1;
    }

    return 0;
}