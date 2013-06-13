#include "lib.h"
#include "build.h"

BUILD *build_new() {
	BUILD *self = (BUILD *)malloc(sizeof(BUILD));
	if (self == NULL)
		error("unable to get memory");

    memset(self, 0, sizeof(BUILD));             /* clean memory */

    self->input = NULL;                         /* no filename set */
    self->output = NULL;                        /* no filename set */
    self->format = format_c;                    /* C */
    self->parameter = parameter_new();          /* init parameter */
    self->load = FALSE;                         /* not loaded */

	return self;
}

void build_setInput(BUILD *self, const char *filename) {
	assert(self != NULL);
	assert(self->input == NULL);

	self->input = strdup(filename);
}

void build_setOutput(BUILD *self, const char *filename) {
	assert(self != NULL);
	assert(self->output == NULL);

	self->output = strdup(filename);
}

void build_setFormat(BUILD *self, const format_t format) {
	assert(self != NULL);

	self->format = format;
}

void build_load(BUILD *self) {
	assert(self != NULL);
	assert(self->input != NULL);
	assert(self->parameter != NULL);
	assert(self->load == FALSE);

	parameter_loadFromFile(self->parameter, self->input);

	self->load = TRUE;
}

void build_show(BUILD *self) {
	assert(self != NULL);
	assert(self->parameter != NULL);
	assert(self->input != NULL);
	assert(self->output != NULL);
	assert(self->format != format_unset);
	assert(self->load == TRUE);

	printf ( "INPUT:\t\t%s\n", self->input );
	printf ( "OUTPUT:\t\t%s\n", self->output );
	printf ( "FORMAT:\t\t");
	switch (self->format) {
		case format_unset:
			error("unset format");
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
			error("unknown format");
		default:
			error("unknown error");
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

void build_free(BUILD *self) {
	assert(self != NULL);

	if (self->input != NULL) {
		free(self->input);
		self->input = NULL;
	}
	if (self->output != NULL) {
		free(self->output);
		self->output = NULL;
	}
	if (self->parameter != NULL) {
		parameter_free(self->parameter);
		self->parameter = NULL;
	}

    memset(self, 0, sizeof(BUILD));             /* clean memory */
    free(self);                                 /* free memory */
    self = NULL;                                /* clean pointer */
}
