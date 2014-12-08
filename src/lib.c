#include <assert.h>
#include <la_boolean.h>
#include <la_character.h>
#include "lib.h"

void showHelp() {
	printf ( "\n" );
	printf ( "Usage %s [-f FILE] [-n NAME] [-t TYPE]\n", PRG_NAME );
	printf ( "Creates source code to read the given properties file.\n" );
	printf ( "\n" );
	printf ( "Help Options:\n" );
	printf ( "\t-h\t\t\tShow help options\n" );
	printf ( "\t-v\t\t\tDisplays the current verion\n" );
	printf ( "\t-r\t\t\tread only\n" );
	printf ( "\t-d\t\t\tshow debug information\n" );
	printf ( "\t-f <file>\t\tProperties file which is the source\n" );
	printf ( "\t-n <name>\t\tName of the class and its files\n" );
	printf ( "\t-t <type>\t\tType of output\n\t\t\t\tC, Cpp, Perl (default is C)\n" );
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

char *key2Alpha(const char *key) {
	char *res = strdup(key);
    int i;
	for (i = 0; i < strlen(res); ++i) {
		if (!i && isdigit(res[i])) {
			res[i]='_';
			continue;
		}
		if (!isalnum(res[i]))
			res[i] = '_';
	}

	return res;
}

char *key2Upper(const char *key) {
	char *res = strdup(key);
	res[0] = toupper(res[0]);

	return res;
}

char *key2Function(const char *key) {
	unsigned int len = strlen(key);
	char *res = (char *)malloc(len - character_count(key, '_') + 1);
	unsigned int i;
	unsigned int j;
	BOOL isUpper = TRUE;

	j = 0;
	for (i = 0; i < len; ++i) {
		if (key[i] == '_') {
			isUpper = TRUE;
			continue;
		}

		if (isUpper) {
			isUpper = FALSE;
			res[j] = toupper(key[i]);
		} else
			res[j] = key[i];
		++j;
	}
	res[j] = '\0';

	return res;
}

char *correctValue(const char *value) {
	size_t len = strlen(value);
	int count = 0;
	count += character_count(value, '\\');
	count += character_count(value, '"');

	char *res = (char *)malloc(len + count + 1);

	int i;
	int j = 0;
	for (i = 0; i < len; ++i) {
		if (value[i] == '\\') res[j++] = '\\';
		else if (value[i] == '"') res[j++] = '\\';
		res[j++] = value[i];
	}
	res[j] = '\0';

	return res;
}
