#include <assert.h>
#include "lib.h"

void showHelp() {
	printf ( "\n" );
	printf ( "Usage %s [-i PROPERTIES-FILE] [-o OUTPUT-FILE] [-f FORMAT]\n", PRG_NAME );
	printf ( "Creates source code to read the given properties file.\n" );
	printf ( "\n" );
	printf ( "Help Options:\n" );
	printf ( "\t-h\t\t\tShow help options\n" );
	printf ( "\t-v\t\t\tDisplays the current verion\n" );
	printf ( "\t-d\t\t\tshow debug information\n" );
	printf ( "\t-i <properties file>\tInput which is the source\n" );
	printf ( "\t-o <output file>\tOutput which is the destination\n" );
	printf ( "\t-f <format>\t\tFormat of output\n\t\t\t\tC, Cpp, Perl (default is C)\n" );
	printf ( "\n" );
	printf ( "Report %s bugs to software@laukien.com\n", PRG_NAME );
	printf ( "Software home page: http://software.laukien.com\n" );
	printf ( "\n" );
}

void showVersion() {
	printf ("%s v.%s (%s)\n", PRG_NAME, PRG_VERSION, __DATE__ );
	printf ("(c) 2012-2013 by %s\n", PRG_AUTHOR);
	printf ( "License BSDv2: Simplified BSD License <http://opensource.org/licenses/BSD-2-Clause>\n" );
	printf ( "This is free software: you are free to change and redistribute it.\n" );
	printf ( "There is NO WARRANTY, to the extent permitted by law.\n" );
}

void error(const char *message) {
	fprintf (stderr, "ERROR: %s\n", message);
	exit (EXIT_FAILURE);
}
