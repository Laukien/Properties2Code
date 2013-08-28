#include <la_message.h>
#include "lib.h"
#include "build.h"

BUILD *build_new() {
	BUILD *self = (BUILD *)malloc(sizeof(BUILD));
	if (self == NULL)
		message_error("unable to get memory");

    memset(self, 0, sizeof(BUILD));             /* clean memory */

    self->file = NULL;                          /* no file set */
    self->name = NULL;                          /* no name set */
    self->format = format_unset;                /* no format */
    self->read = FALSE;                         /* read only */
    self->debug = FALSE;                        /* no debugging */
    self->parameter = parameter_new();          /* init parameter */
    self->load = FALSE;                         /* not loaded */

	return self;
}

void build_setFile(BUILD *self, const char *filename) {
	assert(self != NULL);
	assert(self->file == NULL);

	self->file = strdup(filename);
}

void build_setName(BUILD *self, const char *name) {
	assert(self != NULL);
	assert(self->name == NULL);

	self->name = strdup(name);
}

void build_setType(BUILD *self, const format_t format) {
	assert(self != NULL);

	self->format = format;
}

void build_setRead(BUILD *self, const BOOL read) {
	assert(self != NULL);

	self->read = read;
}

void build_setDebug(BUILD *self, const BOOL debug) {
	assert(self != NULL);

	self->debug = debug;
}

void build_load(BUILD *self) {
	assert(self != NULL);
	assert(self->file != NULL);
	assert(self->parameter != NULL);
	assert(self->load == FALSE);

	parameter_loadFromFile(self->parameter, self->file);

	self->load = TRUE;
}

void build_show(BUILD *self) {
	assert(self != NULL);
	assert(self->parameter != NULL);
	assert(self->file != NULL);
	assert(self->name != NULL);
	assert(self->format != format_unset);
	assert(self->load == TRUE);

	printf ( "FILE:\t\t%s\n", self->file );
	printf ( "NAME:\t\t%s\n", self->name );
	printf ( "TYPE:\t\t");
	switch (self->format) {
		case format_unset:
			message_error("unset type");
		case format_c:
			printf ( "C\n" );
			break;
		case format_cpp:
			printf ( "Cpp\n" );
			break;
		case format_perl:
			printf ( "Perl\n" );
			break;
		case format_unknown:
			message_error("unknown type");
		default:
			message_error("unknown error");
	}

	unsigned int i;
	char *key;
	char *value;
	for (i = 0; i < parameter_size(self->parameter); ++i) {
		key = parameter_getKeyByIndex(self->parameter, i);
		value = parameter_getValueByIndex(self->parameter, i);

		printf ( "PARMETER %d:\t%s = %s\n", i + 1, key, value );

		free(value);
		free(key);
	}
}

void build_run(BUILD *self) {
	assert(self != NULL);
	assert(self->name != NULL);
	assert(self->format != format_unset);
	assert(self->load == TRUE);

	switch (self->format) {
		case format_c:
			build_run_c(self);
			break;
		case format_cpp:
			build_run_cpp(self);
			break;
		default:
			message_error("invalid type");
	}
}

void build_free(BUILD *self) {
	assert(self != NULL);

	if (self->file != NULL) {
		free(self->file);
		self->file = NULL;
	}
	if (self->name != NULL) {
		free(self->name);
		self->name = NULL;
	}
	if (self->parameter != NULL) {
		parameter_free(self->parameter);
		self->parameter = NULL;
	}

    memset(self, 0, sizeof(BUILD));             /* clean memory */
    free(self);                                 /* free memory */
    self = NULL;                                /* clean pointer */
}
