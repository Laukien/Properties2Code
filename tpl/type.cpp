#include <iostream>
#include <la_boolean.h>
#include <la_file.h>
#include <la_message.h>
#include <la_number.h>
#include <la_parameter.h>
#include <la_string.h>
#include <la_stringbuffer.h>
#include <la_system.h>
#include "type.hpp"


void test::show() {
	/* key */
	std::cout << "key: " << this->getKey() << std::endl;

	/* integer */
	std::cout << "integer: " << this->getInteger() << std::endl;

	/* boolean */
	std::cout <<  "boolean: " << la::boolean::toString(this->getBoolean()) << std::endl;

	/* main.sub */
	std::cout << "main.sub: " <<  this->getMainSub() << std::endl;
}

void test::load(const std::string &filename) {
	if (!la::file::exists(filename))
		la::message::error("properties-file not found");

	test->clean();

	char *tmp;
	PARAMETER *param = parameter_new();
	parameter_loadFromFile(param, filename);

	/* key */
	tmp = parameter_get(param, "key");
	if (tmp == NULL)
		this->key[0] = '\0';
	else {
		strcpy(this->key, tmp);
		free(tmp);
	}

	/* integer */
	tmp = parameter_get(param, "integer");
	if (tmp == NULL)
		this->integer = -1;
	else {
		this->integer = number_toInteger(tmp);
		free(tmp);
	}

	/* boolean */
	tmp = parameter_get(param, "boolean");
	if (tmp == NULL)
		this->boolean = FALSE;
	else {
		this->boolean = boolean_toBoolean(tmp);
		free(tmp);
	}

	/* main.sub */
	tmp = parameter_get(param, "main.sub");
	if (tmp == NULL)
		this->main_sub[0] = '\0';
	else {
		strcpy(this->main_sub, tmp);
		free(tmp);
	}

	parameter_free(param);
}

void test::save(const std::string &filename) {
	char *tmp;
	PARAMETER *param = parameter_new();

	/* key */
	parameter_add(param, "key", this->key);

	/* integer */
	tmp = number_integerToString(this->integer);
	parameter_add(param, "integer", tmp);
	free(tmp);

	/* boolean */
	tmp = boolean_toString(this->boolean);
	parameter_add(param, "boolean", tmp);
	free(tmp);

	/* main.sub */
	parameter_add(param, "main.sub", this->main_sub);

	parameter_saveToFile(param, filename);
	parameter_free(param);
}

void test::open(const std::string &filename) {
	if (!file_exists(filename))
		test::save(filename);

	STRINGBUFFER *cmd = stringbuffer_new();
#ifdef SYSTEM_OS_TYPE_WINDOWS
	stringbuffer_append(cmd, "notepad.exe");
#else
	if (file_exists("/usr/bin/vim"))
		stringbuffer_append(cmd, "/usr/bin/vim");
	else if (file_exists("/usr/bin/emacs"))
		stringbuffer_append(cmd, "/usr/bin/emacs");
	else if (file_exists("/usr/bin/nano"))
		stringbuffer_append(cmd, "/usr/bin/nano");
	else if (file_exists("/bin/vi"))
		stringbuffer_append(cmd, "/bin/vi");
	else
		la::message::error("no editor found");
#endif
	stringbuffer_append(cmd, " ");
	stringbuffer_append(cmd, filename);

	system(stringbuffer_getTextPointer(cmd));

	stringbuffer_free(cmd);

	test::load(filename);
}

void test::edit() {
	char buffer[PARAMETER_VALUE_SIZE + 1];
	char *tmp;

	/* key */
	printf("key [%s]: ", this->key);
	fflush(stdout);
	fgets(buffer, PARAMETER_VALUE_SIZE , stdin);
	tmp = string_trim(buffer);
	if (tmp) {
		strcpy(this->key, tmp);
	}
	free(tmp);

	/* integer */
	printf("integer [%d]: ", this->integer);
	fflush(stdout);
	fgets(buffer, PARAMETER_VALUE_SIZE , stdin);
	tmp = string_trim(buffer);
	if (tmp) {
		if (!number_isInteger(tmp))
			la::message::error("invalid value");
		this->integer = number_toInteger(tmp);
	}
	free(tmp);

	/* boolean */
	tmp = boolean_toString(this->boolean);
	printf("boolean [%s]: ", tmp);
	free(tmp);
	fflush(stdout);
	fgets(buffer, PARAMETER_VALUE_SIZE , stdin);
	tmp = string_trim(buffer);
	if (tmp) {
		if (!boolean_isBoolean(tmp))
			la::message::error("invalid value");
		this->boolean = boolean_toBoolean(tmp);
	}
	free(tmp);

	/* main.sub */
	printf("main.sub [%s]: ", this->main_sub);
	fflush(stdout);
	fgets(buffer, PARAMETER_VALUE_SIZE , stdin);
	tmp = string_trim(buffer);
	if (tmp) {
		strcpy(this->main_sub, tmp);
	}
	free(tmp);
}

void test::setKey(const std::string &value) {
	if (strlen(value) > PARAMETER_VALUE_SIZE)
		la::message::error("value to long");

	strcpy(this->key, value);
}

char *test::getKey() {
	return this->key;
}

void test::setInteger(const int value) {
	this->integer = value;
}

int test::getInteger() {
	return this->integer;
}

void test::setBoolean(const bool value) {
	this->boolean = value;
}

bool test::getBoolean() {
	return this->boolean;
}

void test::setMainSub(const std::string &value) {
	if (strlen(value) > PARAMETER_VALUE_SIZE)
		la::message::error("value to long");

	strcpy(this->main_sub, value);
}

char *test::getMainSub() {
	return this->main_sub;
}

#include <stdio.h>
int main(void) {
	test test;
	test.load("test.properties");

	test.edit();
	test.show();
//	test.open("test.properties");

	test.save("test.backup");

	return 0;
}
