#include <stdbool.h>
#include <stdint.h>

#ifdef CONFIG_EMBEDDED
#include "ff.h" 
#else
#include <stdio.h>
#endif

#define END_BUFFER '\0'
#define BREAK_LINE "\n"

#ifdef CONFIG_EMBEDDED

void read_file(FIL *file, char *buffer);
FIL *open_file(const char *file_name);
void flush_file(FIL *file);
void close_file(FIL *file);

char *getcfg (const char *attr);
bool register_info(FIL *file, const char *fmt, ...);
bool sdcard_mount(void);
#else
void read_file(FILE *file);
FILE *open_file(const char *file_name);
void flush_file(FILE *file);
void close_file(FILE *file);
#endif

void log_error(const char *error);
