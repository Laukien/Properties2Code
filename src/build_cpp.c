#include <la_boolean.h>
#include <la_message.h>
#include <la_number.h>
#include <la_string.h>
#include <la_stringbuffer.h>
#include "lib.h"
#include "build.h"

void build_run_cpp(BUILD *self) {
	assert(self != NULL);
	assert(self->name != NULL);

	unsigned int i;
	char *key;
	char *value;
	char type[15];

	/* class */
	char *class = string_toCamel(self->name);

	/* body */
	BOOL isChar;
	BOOL isInteger;
	BOOL isBoolean;
	char *alpha;
	char *upper;
	char *funct;
	size_t size = parameter_size(self->parameter);
	STRINGBUFFER *sb_public = stringbuffer_new();
	STRINGBUFFER *sb_private = stringbuffer_new();
	STRINGBUFFER *sb_init = stringbuffer_new();
	STRINGBUFFER *sb_show = stringbuffer_new();
	STRINGBUFFER *sb_load = stringbuffer_new();
	STRINGBUFFER *sb_save = stringbuffer_new();
	STRINGBUFFER *sb_edit = stringbuffer_new();
	STRINGBUFFER *sb_access = stringbuffer_new();
	for (i = 0; i < size; ++i) {
		isChar = FALSE;
		isInteger = FALSE;
		isBoolean = FALSE;

		key = parameter_getKeyByIndex(self->parameter, i);
		alpha = key2Alpha(key);
		upper = key2Upper(key);
		funct = key2Function(alpha);
		char *tmp = parameter_getValueByIndex(self->parameter, i);
		value = correctValue(tmp);
		free(tmp);


		if (number_isInteger(value)) {
			isInteger = TRUE;
			strcpy(type, "int ");
		} else if (boolean_isBoolean(value)) {
			isBoolean = TRUE;
			strcpy(type, "bool ");
		} else {
			isChar = TRUE;
			strcpy(type, "std::string ");
		}

		/* public */
        if (!self->read) {                      /* NOT read only */
			/*
			 * void self->name_setfunct(const typevalue);\n
			 */
			stringbuffer_append(sb_public, "\t\tvoid ");
			stringbuffer_append(sb_public, "set");
			stringbuffer_append(sb_public, funct);
			stringbuffer_append(sb_public, "(const ");
			stringbuffer_append(sb_public, type);
			if (isChar) {
				stringbuffer_append(sb_public, "&");
			}
			stringbuffer_append(sb_public, "value);\n");
		}

		/*
		 * typeself->name_getfunct();\n
		 */
		stringbuffer_append(sb_public, "\t\t"); 
		stringbuffer_append(sb_public, type); 
		stringbuffer_append(sb_public, "get"); 
		stringbuffer_append(sb_public, funct); 
		stringbuffer_append(sb_public, "();\n"); 
	
		/* private */
		stringbuffer_append(sb_private, "\t\t");
		stringbuffer_append(sb_private, type);
		stringbuffer_append(sb_private, alpha);
		stringbuffer_append(sb_private, ";\n");

		/* init */
		stringbuffer_append(sb_init, "\t");
		stringbuffer_append(sb_init, "this->");
		stringbuffer_append(sb_init, alpha);
		stringbuffer_append(sb_init, " = ");
		if (isChar) {
			/*
			 * this->alpha = "value";
			 */
			stringbuffer_append(sb_init, "\"");
			stringbuffer_append(sb_init, value);
			stringbuffer_append(sb_init, "\"");
		} else if (isInteger) {
			/*
			 * this->alpha = value
			 */
			stringbuffer_append(sb_init, value);
		} else if (isBoolean) {
			/*
			 * this->alpha = true|false
			 */
			BOOL tmp = boolean_toBoolean(value);
			stringbuffer_append(sb_init, tmp ? "true" : "false");
		}
		stringbuffer_append(sb_init, ";\n");

		/* show */
		stringbuffer_append(sb_show, "\n");
		stringbuffer_append(sb_show, "\t/* ");
		stringbuffer_append(sb_show, key);
		stringbuffer_append(sb_show, " */\n");
		stringbuffer_append(sb_show, "\tstd::cout << \"");
		stringbuffer_append(sb_show, key);
		stringbuffer_append(sb_show, ": \" << ");
		if (isBoolean)
			stringbuffer_append(sb_show, "la::boolean::toString(");
		stringbuffer_append(sb_show, "this->get");
		stringbuffer_append(sb_show, funct);
		stringbuffer_append(sb_show, "()");
		if (isBoolean)
			stringbuffer_append(sb_show, ")");
		stringbuffer_append(sb_show, " << std::endl;\n");

		/* load */
		stringbuffer_append(sb_load, "\n");
		stringbuffer_append(sb_load, "\t/* ");
		stringbuffer_append(sb_load, key);
		stringbuffer_append(sb_load, " */\n");

		if (isChar) {
			stringbuffer_append(sb_load, "\tthis->");
			stringbuffer_append(sb_load, alpha);
			stringbuffer_append(sb_load, " = param.get(\"");
			stringbuffer_append(sb_load, key);
			stringbuffer_append(sb_load, "\");\n");
		} else {
			stringbuffer_append(sb_load, "\ttmp = param.get(\"");
			stringbuffer_append(sb_load, key);
			stringbuffer_append(sb_load, "\");\n");
			stringbuffer_append(sb_load, "\tif (tmp.empty())\n");
			if (isInteger) {
				stringbuffer_append(sb_load, "\t\tthis->");
				stringbuffer_append(sb_load, alpha);
				stringbuffer_append(sb_load, " = -1;\n");
				stringbuffer_append(sb_load, "\telse\n");
				stringbuffer_append(sb_load, "\t\tthis->");
				stringbuffer_append(sb_load, alpha);
				stringbuffer_append(sb_load, " = la::number::toInteger(tmp);\n");
			} else if (isBoolean) {
				stringbuffer_append(sb_load, "\t\tthis->");
				stringbuffer_append(sb_load, alpha);
				stringbuffer_append(sb_load, " = false;\n");
				stringbuffer_append(sb_load, "\telse\n");
				stringbuffer_append(sb_load, "\t\tthis->");
				stringbuffer_append(sb_load, alpha);
				stringbuffer_append(sb_load, " = la::boolean::toBoolean(tmp);\n");
			}

		}

		/* save */
		if (!self->read) {
			stringbuffer_append(sb_save, "\n");
			stringbuffer_append(sb_save, "\t/* ");
			stringbuffer_append(sb_save, key);
			stringbuffer_append(sb_save, " */\n");

			stringbuffer_append(sb_save, "\tparam.add(\"");
			stringbuffer_append(sb_save, key);
			stringbuffer_append(sb_save, "\", ");
			if (isChar) {
				stringbuffer_append(sb_save, "this->");
				stringbuffer_append(sb_save, alpha);
			} else if (isInteger) {
				stringbuffer_append(sb_save, "la::number::toString(this->");
				stringbuffer_append(sb_save, alpha);
				stringbuffer_append(sb_save, ")");
			} else if (isBoolean) {
				stringbuffer_append(sb_save, "la::boolean::toString(this->");
				stringbuffer_append(sb_save, alpha);
				stringbuffer_append(sb_save, ")");
			}
			stringbuffer_append(sb_save, ");\n");
		}

		/* edit */
		if (!self->read) {
			stringbuffer_append(sb_edit, "\n");
			stringbuffer_append(sb_edit, "\t/* ");
			stringbuffer_append(sb_edit, key);
			stringbuffer_append(sb_edit, " */\n");
			stringbuffer_append(sb_edit, "\tstd::cout << \"");
			stringbuffer_append(sb_edit, key);
			stringbuffer_append(sb_edit, " [\" << ");
			if (isBoolean)
				stringbuffer_append(sb_edit, "la::boolean::toString(");
			stringbuffer_append(sb_edit, "this->");
			stringbuffer_append(sb_edit, alpha);
			if (isBoolean)
				stringbuffer_append(sb_edit, ")");
			stringbuffer_append(sb_edit, " << \"]: \" << std::flush;\n");

			stringbuffer_append(sb_edit, "\tstd::cin.clear();\n");
			stringbuffer_append(sb_edit, "\tstd::cin.getline(buffer, PARAMETER_VALUE_SIZE, '\\n');\n");
			stringbuffer_append(sb_edit, "\tif (buffer[0] != '\\0') {\n");
			stringbuffer_append(sb_edit, "\t\ttmp = la::string::trim(buffer);\n");
			if (isChar) {
				stringbuffer_append(sb_edit, "\t\tthis->");
				stringbuffer_append(sb_edit, alpha);
				stringbuffer_append(sb_edit, " = tmp;\n");
			} else if (isInteger) {
				stringbuffer_append(sb_edit, "\t\tif (tmp.empty() || !la::number::isInteger(tmp))\n");
				stringbuffer_append(sb_edit, "\t\t\tla::message::error(\"invalid value\");\n");
				stringbuffer_append(sb_edit, "\t\tthis->");
				stringbuffer_append(sb_edit, alpha);
				stringbuffer_append(sb_edit, " = la::number::toInteger(tmp);\n");
			} else if (isBoolean) {
				stringbuffer_append(sb_edit, "\t\tif (tmp.empty() || !la::boolean::isBoolean(tmp))\n");
				stringbuffer_append(sb_edit, "\t\t\tla::message::error(\"invalid value\");\n");
				stringbuffer_append(sb_edit, "\t\tthis->");
				stringbuffer_append(sb_edit, alpha);
				stringbuffer_append(sb_edit, " = la::boolean::toBoolean(tmp);\n");
			}
			stringbuffer_append(sb_edit, "\t}\n");
		}
			/* access */
		if (!self->read) {
			stringbuffer_append(sb_access, "\n");
			stringbuffer_append(sb_access, "void ");
			stringbuffer_append(sb_access, class);
			stringbuffer_append(sb_access, "::set");
			stringbuffer_append(sb_access, funct);
			stringbuffer_append(sb_access, "(const ");
			stringbuffer_append(sb_access, type);
			if (isChar)
				stringbuffer_append(sb_access, "&");
			stringbuffer_append(sb_access, "value) {\n");
			if (isChar) {
				stringbuffer_append(sb_access, "\tif (value.length() > PARAMETER_VALUE_SIZE)\n");
				stringbuffer_append(sb_access, "\t\tla::message::error(\"value to long\");\n");
				stringbuffer_append(sb_access, "\n");
			}
			stringbuffer_append(sb_access, "\t");
			stringbuffer_append(sb_access, "this->");
			stringbuffer_append(sb_access, alpha);
			stringbuffer_append(sb_access, " = value;\n");
			stringbuffer_append(sb_access, "}\n");

			stringbuffer_append(sb_access, "\n");
			stringbuffer_append(sb_access, type);
			stringbuffer_append(sb_access, class);
			stringbuffer_append(sb_access, "::get");
			stringbuffer_append(sb_access, funct);
			stringbuffer_append(sb_access, "() {\n");
			stringbuffer_append(sb_access, "\treturn this->");
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
	char *file_h = (char *) malloc(strlen(self->name) + 4 + 1);
	strcpy(file_h, self->name);
	strcat(file_h, ".hpp");
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
	fprintf(pfile_h, "#include <string>\n");
	fprintf(pfile_h, "\n");
	fprintf(pfile_h, "class %s {\n", class);
	fprintf(pfile_h, "\tprivate:\n");
	fprintf(pfile_h, "%s", stringbuffer_getTextPointer(sb_private));
	fprintf(pfile_h, "\tpublic:\n");
	fprintf(pfile_h, "\t\t%s();\n", class);
	fprintf(pfile_h, "\t\t~%s();\n", class);
	fprintf(pfile_h, "\t\tvoid init();\n");
	fprintf(pfile_h, "\t\tvoid show();\n");
	fprintf(pfile_h, "\t\tvoid load(const std::string &filename);\n");
	if (!self->read) {                          /* NOT read only */
		fprintf(pfile_h, "\t\tvoid save(const std::string &filename);\n");
		fprintf(pfile_h, "\t\tvoid open(const std::string &filename);\n");
		fprintf(pfile_h, "\t\tvoid edit();\n");
	}
	fprintf(pfile_h, "\n");
	fprintf(pfile_h, "%s", stringbuffer_getTextPointer(sb_public));
	fprintf(pfile_h, "};\n");
	fprintf(pfile_h, "\n");
	fprintf(pfile_h, "#endif\n");

	/* code */
	char *file_c = (char *) malloc(strlen(self->name) + 4 + 1);
	strcpy(file_c, self->name);
	strcat(file_c, ".cpp");
	if (self->debug)
		printf ( "CODE-FILE:\t%s\n", file_c );
	FILE *pfile_c;
	pfile_c = fopen(file_c, "w");
	if (pfile_c ==NULL)
		message_error("unable to create code");

	fprintf(pfile_c, "#include <iostream>\n");
	fprintf(pfile_c, "#include <string>\n");
	fprintf(pfile_c, "#include <la_boolean.h>\n");
	fprintf(pfile_c, "#include <la_file.h>\n");
	fprintf(pfile_c, "#include <la_message.h>\n");
	fprintf(pfile_c, "#include <la_number.h>\n");
	fprintf(pfile_c, "#include <la_parameter.h>\n");
	fprintf(pfile_c, "#include <la_string.h>\n");
	fprintf(pfile_c, "#include <la_system.h>\n");
	fprintf(pfile_c, "#include \"%s.hpp\"\n", self->name);
	fprintf(pfile_c, "\n");

	/* constructor */
	fprintf(pfile_c, "\n");
	fprintf(pfile_c, "%s::%s() {\n", class, class);
	fprintf(pfile_c, "\tthis->init();\n");
	fprintf(pfile_c, "}\n");

	/* destructor */
	fprintf(pfile_c, "\n");
	fprintf(pfile_c, "%s::~%s() {\n", class, class);
	fprintf(pfile_c, "\n");
	fprintf(pfile_c, "}\n");

	/* init */
	fprintf(pfile_c, "\n");
	fprintf(pfile_c, "void %s::init() {\n", class);
	fprintf(pfile_c, "%s", stringbuffer_getTextPointer(sb_init));
	fprintf(pfile_c, "}\n");

	/* show */
	fprintf(pfile_c, "\n");
	fprintf(pfile_c, "void %s::show() {", class);
	fprintf(pfile_c, "%s", stringbuffer_getTextPointer(sb_show));
	fprintf(pfile_c, "}\n");

	/* load */
	fprintf(pfile_c, "\n");
	fprintf(pfile_c, "void %s::load(const std::string &filename) {\n", class);
	fprintf(pfile_c, "\tif (!la::file::exists(filename))\n");
	fprintf(pfile_c, "\t\tla::message::error(\"properties-file not found\");\n");
	fprintf(pfile_c, "\n");
	fprintf(pfile_c, "\tthis->init();\n");
	fprintf(pfile_c, "\n");
	fprintf(pfile_c, "\tstd::string tmp;\n");
	fprintf(pfile_c, "\tla::parameter::Parameter param;\n");
	fprintf(pfile_c, "\tparam.loadFromFile(filename);\n");
	fprintf(pfile_c, "%s", stringbuffer_getTextPointer(sb_load));
	fprintf(pfile_c, "}\n");

	/* save */
	if (!self->read) {
		fprintf(pfile_c, "\n");
		fprintf(pfile_c, "void %s::save(const std::string &filename) {\n", class);
		fprintf(pfile_c, "\tstd::string tmp;\n");
		fprintf(pfile_c, "\tla::parameter::Parameter param;\n");
		fprintf(pfile_c, "%s", stringbuffer_getTextPointer(sb_save));
		fprintf(pfile_c, "\n");
		fprintf(pfile_c, "\tparam.saveToFile(filename);\n");
		fprintf(pfile_c, "}\n");
	}

	/* open */
	if (!self->read) {
		fprintf(pfile_c, "\n");
		fprintf(pfile_c, "void %s::open(const std::string &filename) {\n", class);
		fprintf(pfile_c, "\tif (!la::file::exists(filename))\n");
		fprintf(pfile_c, "\t\tthis->save(filename);\n");
		fprintf(pfile_c, "\n");
		fprintf(pfile_c, "\tstd::string cmd;\n");
		fprintf(pfile_c, "#ifdef SYSTEM_OS_TYPE_WINDOWS\n");
		fprintf(pfile_c, "\tcmd += \"notepad.exe\";\n");
		fprintf(pfile_c, "#else\n");
		fprintf(pfile_c, "\tif (la::file::exists(\"/usr/bin/vim\"))\n");
		fprintf(pfile_c, "\t\tcmd += \"/usr/bin/vim\";\n");
		fprintf(pfile_c, "\telse if (la::file::exists(\"/usr/bin/emacs\"))\n");
		fprintf(pfile_c, "\t\tcmd += \"/usr/bin/emacs\";\n");
		fprintf(pfile_c, "\telse if (la::file::exists(\"/usr/bin/nano\"))\n");
		fprintf(pfile_c, "\t\tcmd += \"/usr/bin/nano\";\n");
		fprintf(pfile_c, "\telse if (la::file::exists(\"/bin/vi\"))\n");
		fprintf(pfile_c, "\t\tcmd += \"/bin/vi\";\n");
		fprintf(pfile_c, "\telse\n");
		fprintf(pfile_c, "\t\tla::message::error(\"no editor found\");\n");
		fprintf(pfile_c, "#endif\n");
		fprintf(pfile_c, "\tcmd += \" \";\n");
		fprintf(pfile_c, "\tcmd += filename;\n");
		fprintf(pfile_c, "\n");
		fprintf(pfile_c, "\tsystem(cmd.c_str());\n");
		fprintf(pfile_c, "\n");
		fprintf(pfile_c, "\tthis->load(filename);\n");
		fprintf(pfile_c, "}\n");
	}

	/* edit */
	if (!self->read) {
		fprintf(pfile_c, "\n");
		fprintf(pfile_c, "void %s::edit() {\n", class);
		fprintf(pfile_c, "\tchar buffer[PARAMETER_VALUE_SIZE + 1];\n");
		fprintf(pfile_c, "\tstd::string tmp;\n");
		fprintf(pfile_c, "%s", stringbuffer_getTextPointer(sb_edit));
		fprintf(pfile_c, "}\n");
	}

	/* access */
	fprintf(pfile_c, "%s", stringbuffer_getTextPointer(sb_access));

	fclose(pfile_c);
	fclose(pfile_h);
		
	free(file_h);
	free(file_c);

	/* free */
	stringbuffer_free(sb_access);
	stringbuffer_free(sb_edit);
	stringbuffer_free(sb_save);
	stringbuffer_free(sb_load);
	stringbuffer_free(sb_show);
	stringbuffer_free(sb_private);
	stringbuffer_free(sb_public);

	free(class);

	printf ( "%s.cpp and %s.hpp has been crested successfully.\n", self->name, self->name );
}

