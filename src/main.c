#include <la_file.h>
#include <la_parameter.h>
#include <la_string.h>
#include "lib.h"
#include "build.h"

int main(int argc, char *argv[]) {

	/* parameters */
	char *input = NULL;
	char *output = NULL;
	format_t format = format_c;
	BOOL debug = FALSE;
	int c;                                      /* switch */
	opterr = 0;                                 /* reset error */
	while ((c = getopt(argc, argv, "i:o:f:dhv")) != -1) {
		switch (c) {
			case 'i':                           /* input */
				if (optarg == NULL)
					error("properties file not given");
				input = strdup(optarg);
				if (!file_exists(input))
					error ("no such input file");
				break;
			case 'o':                           /* output */
				if (optarg == NULL)
					error("file not given");
				output = strdup(optarg);
				break;
			case 'f':                           /* output */
				if (optarg == NULL)
					error("format not given");
				char *tmp = string_toLower(optarg);
				if (strcmp(tmp, "c") == 0) format = format_c;
				else if (strcmp(tmp, "cpp") == 0) format = format_cpp;
				else if (strcmp(tmp, "perl") == 0) format = format_perl;
				else format = format_unknown;
				free(tmp);
				if (format == format_unknown)
					error("invalid format");
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
				error("invalid argument");
			default:                            /* unknown */
				error("unknown error");
		}
	}

	/* input */
	if (input == NULL) {
		if (argc == 2)
			input = strdup(argv[1]);
		else {
			showHelp();
			error("no input argument");
		}
	}
	
	/* output */
	if (output == NULL) {
		output = string_toLower(input);
		int i;
		for (i = 0; i < strlen(output); ++i) {
			if (!isalnum(output[i]))
				output[i] = '_';
		}
	}

	/* init build-object */
	BUILD *obj = build_new();
	build_setInput(obj, input);
	build_setOutput(obj, output);
	build_setFormat(obj, format);
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
