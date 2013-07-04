#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <la_file.h>
#include <la_message.h>
#include <la_number.h>
#include <la_parameter.h>
#include <la_string.h>
#include "type_c.h"

typedef struct {
	char key[PARAMETER_VALUE_SIZE + 1];
	int integer;
	BOOL boolean;
	char main_sub[PARAMETER_VALUE_SIZE + 1];
} test_t;

static test_t _test;


void test_show() {
	char *tmp;

	/* key */
	printf("key: %s\n", test_getKey());

	/* integer */
	printf("integer: %d\n", test_getInteger());

	/* boolean */
	tmp = boolean_toString(test_getBoolean());
	printf("boolean: %s\n", tmp);
	free(tmp);

	/* main.sub */
	printf("main.sub: %s\n", test_getMainSub());
}

void test_load(const char *filename) {
	if (!file_exists(filename))
		message_error("properties-file not found");

	memset(&_test, '\0', sizeof(test_t));

	char *tmp;
	PARAMETER *param = parameter_new();
	parameter_loadFromFile(param, filename);

	/* key */
	tmp = parameter_get(param, "key");
	if (tmp == NULL)
		_test.key[0] = '\0';
	else {
		strcpy(_test.key, tmp);
		free(tmp);
	}

	/* integer */
	tmp = parameter_get(param, "integer");
	if (tmp == NULL)
		_test.integer = -1;
	else {
		_test.integer = number_toInteger(tmp);
		free(tmp);
	}

	/* boolean */
	tmp = parameter_get(param, "boolean");
	if (tmp == NULL)
		_test.boolean = FALSE;
	else {
		_test.boolean = boolean_toBoolean(tmp);
		free(tmp);
	}

	/* main.sub */
	tmp = parameter_get(param, "main.sub");
	if (tmp == NULL)
		_test.main_sub[0] = '\0';
	else {
		strcpy(_test.main_sub, tmp);
		free(tmp);
	}

	parameter_free(param);
}

void test_save(const char *filename) {
	char *tmp;
	PARAMETER *param = parameter_new();

	/* key */
	parameter_add(param, "key", _test.key);

	/* integer */
	tmp = number_integerToString(_test.integer);
	parameter_add(param, "integer", tmp);
	free(tmp);

	/* boolean */
	tmp = boolean_toString(_test.boolean);
	parameter_add(param, "boolean", tmp);
	free(tmp);

	/* main.sub */
	parameter_add(param, "main.sub", _test.main_sub);

	parameter_saveToFile(param, filename);
	parameter_free(param);
}

void test_setKey(const char *value) {
	if (strlen(value) > PARAMETER_VALUE_SIZE)
		message_error("value to long");

	strcpy(_test.key, value);
}

char *test_getKey() {
	return _test.key;
}

void test_setInteger(const int value) {
	_test.integer = value;
}

int test_getInteger() {
	return _test.integer;
}

void test_setBoolean(const BOOL value) {
	_test.boolean = value;
}

BOOL test_getBoolean() {
	return _test.boolean;
}

void test_setMainSub(const char *value) {
	if (strlen(value) > PARAMETER_VALUE_SIZE)
		message_error("value to long");

	strcpy(_test.main_sub, value);
}

char *test_getMainSub() {
	return _test.main_sub;
}

#include <stdio.h>
int main(void) {
	test_load("test.properties");

	test_show();

	test_save("test.backup");

	return 0;
}
