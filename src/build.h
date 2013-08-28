#ifndef BUILD_H
#define BUILD_H

#include <la_common.h>
#include <la_parameter.h>

typedef struct {
	PARAMETER *parameter;
	char *file;
	char *name;
	format_t format;
	BOOL read;
	BOOL debug;
	BOOL load;
} BUILD;

BUILD *build_new();
void build_setFile(BUILD *self, const char *filename);
void build_setName(BUILD *self, const char *name);
void build_setType(BUILD *self, const format_t format);
void build_setRead(BUILD *self, const BOOL read);
void build_setDebug(BUILD *self, const BOOL debug);
void build_load(BUILD *self);
void build_show(BUILD *self);
void build_run(BUILD *self);
void build_run_c(BUILD *self);
void build_run_cpp(BUILD *self);
void build_free(BUILD *self);

#endif
