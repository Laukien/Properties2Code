#include <la_file.h>
#include <la_message.h>
#include <la_parameter.h>
#include <la_string.h>
#include <la_system.h>
#include "lib.h"
#include "build.h"

int main(int argc, char *argv[]) {

	/* parameters */
	char *input = NULL;
	char *output = NULL;
	format_t format = format_c;
	BOOL read = FALSE;
	BOOL debug = FALSE;
	int c;                                      /* switch */
	opterr = 0;                                 /* reset error */
	while ((c = getopt(argc, argv, "f:n:t:ierdhv")) != -1) {
		switch (c) {
			case 'f':                           /* input file */
				if (optarg == NULL)
					message_error("properties file not given");
				input = strdup(optarg);
				if (!file_exists(input))
					message_error ("no such input file");
				break;
			case 'n':                           /* name */
				if (optarg == NULL)
					message_error("name not given");
				output = strdup(optarg);
				break;
			case 't':                           /* type */
				if (optarg == NULL)
					message_error("type of class");
				char *tmp = string_toLower(optarg);
				if (strcmp(tmp, "c") == 0) format = format_c;
				else if (strcmp(tmp, "cpp") == 0) format = format_cpp;
				else if (strcmp(tmp, "perl") == 0) format = format_perl;
				else format = format_unknown;
				free(tmp);
				if (format == format_unknown)
					message_error("invalid format");
				break;
			case 'r':                           /* read only */
				read = TRUE;
				break;
			case 'd':                           /* debug */
				debug = TRUE;
				break;
			case 'h':                           /* help */
				showHelp();
				return (EXIT_SUCCESS);
			case 'v':                           /* version */
				showVersion();
				return (EXIT_SUCCESS);
			case '?':                           /* other */
				message_error("invalid argument");
			default:                            /* unknown */
				message_error("unknown error");
		}
	}

	/* input */
	if (input == NULL) {
		if (argc == 2)
			input = strdup(argv[1]);
		else {
			showHelp();
			message_error("no input argument");
		}
	}
	
	/* output */
	if (output == NULL) {
		output = string_toLower(input);
		char *prop = strstr(output, ".properties");
		if (prop != NULL)                       /* remove extension */
			prop[0] = '\0';

		int i;
		for (i = 0; i < strlen(output); ++i) {
			if (!isalnum(output[i]))
				output[i] = '_';
		}
	}

	/* debug */
	if (system_isDebug())
		debug = TRUE;

	/* init build-object */
	BUILD *obj = build_new();
	build_setFile(obj, input);
	build_setName(obj, output);
	build_setType(obj, format);
	build_setRead(obj, read);
	build_setDebug(obj, debug);
	build_load(obj);

	/* show parameters */
	if (debug)
		build_show(obj);

	/* run build */
	build_run(obj);

	/* free */
	build_free(obj);
	free(output);
	free(input);

	return EXIT_SUCCESS;
}
