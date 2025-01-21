#include "no_sign.h"

// load_type:
//   0 => secret key
//   1 => public key
pki_t read_key_no_sign(char _load_type, const char *_file_name)
{
    static char value = '1';
    pki_t key_res = {.char_key= &value};

    return key_res;
}
