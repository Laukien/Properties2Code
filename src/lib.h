#ifndef LIB_H
#define LIB_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <assert.h>

#define PRG_NAME "Properties2Code"
#define PRG_VERSION "0.4.1"
#define PRG_AUTHOR "Stephan Laukien"

typedef enum {
	format_unknown = -1,
	format_unset = 0,
	format_c = 1,
	format_cpp = 2,
	format_perl = 3
} format_t;

void showHelp();
void showVersion();
char *key2Alpha(const char *key);
char *key2Upper(const char *key);
char *key2Function(const char *key);

#endif
