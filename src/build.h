#ifndef BUILD_H
#define BUILD_H

#include <la_common.h>
#include <la_parameter.h>

typedef struct {
	PARAMETER *parameter;
	char *input;
	char *output;
	format_t format;
	BOOL load;
} BUILD;

BUILD *build_new();
void build_setInput(BUILD *self, const char *filename);
void build_setOutput(BUILD *self, const char *filename);
void build_setFormat(BUILD *self, const format_t format);
void build_load(BUILD *self);
void build_show(BUILD *self);
void build_action(BUILD *self);
void build_free(BUILD *self);

#endif
