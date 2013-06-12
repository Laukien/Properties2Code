#ifndef LIB_H
#define LIB_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#define PRG_NAME "Properties2Code"
#define PRG_VERSION "0.1.3"
#define PRG_AUTHOR "Stephan Laukien"

typedef enum {
	format_unknown = -1,
	format_c = 1,
	format_cpp = 2,
	format_perl = 3
} format_t;

void showHelp();
void showVersion();
void error(const char *message);

#endif
