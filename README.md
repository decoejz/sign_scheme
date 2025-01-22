# Sign Scheme

## Table of Contents

- [Sign Scheme](#sign-scheme)
  - [Table of Contents](#table-of-contents)
  - [Overview](#overview)
  - [Requirements](#requirements)
  - [Lib Architecture](#lib-architecture)
  - [Getting Started](#getting-started)
    - [Protocol Structure](#protocol-structure)
    - [Configuring the Lib](#configuring-the-lib)
    - [Code Definitions](#code-definitions)
    - [Adding New Sign Schemes](#adding-new-sign-schemes)
  - [Important Information](#important-information)
  - [Testing Logs](#testing-logs)
  - [Author](#author)
  - [License](#license)


## Overview

This lib was build to validate and test a series of cryptographic sign schemes for message transmission between a ground station and a drone. For this we used the [PX4 Autopilot](https://px4.io/) and the [QGroundControl (QGC)](https://qgroundcontrol.com/) for testing the communication.

These two applications communicate through [MAVlink Protocol](https://mavlink.io/) which was the implementation focus.

As the project involves changing the communication we also had to modify those projects. The modified code (forked version) can be found in:
- [Forked PX4](https://github.com/decoejz/PX4-Autopilot)
- [Forked QGC](https://github.com/decoejz/qgroundcontrol)

This project is build as a scientific study of a Master Degree in Aeronautical and Computer Engineer from [ITA - Instituto Tecnológico de Aeronáutica](http://www.ita.br/).

## Requirements

Programming Language: C

Libs:
- OpenSSL 3.4.0

## Lib Architecture

This project is organized as follow:

```BASH
Root
├── CMakeLists.txt
├── README.md
├── build
├── cmake
│   ├── SignSchemeConfig.cmake
│   ├── SignSchemeConfig.cmake.in
│   └── SignSchemeConfigVersion.cmake
├── compile.sh
├── include
│   ├── sign_scheme.h
│   └── sign_scheme_definitions.h
├── install
├── key_gen
│   ├── Makefile
│   ├── README.md
│   └── main.c
├── sign_schemeConfig.cmake.in
└── src
    ├── helper
    │   ├── helper.h
    │   ├── int2uc.c
    │   └── uc2int.c
    ├── rsa
    │   ├── key_gen.c
    │   ├── read_key.c
    │   ├── rsa.h
    │   ├── sign.c
    │   └── verify.c
    └── sign_scheme.c
```

The compilation files are: `CMakeLists.txt`, `cmake` directory and `sign_schemeConfig.cmake.in`. The `compilation.sh` is a simple bash script that builds the project easly, creating all necessary folder and files for the compilation process. As a result of the compilation process, the directories `build` and `install` and all related files will be created automatically.

The `include` directory contains:
- sign_scheme.h: Contains all headers that are important for everyone when the lib is being used.
- sign_scheme_definitions.h: Contains some importante definitions off the lib in general.

The `src` directory contains:
- sign_scheme.c: The implementation of the lib itself. It has all the functions that will be externalized for everyone to use it.
- helper directory: some functions that are being used inside the lib.
- rsa directory: This is just as an example for every cryptographic algorithm that need to be implemented. Inside it has one main file that contains all the headers (`rsa.h`), and the other files are each the implementation of an important function that will be used for the cryptographic process. They are sign, verify, read key and generate key.

The `key_gen` directory is just a simple project to generate the required key for an algorithm. More information on how to use it can be found in the [README.md](https://github.com/decoejz/sign_scheme/key_gen/README.md) inside the directory.

## Getting Started

After installing all the [requirement packages](#requirements) you can start building the project. For that you can simple run the `compile.sh` shell script which will create the folder `build` and run the `cmake` command inside it. It will also create the `install` folder where the archive file (.a) can be found in the `install/lib`. This folder will also have a `include` folder with all headers inside it.

After compiling the lib it will be ready to be used and can normally be imported in other projects.

One simple example using cmake:

```CMakeLists.txt
set(LIB_DIR "path/to/dir/sign_scheme")

list(APPEND CMAKE_PREFIX_PATH ${LIB_DIR})
find_package(SignScheme REQUIRED)

include_directories(${LIB_DIR})
link_directories(${LIB_DIR})

target_link_libraries(test_bin PRIVATE SignScheme::SignScheme)
```

### Protocol Structure

As this lib focus on testing some cryptographic sign schemes and each of them can have different sign sizes, and the MAVlink message also have different sizes. Tt is important for the lib to know were the message ends and the sign begins. For this to be easly of use on both sides of the communication, a header is added to the begging of the message transmited.

In this way, the message transmited sctructure is:

`HEADER + MESSAGE RAW + SIGNATURE`

- HEADER: Will be fixed for each sign scheme (it does not need to be the same between different schemes). In all schemes implement so far they are an int size, which means 4 bytes of information. It will always tells the size of the signature.
- MESSAGE RAW: The [MAVlink message](https://mavlink.io/en/messages/common.html) that is being signed.
- SIGNATURE: The signature itself.

### Configuring the Lib

To configure the project environmental variables is used. You need to set specific variables that the library will read during initialization. These variables can be set in your shell profile or directly in the terminal before running your application. The main environmental variables used are:

- `SIGN_SCHEME`: Defines the cryptographic algorithm to be used (e.g., `RSA`, `ECDSA`).
- `SECRET_KEY_PATH`: Specifies the path to the file where the private key is stored.
- `PUBLIC_KEY_PATH`: Specifies the path to the file where the public key is stored.
- `LOG_FILE_PATH`: Specified the path where the log file will be writen.
- `APP_NAME`: Which app is the lib using. It is automatically configured inside the [PX4](https://github.com/decoejz/PX4-Autopilot) and [QGC](https://github.com/decoejz/qgroundcontrol) project but if desired can be forced in the terminal. It can only be one of two values (`PX4` or `QGC`).

Example of setting these variables in a Unix-based system:

```bash
export SIGN_SCHEME=RSA
export SECRET_KEY_PATH=/path/to/private_key.pem
export PUBLIC_KEY_PATH=/path/to/public_key.pem
export LOG_FILE_PATH=/path/to/log/dir
export APP_NAME=PX4
```

Make sure to set these variables according to your environment and the cryptographic algorithm you intend to use. The library will automatically read these variables and configure itself accordingly.

### Code Definitions

There are some important definitions that are being used in this project. They can be found in the `include/sign_scheme_definitions.h`. The most important ones will be described here:

- PUBLIC_KEY: Used in the read key function;
- PRIVATE_KEY: Used in the read key function;
- pki_t: A *union* that contains all possible key types.

### Adding New Sign Schemes

The process of adding a new sign scheme is pretty easy. As it is described in [Lib Architecture](#lib-architecture) you first need to create a new folder inside the `src` directory. It can be called anything you want but it is recommended to call it the name of the cryptographic algorithm (rsa for example). Then some files has to be created:

1. [name].h: Will contain all headers from this folder (sign_[]; verify_[name]; key_gen_[name] and read_key_[name]). As this project can be imported in c++ projects, it is important to `extern "C"`.

2. sign.c: This file will implement the signing method of that algorithm. 

Function header:
```C
int sign_[name](uint8_t *msg_signed, uint8_t *msg_raw, unsigned int msg_len, pki_t secret_key);
```

Parameters:
- msg_signed: The complete message (header + message + sign)
- msg_raw: The message to be signed
- msg_len: The size of message to be signed
- secret_key: The key to sign the message. As it is a [pki_t](#code-definitions), inside this function it is need to call the correct type.

Return:
- return = 0: As an error
- return > 0: Complete message size (with header and signature)

3. verify.c: Verify the message

Function header:
```C
int verify_[name](uint8_t *msg_raw, uint8_t *msg_signed, int total_len, pki_t public_key);
``` 

Parametes:
- msg_raw: The raw message without the signature and header
- msg_signed: The complete signed message (header + message + signature)
- total_len: The complete message size
- public_key: The public key to validate the sign. As it is a [pki_t](#code-definitions), inside this function it is need to call the correct type.

Return:
- return < 0: Error on validation
- return = 0: Invalid signature
- return > 0: Size of the raw message

4. key_gen.c: Generates a new key

Function header:
```C
int key_gen_[name](const char *secret_name, const char *public_name)
``` 

Parametes:
- secret_name: A string with the secret key name to be created
- public_name: A string with the public key name to be created

Return:
- return = 0: Error
- return = 1: Success

5. read_key.c: Read the key file so it can be used

Function header:
```C
pki_t read_key_rsa(char load_type, const char *file_name)
```

Parametes:
- load_type: The type of the key (public or private key) that is going to be read. It can be `PUBLIC_KEY` | `PRIVATE_KEY` (see [Code Definitions](#code-definitions))
- file_name: A string with the public/private key

Return:
- return [pki_t](#code-definitions). As it is a [pki_t](#code-definitions), inside this function it is need to save in the correct type.

---
In the `include/sign_scheme_definitions.h` it is needed to:

- Add the key type of the new algorithm if it is not already on the *union*.
- Add a *define* for the new algorithm for use in the log. It should be a 3 bits number (see [Important Information](#important-information) for more details), and cannot be used yet. For example, if there are already three (NO_SIGN=0; RSA=1; ECDSA=2) then the next need to be NEW_NAME=4.

---
In the `include/sign_scheme.h` just need to *include* the new algorithm header.

---
In the `src/sign_scheme` there are several functions, but when adding a new algorithm only the `static void init_schemes()` has to be changed. Inside it there is a condition that configure it scheme. The code should be as follow:

```C
if (...) { ... }
else if (strcmp(sign_scheme, "NEW_SCHEME") == 0)
{
    scheme_number = NEW_SCHEME; // The same defined in the sign_scheme_defintions.h
    read_key_p = read_key_[name]; // read key created function
    sign_p = sign_[name]; // sign message created function
    verify_p = verify_[name]; // verify message created function
    key_gen_p = key_gen_[name]; // generate key created function
}
```

## Important Information

The sign_scheme.c has some parte of the code that is related to logging information for the data analysis after the tests are done. It will be explained latter in [Testing Logs](#testing-logs) but for now it is important to notice that in the way the implementation was made, it is possible of testing only 8 sign scheme algorithms at a time. It does not mean more of them cannot be implemented, but changes in the logging part would need to be changed. This happened because for better use of memory and optimizing time consumption, we put some information together so it will all use a singe byte of information.

## Testing Logs

Every time this lib is used a new log file will be created as a csv file. These file is organized as described below:

    CSV Header:
        id,app,operation,step,valid,alg,time,mavlink_len,mavlink_seq,mavlink_sysid,mavlink_compid,mavlink_msgid

    Each line below will be a parameter of the CSV file:

    id:              Unique identification of the operation                             > int
    app:             Which app is logging                                               > 0 (qgc) | 1 (px4)
    operation:       Which operation is being mesured                                   > 0 (sign) | 1 (verify)
    step:            When is this data from (before or after operation)                 > 0 (before) | 1 (after)
    valid:           Indicates if the validation was sucessfull                         > 0 (invalid) | 1 (valid) | 2 (Not Applicable)
                     (2 if not applicable)
    alg:             Cryptographic algorithm being used                                 > 0 (no_sign) | 1 (rsa) | 2 (ecdsa)
    time:            Moment in second of the operation                                  > timestemp (s)
    mavlink_len:     Mavlink payload length                                             > char
    mavlink_seq:     Mavlink message sequence                                           > char
    mavlink_sysid:   Mavlink system id                                                  > char
    mavlink_compid:  Mavlink component id                                               > char
    mavlink_msgid:   Mavling message ID                                                 > int

    For the purpose of optimizing memory consumption during the execution of the program, some parameters will be encoded to
    a smaller size and writen all toghether in a char. Those parameters are:
        - app: 1 bit
        - operation: 1 bit
        - step: 1 bit
        - valid: 2 bits
        - alg: 3 bits

    Final CSV strutucture:
        id,encoded,time,mavlink_len,mavlink_seq,mavlink_sysid,mavlink_compid,mavlink_msgid

    Output Examples:
        67,17,33,9,4,42,158,0
        67,49,40,9,4,42,158,0

## Author

- André Ejzenmesser: [Github](https://github.com/decoejz) | [Email](decoejz@gmail.com)
  - Advisor: Lourenço Alves Pereira Junior: [Github](https://github.com/ljr-ita) | [Email](ljr@ita.br)

## License

[MIT](https://choosealicense.com/licenses/mit/) Permission is hereby granted, free of charge, to any person obtaining a copy of this solution to deal with the publication, use or customization of the Software without restriction to whom it is provided, subject to the following conditions:

The notice of Reaction Team and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION WARRANTIES OF MERCHANTABILITY FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION