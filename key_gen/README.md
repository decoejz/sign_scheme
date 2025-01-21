# Key generation

This project is exclusivelly for key generation of the desired key type.

1. Compile the code using `make` in terminal.
2. Execute the code `./main` in terminal with the desired configuration. For that run:

```bash
./main <crypto_type> <key_name>
```

The `crypto_type` can be the ones that exists in the code (rsa, ecdsa).
The `key_name` can be anything desired by whom is executing it. The resulting key will be saved in the same directory and named `secret_key_[key_name]` for private key and `public_key_[key_name]` for public key.

For adding a new key generation algorithm just add these algorithm in the condition that already exist:

```C
if (...) { ... }
else if (strcmp(input, "new_scheme") == 0)
{
    key_gen(NEW_SCHEME, sk_name, pk_name); // The same defined in include/sign_scheme_definitions.h
}
```

For more information on how to create a new function, check the main [README.md](https://github.com/decoejz/sign_scheme/README.md).