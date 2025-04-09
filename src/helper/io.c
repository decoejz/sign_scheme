#include "io.h"

#ifdef CONFIG_EMBEDDED
static FATFS fs;
static const char *cfg_path_file = "sign_scheme.cfg"; 
static bool fs_mounted = false;

void log_error(const char *msg)
{
    if (!sdcard_mount()) {
        return; // não conseguiu montar o SD, não há o que fazer
    }

    FIL file;
    if (f_open(&file, "error.log", FA_WRITE | FA_OPEN_APPEND) == FR_OK) {
        UINT written;
        f_write(&file, msg, strlen(msg), &written);
        f_write(&file, "\n", 1, &written); // adiciona nova linha
        f_close(&file);
    }
}

char *getcfg (const char *attr)
{
    if (!cfg_path_file || strstr(cfg_path_file, ".cfg") == NULL) return NULL;
    if (!sdcard_mount()) return NULL;

    FIL file;
    FRESULT res = f_open(&file, cfg_path_file, FA_READ);
    if (res != FR_OK) {
        log_error("sign_scheme.cfg not found");
        return NULL;
    } else {        
        log_error("sign_scheme.cfg found");
    }

    UINT bytes_read;
    static char buffer[1024];  // or malloc + free later
    res = f_read(&file, buffer, sizeof(buffer) - 1, &bytes_read);
    if (res != FR_OK) {
        f_close(&file);
        return NULL;
    }

    buffer[bytes_read] = '\0';
    f_close(&file);

    // Parse for attr=...
    char *line = strtok(buffer, BREAK_LINE);
    while (line != NULL) {
        if (line[0] == '#') {
            line = strtok(NULL, BREAK_LINE);
            continue;
        }

        char *eq = strchr(line, '=');
        if (!eq) {
            line = strtok(NULL, BREAK_LINE);
            continue;
        }

        *eq = '\0';
        char *key = line;
        char *value = eq + 1;

        if (strcmp(attr, key) == 0) {
            return value;
        }

        line = strtok(NULL, BREAK_LINE);
    }

    char msg[128];
    snprintf(msg, sizeof(msg), "%s not found", attr);
    log_error(msg);

    return NULL; // not found
}
FIL *open_file(const char *file_name) 
{
    static FIL file;
    FRESULT fr;
    fr = f_open(&file, file_name, FA_READ);
    if (fr != FR_OK) {
        // Handle error: file not found or inaccessible
        char msg[128];
        snprintf(msg, sizeof(msg), "open_file - file %s not found", file_name);
        log_error(msg);
        return NULL;
    }
    return &file;
}

void read_file(FIL *file, char *buffer)
{
    UINT br;
    FRESULT fr;
    fr = f_read(&file, buffer, sizeof(buffer) - 1, &br);
    if (fr != FR_OK || br == 0) {
        f_close(&file);
        buffer[0] = END_BUFFER;
        return;
    } else {
        log_error("error to readd file",);
    }

    buffer[br] = END_BUFFER;  // Null-terminate for PEM format
    f_close(&file);
}
bool register_info(FIL *file, const char *fmt, ...) {
    if (!file) return false;

    char buffer[256];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    UINT written;
    FRESULT res = f_write(file, buffer, strlen(buffer), &written);
    return (res == FR_OK && written == strlen(buffer));
}
void log_error(const char *error) {
    char buffer[256];
    va_list args;
    //va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    DEBUG_PRINT("%s", buffer);  // ou printf() se preferir
}
bool sdcard_mount(void) 
{
    if (!fs_mounted) {
        if (f_mount(&fs, "", 1) == FR_OK) {
            fs_mounted = true;
        }
    }
    return fs_mounted;
}

void flush_file(FIL *file)
{
    f_sync(file);
}

void close_file(FIL *file)
{
    f_close(file);
}

#else
FILE *open_file(const char *file_name)
{
    return fopen(file_name, "r");
}

void read_file(FILE *file)
{

}

void log_error(const char *error) 
{
    log_error(error);
}

void flush_file(FILE *file)
{
    fflush(file);
}

void close_file(FILE *file)
{
    fclose(file);
}
#endif