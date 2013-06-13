#include "lib.h"
#include "build.h"

void build_run_c(BUILD *self) {
	assert(self != NULL);
	assert(self->output != NULL);

	char *file_c = (char *) malloc(strlen(self->output) + 2 + 1);
	strcpy(file_c, self->output);
	strcat(file_c, ".c");
	if (self->debug)
		printf ( "CODE-FILE:\t%s\n", file_c );

	char *file_h = (char *) malloc(strlen(self->output) + 2 + 1);
	strcpy(file_h, self->output);
	strcat(file_h, ".h");
	if (self->debug)
		printf ( "HEADER-FILE:\t%s\n", file_h );

	printf ( "C!!!\n" );

	free(file_h);
	free(file_c);
}

