#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#include "sign_scheme.h"

typedef unsigned char uchar;

static char *sign_scheme;

static const char *private_key_name;
static const char *public_key_name;

static FILE *data_log;
static char file_name[200];

static int initialized = 0;

static uchar app;
static uchar scheme_number = NO_SIGN;

static pki_t (*read_key_p)(char, const char *) = read_key_no_sign;
static int (*sign_p)(uint8_t *, uint8_t *, unsigned int, pki_t) = sign_no_sign;
static int (*verify_p)(uint8_t *, uint8_t *, int, pki_t) = verify_no_sign;
static int (*key_gen_p)(const char *, const char *) = key_gen_no_sign;

/* The CSV structure will be:

    CSV Header:
        id,app,operation,step,valid,alg,time,len

    Each line below will be a parameter of the CSV file:

    id:              Unique identification of the operation                             > int
    app:             Which app is logging                                               > 0 (GroundControl) | 1 (Autopilot)
    operation:       Which operation is being mesured                                   > 0 (sign) | 1 (verify)
    step:            When is this data from (before or after operation)                 > 0 (before) | 1 (after)
    valid:           Indicates if the validation was sucessfull                         > 0 (invalid) | 1 (valid) | 2 (Not Applicable)
                     (2 if not applicable)
    alg:             Cryptographic algorithm being used                                 > 0 (no_sign) | 1 (rsa) | 2 (ecdsa) | 3 (eddsa)
    time:            Moment in second of the operation                                  > timestemp (s)
    len:             Message length                                                    > char

    For the purpose of optimizing memory consumption during the execution of the program, some parameters will be encoded to
    a smaller size and writen all toghether in a char. Those parameters are:
        - app: 1 bit
        - operation: 1 bit
        - step: 1 bit
        - valid: 2 bits
        - alg: 3 bits

    Final CSV strutucture:
        id,encoded,time,len

    Output Examples:
        67,17,33,9
        67,49,40,9
*/

static void close_all(void)
{
    fclose(data_log);
}

static void cleanup_handler(int signum)
{
    fclose(data_log);
    _exit(1);
}

static void write_log(int id, uchar encoded, time_t time, uchar len)
{
    fprintf(data_log, "%d,%d,%ld,%d\n", id, encoded, time, len);
}

static uchar encode(uchar op, uchar step, uchar valid)
{
    /*
    app | operation | step | valid | alg
     0  |    0      |  0   |   00  | 000
    */

    // Initialize with the algorithm number since it is the less significant bits
    uchar res = scheme_number;

    res |= (app) << 7;
    res |= (op) << 6;
    res |= (step) << 5;
    res |= (valid) << 3;

    return res;
}

static void init_schemes()
{
    sign_scheme = getenv("SIGN_SCHEME");
    if (sign_scheme != NULL)
    {
        if (strcmp(sign_scheme, "RSA") == 0)
        {
            scheme_number = RSA;
            read_key_p = read_key_rsa;
            sign_p = sign_rsa;
            verify_p = verify_rsa;
            key_gen_p = key_gen_rsa;
        }
        else if (strcmp(sign_scheme, "ECDSA") == 0)
        {
            scheme_number = ECDSA;
            read_key_p = read_key_ecdsa;
            sign_p = sign_ecdsa;
            verify_p = verify_ecdsa;
            key_gen_p = key_gen_ecdsa;
        }
        else if (strcmp(sign_scheme, "EDDSA") == 0)
        {
            scheme_number = EDDSA;
            read_key_p = read_key_eddsa;
            sign_p = sign_eddsa;
            verify_p = verify_eddsa;
            key_gen_p = key_gen_eddsa;
        }
    }
}

static void init_keys()
{
    private_key_name = getenv("SECRET_KEY_PATH");
    public_key_name = getenv("PUBLIC_KEY_PATH");

    if (!private_key_name || !public_key_name)
    {
        fprintf(stderr, "Key path not defined\n");
        exit(EXIT_FAILURE);
    }
}

static void init_app()
{
    char *app_name = getenv("APP_NAME");
    if (app_name != NULL)
    {
        if (strcmp(app_name, "GroundControl") == 0)
        {
            app = GroundControl;
        }
        else if (strcmp(app_name, "Autopilot") == 0)
        {
            app = Autopilot;
        }
        else
        {
            fprintf(stderr, "Invalid app name\n");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        fprintf(stderr, "Invalid app name\n");
        exit(EXIT_FAILURE);
    }
}

static void init_logs()
{
    const char *log_path = getenv("LOG_FILE_PATH");
    if (!log_path)
    {
        printf("No path defined for log file.\n");
        exit(EXIT_FAILURE);
    }

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = cleanup_handler;

    time_t now = time(NULL);
    srand(now); // Add seed from now so it never repeats the ID between executions
    sprintf(file_name, "%s/data_log_%ld.csv", log_path, now);
    data_log = fopen(file_name, "a+");

    // For normal exits
    atexit(close_all);

    // For error exits
    sigaction(SIGSEGV, &sa, NULL);
    sigaction(SIGABRT, &sa, NULL);
    sigaction(SIGFPE, &sa, NULL);
    sigaction(SIGILL, &sa, NULL);
}

static void init_all()
{
    if (!initialized)
    {
        init_app();
        init_schemes();
        init_keys();
        init_logs();

        initialized = 1;
    }
}

pki_t read_key(char load_type)
{
    init_all();
    return read_key_p(load_type, (load_type == PRIVATE_KEY) ? private_key_name : public_key_name);
}

int sign(uint8_t *msg_signed, uint8_t *msg_raw, unsigned int msg_len, pki_t secret_key)
{
    int res;
    init_all();

    int id = rand();
    time_t before_exec = time(NULL);

    res = sign_p(msg_signed, msg_raw, msg_len, secret_key);

    time_t after_exec = time(NULL);

    write_log(id, encode(SIGN, BEFORE, NA), before_exec, msg_len);
    write_log(id, encode(SIGN, AFTER, NA), after_exec, msg_len);
    fflush(data_log);

    return res;
}

int verify(uint8_t *msg_raw, uint8_t *msg_signed, int total_len, pki_t public_key)
{
    int res;

    init_all();

    int id = rand();
    time_t before_exec = time(NULL);

    res = verify_p(msg_raw, msg_signed, total_len, public_key);

    time_t after_exec = time(NULL);

    write_log(id, encode(VERIFY, BEFORE, NA), before_exec, res);
    write_log(id, encode(VERIFY, AFTER, res > 0), after_exec, res);

    fflush(data_log);

    return res;
}

int key_gen(const char *secret_name, const char *public_name)
{
    init_schemes();
    return key_gen_p(secret_name, public_name);
}