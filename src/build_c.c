#include <la_boolean.h>
#include <la_message.h>
#include <la_number.h>
#include <la_string.h>
#include <la_stringbuffer.h>
#include "lib.h"
#include "build.h"

void build_run_c(BUILD *self) {
	assert(self != NULL);
	assert(self->name != NULL);

	unsigned int i;
	char *key;
	char *value;
	char type[8];

	/* header */
	char *file_h = (char *) malloc(strlen(self->name) + 2 + 1);
	strcpy(file_h, self->name);
	strcat(file_h, ".h");
	if (self->debug)
		printf ( "HEADER-FILE:\t%s\n", file_h );
	FILE *pfile_h;
	pfile_h = fopen(file_h, "w");
	if (pfile_h ==NULL)
		message_error("unable to create header");

	fprintf(pfile_h, "/* %s v.%s by %s */\n\n", PRG_NAME, PRG_VERSION, PRG_AUTHOR);
	char *def = string_toUpper(self->name);
	fprintf(pfile_h, "#ifndef %s_H\n", def);
	fprintf(pfile_h, "#define %s_H\n", def);
	free(def);
	fprintf(pfile_h, "\n");
	fprintf(pfile_h, "#include <la_boolean.h>\n\n");
	fprintf(pfile_h, "void %s_show();\n", self->name);
	fprintf(pfile_h, "void %s_load(const char *filename);\n", self->name);
	if (!self->read) {                          /* NOT read only */
		fprintf(pfile_h, "void %s_save(const char *filename);\n", self->name);
		fprintf(pfile_h, "//void %s_open(const char *filename);\n", self->name);
		fprintf(pfile_h, "//void %s_edit();\n", self->name);
	}
	fprintf(pfile_h, "\n");
	
	/* code */
	char *file_c = (char *) malloc(strlen(self->name) + 2 + 1);
	strcpy(file_c, self->name);
	strcat(file_c, ".c");
	if (self->debug)
		printf ( "CODE-FILE:\t%s\n", file_c );
	FILE *pfile_c;
	pfile_c = fopen(file_c, "w");
	if (pfile_c ==NULL)
		message_error("unable to create code");

	fprintf(pfile_c, "#include <stdlib.h>\n");
	fprintf(pfile_c, "#include <stdio.h>\n");
	fprintf(pfile_c, "#include <string.h>\n");
	fprintf(pfile_c, "#include <la_file.h>\n");
	fprintf(pfile_c, "#include <la_message.h>\n");
	fprintf(pfile_c, "#include <la_number.h>\n");
	fprintf(pfile_c, "#include <la_parameter.h>\n");
	fprintf(pfile_c, "#include <la_string.h>\n");
	fprintf(pfile_c, "#include \"%s.h\"\n", self->name);
	fprintf(pfile_c, "\n");
	fprintf(pfile_c, "typedef struct {\n");

	/* body */
	BOOL isChar;
	BOOL isInteger;
	BOOL isBoolean;
	char *alpha;
	char *upper;
	char *funct;
	STRINGBUFFER *sb_typedef = stringbuffer_new();
	STRINGBUFFER *sb_show = stringbuffer_new();
	STRINGBUFFER *sb_load = stringbuffer_new();
	STRINGBUFFER *sb_save = stringbuffer_new();
	STRINGBUFFER *sb_access = stringbuffer_new();
	for (i = 0; i < parameter_size(self->parameter); ++i) {
		isChar = FALSE;
		isInteger = FALSE;
		isBoolean = FALSE;

		key = parameter_getKeyByIndex(self->parameter, i);
		alpha = key2Alpha(key);
		upper = key2Upper(key);
		funct = key2Function(alpha);
		value = parameter_getValueByIndex(self->parameter, i);

		if (number_isInteger(value)) {
			isInteger = TRUE;
			strcpy(type, "int ");
		} else if (boolean_isBoolean(value)) {
			isBoolean = TRUE;
			strcpy(type, "BOOL ");
		} else {
			isChar = TRUE;
			strcpy(type, "char *");
		}

		/* header */
		if (!self->read)                        /* NOT read only */
			fprintf(pfile_h, "void %s_set%s(const %svalue);\n", self->name, funct, type);

		fprintf(pfile_h, "%s%s_get%s();\n", type, self->name, funct);

		/* typedef */
		stringbuffer_append(sb_typedef, "\t");
		if (isChar) {
			stringbuffer_append(sb_typedef, "char ");
			stringbuffer_append(sb_typedef, alpha);
			stringbuffer_append(sb_typedef, "[PARAMETER_VALUE_SIZE + 1]");
		} else {
			stringbuffer_append(sb_typedef, type);
			stringbuffer_append(sb_typedef, alpha);
		}
		stringbuffer_append(sb_typedef, ";\n");

		/* show */
		stringbuffer_append(sb_show, "\n");
		stringbuffer_append(sb_show, "\t/* ");
		stringbuffer_append(sb_show, key);
		stringbuffer_append(sb_show, " */\n");

		if (isBoolean) {
			stringbuffer_append(sb_show, "\ttmp = boolean_toString(");
			stringbuffer_append(sb_show, self->name);
			stringbuffer_append(sb_show, "_get");
			stringbuffer_append(sb_show, funct);
			stringbuffer_append(sb_show, "());\n");
		}
		stringbuffer_append(sb_show, "\tprintf(\"");
		stringbuffer_append(sb_show, key);
		if (isInteger)
			stringbuffer_append(sb_show, ": %d\\n\", ");
		else
			stringbuffer_append(sb_show, ": %s\\n\", ");
		if (isBoolean) {
			stringbuffer_append(sb_show, "tmp);\n");
			stringbuffer_append(sb_show, "\tfree(tmp);\n");
		} else {
			stringbuffer_append(sb_show, self->name);
			stringbuffer_append(sb_show, "_get");
			stringbuffer_append(sb_show, funct);
			stringbuffer_append(sb_show, "());\n");
		}

		/* load */
		stringbuffer_append(sb_load, "\n");
		stringbuffer_append(sb_load, "\t/* ");
		stringbuffer_append(sb_load, key);
		stringbuffer_append(sb_load, " */\n");

		stringbuffer_append(sb_load, "\ttmp = parameter_get(param, \"");
		stringbuffer_append(sb_load, key);
		stringbuffer_append(sb_load, "\");\n");

		stringbuffer_append(sb_load, "\tif (tmp == NULL)\n");
		stringbuffer_append(sb_load, "\t\t_");
		stringbuffer_append(sb_load, self->name);
		stringbuffer_append(sb_load, ".");
		stringbuffer_append(sb_load, alpha);
		if (isChar)
			stringbuffer_append(sb_load, "[0] = '\\0';\n");
		else if (isInteger)
			stringbuffer_append(sb_load, " = -1;\n");
		else if (isBoolean)
			stringbuffer_append(sb_load, " = FALSE;\n");

		stringbuffer_append(sb_load, "\telse {\n");
		if (isChar) {
			stringbuffer_append(sb_load, "\t\tstrcpy(_");
			stringbuffer_append(sb_load, self->name);
			stringbuffer_append(sb_load, ".");
			stringbuffer_append(sb_load, alpha);
			stringbuffer_append(sb_load, ", tmp);\n");
		} else {
			stringbuffer_append(sb_load, "\t\t_");
			stringbuffer_append(sb_load, self->name);
			stringbuffer_append(sb_load, ".");
			stringbuffer_append(sb_load, alpha);
			stringbuffer_append(sb_load, " = ");
			if (isInteger)
				stringbuffer_append(sb_load, "number_toInteger(tmp);\n");
			else if (isBoolean)
				stringbuffer_append(sb_load, "boolean_toBoolean(tmp);\n");
		}
			stringbuffer_append(sb_load, "\t\tfree(tmp);\n");
			stringbuffer_append(sb_load, "\t}\n");

		/* save */
		if (!self->read) {
			stringbuffer_append(sb_save, "\n");
			stringbuffer_append(sb_save, "\t/* ");
			stringbuffer_append(sb_save, key);
			stringbuffer_append(sb_save, " */\n");

			if (isChar) {
				stringbuffer_append(sb_save, "\tparameter_add(param, \"");
				stringbuffer_append(sb_save, key);
				stringbuffer_append(sb_save, "\", _");
				stringbuffer_append(sb_save, self->name);
				stringbuffer_append(sb_save, ".");
				stringbuffer_append(sb_save, alpha);
				stringbuffer_append(sb_save, ");\n");
			} else {
				stringbuffer_append(sb_save, "\ttmp = ");
				if (isInteger)
					stringbuffer_append(sb_save, "number_integerToString");
				else if(isBoolean)
					stringbuffer_append(sb_save, "boolean_toString");
				stringbuffer_append(sb_save, "(_");
				stringbuffer_append(sb_save, self->name);
				stringbuffer_append(sb_save, ".");
				stringbuffer_append(sb_save, alpha);
				stringbuffer_append(sb_save, ");\n");

				stringbuffer_append(sb_save, "\tparameter_add(param, \"");
				stringbuffer_append(sb_save, key);
				stringbuffer_append(sb_save, "\", tmp);\n");
				stringbuffer_append(sb_save, "\tfree(tmp);\n");
			}

			/* access */
			if (!self->read) {
				stringbuffer_append(sb_access, "\n");
				stringbuffer_append(sb_access, "void ");
				stringbuffer_append(sb_access, self->name);
				stringbuffer_append(sb_access, "_set");
				stringbuffer_append(sb_access, funct);
				stringbuffer_append(sb_access, "(const ");
				stringbuffer_append(sb_access, type);
				stringbuffer_append(sb_access, "value) {\n");
				if (isChar) {
					stringbuffer_append(sb_access, "\tif (strlen(value) > PARAMETER_VALUE_SIZE)\n");
					stringbuffer_append(sb_access, "\t\tmessage_error(\"value to long\");\n");
					stringbuffer_append(sb_access, "\n");
					stringbuffer_append(sb_access, "\tstrcpy(_");
					stringbuffer_append(sb_access, self->name);
					stringbuffer_append(sb_access, ".");
					stringbuffer_append(sb_access, alpha);
					stringbuffer_append(sb_access, ", value);\n");
				} else {
					stringbuffer_append(sb_access, "\t_");
					stringbuffer_append(sb_access, self->name);
					stringbuffer_append(sb_access, ".");
					stringbuffer_append(sb_access, alpha);
					stringbuffer_append(sb_access, " = value;\n");
				}
				stringbuffer_append(sb_access, "}\n");
			}
			stringbuffer_append(sb_access, "\n");
			stringbuffer_append(sb_access, type);
			stringbuffer_append(sb_access, self->name);
			stringbuffer_append(sb_access, "_get");
			stringbuffer_append(sb_access, funct);
			stringbuffer_append(sb_access, "() {\n");
			stringbuffer_append(sb_access, "\treturn _");
			stringbuffer_append(sb_access, self->name);
			stringbuffer_append(sb_access, ".");
			stringbuffer_append(sb_access, alpha);
			stringbuffer_append(sb_access, ";\n");
			stringbuffer_append(sb_access, "}\n");

		}

		free(value);
		free(funct);
		free(upper);
		free(alpha);
		free(key);
	}

	/* header */
	fprintf(pfile_h, "\n");
	fprintf(pfile_h, "#endif\n");

	/* typedef */
	fprintf(pfile_c, "%s", stringbuffer_getTextPointer(sb_typedef));
	fprintf(pfile_c, "} %s_t;\n", self->name);
	fprintf(pfile_c, "\n");
	fprintf(pfile_c, "static %s_t _%s;\n", self->name, self->name);
	fprintf(pfile_c, "\n");

	/* show */
	fprintf(pfile_c, "\n");
	fprintf(pfile_c, "void %s_show() {\n", self->name);
	fprintf(pfile_c, "\tchar *tmp;\n");
	fprintf(pfile_c, "%s", stringbuffer_getTextPointer(sb_show));
	fprintf(pfile_c, "}\n");

	/* load */
	fprintf(pfile_c, "\n");
	fprintf(pfile_c, "void %s_load(const char *filename) {\n", self->name);
	fprintf(pfile_c, "\tif (!file_exists(filename))\n");
	fprintf(pfile_c, "\t\tmessage_error(\"properties-file not found\");\n");
	fprintf(pfile_c, "\n");
	fprintf(pfile_c, "\tmemset(&_%s, '\\0', sizeof(%s_t));\n", self->name, self->name);
	fprintf(pfile_c, "\n");
	fprintf(pfile_c, "\tchar *tmp;\n");
	fprintf(pfile_c, "\tPARAMETER *param = parameter_new();\n");
	fprintf(pfile_c, "\tparameter_loadFromFile(param, filename);\n");
	fprintf(pfile_c, "%s", stringbuffer_getTextPointer(sb_load));
	fprintf(pfile_c, "\n");
	fprintf(pfile_c, "\tparameter_free(param);\n");
	fprintf(pfile_c, "}\n");

	/* save */
	if (!self->read) {
		fprintf(pfile_c, "\n");
		fprintf(pfile_c, "void %s_save(const char *filename) {\n", self->name);
		fprintf(pfile_c, "\tchar *tmp;\n");
		fprintf(pfile_c, "\tPARAMETER *param = parameter_new();\n");
		fprintf(pfile_c, "%s", stringbuffer_getTextPointer(sb_save));
		fprintf(pfile_c, "\n");
		fprintf(pfile_c, "\tparameter_saveToFile(param, filename);\n");
		fprintf(pfile_c, "\tparameter_free(param);\n");
		fprintf(pfile_c, "}\n");
	}

	/* access */
	fprintf(pfile_c, "%s", stringbuffer_getTextPointer(sb_access));

	stringbuffer_free(sb_access);
	stringbuffer_free(sb_save);
	stringbuffer_free(sb_load);
	stringbuffer_free(sb_show);
	stringbuffer_free(sb_typedef);
	fclose(pfile_c);
	fclose(pfile_h);
		
	free(file_h);
	free(file_c);

	printf ( "%s.c and %s.h has been crested successfully.\n", self->name, self->name );
}

