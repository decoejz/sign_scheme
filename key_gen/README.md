# Key generation

This project is exclusivelly for key generation of the desired key type.

1. Compile the code using `make` in terminal.
2. Execute the code `./main` in terminal with the desired configuration. For that run:

```bash
./main <crypto_type> <key_name>
```

The `crypto_type` can be the ones that exists in the code (rsa, eddsa, ecdsa).
The `key_name` can be anything desired by whom is executing it. The resulting key will be saved in the same directory and named `secret_key_[key_name]` for private key and `public_key_[key_name]` for public key.

For adding a new key generation algorithm just add it to the list in the begging of the code file:

```C
char *schemes[] = {"RSA", "EDDSA", "ECDSA", "NEW_SCHEME"};
```

For more information on how to create a new function, check the main [README.md](https://github.com/decoejz/sign_scheme/README.md).