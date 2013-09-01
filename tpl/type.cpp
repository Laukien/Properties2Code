#include <iostream>
#include <string>
#include <la_boolean.h>
#include <la_file.h>
#include <la_message.h>
#include <la_number.h>
#include <la_parameter.h>
#include <la_string.h>
#include <la_stringbuffer.h>
#include <la_system.h>
#include "type.hpp"

void test::test() {
	this->init();
}

void test::init() {
	this->key = "value";
	this->integer = 123;
	this->boolean = true;
	this->main_sub = "example";
}

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
	if (!la::file::exists(filename)) {
		la::message::error("properties-file not found");
	}

	this->clean();

	std::string tmp;
	la::parameter param;
	param.loadFromFile(filename);

	/* key */
	this->key = param.get("key");

	/* integer */
	tmp = param.get("integer");
	if (tmp.empty()) {
		this->integer = -1;
	} else {
		this->integer = la::number::toInteger(tmp);
	}

	/* boolean */
	tmp = param.get("boolean");
	if (tmp.empty()) {
		this->boolean = FALSE;
	} else {
		this->boolean = la::boolean::toBoolean(tmp);
	}

	/* main.sub */
	main_sub = param.get("main.sub");
}

void test::save(const std::string &filename) {
	std::string tmp;
	la::parameter param;

	/* key */
	param.add("key", this->key);

	/* integer */
	param.add("integer", la::number::toString(this->integer));

	/* boolean */
	param.add("boolean", la::boolean::toString(this->boolean));

	/* main.sub */
	param.add("main.sub", this->main_sub);

	param.saveToFile(filename);
}

void test::open(const std::string &filename) {
	if (!la::file::exists(filename)) {
		this->save(filename);
	}

	std::string cmd;
#ifdef SYSTEM_OS_TYPE_WINDOWS
	cmd += "notepad.exe";
#else
	if (la::file::exists("/usr/bin/vim")) {
		cmd += "/usr/bin/vim";
	} else if (la::file::exists("/usr/bin/emacs")) {
		cmd += "/usr/bin/emacs";
	} else if (la::file::exists("/usr/bin/nano")) {
		cmd += "/usr/bin/nano";
	} else if (la::file::exists("/bin/vi")) {
		cmd += "/bin/vi";
	} else {
		la::message::error("no editor found");
	}
#endif
	cmd += " ";
	cmd += filename;

	system(cmd.c_str());

	this->load(filename);
}

void test::edit() {
	char buffer[PARAMETER_VALUE_SIZE + 1];
	std::string tmp;

	/* key */
	std::cout << "key [" <<  this->key << "]: " << std::flush;
	std::cin.clear();
	std::cin.getline(buffer, '\n');
	if (buffer[0] != '\0') {
		tmp = la::string::trim(buffer);
		this->key = tmp;
	}

	/* integer */
	std::cout << "integer [" << this->integer << "]: " << std::flush;;
	std::cin.clear();
	std::cin.getline(buffer, '\n');
	if (buffer[0] != '\0') {
		tmp = la::string::trim(buffer);
		if (tmp.empty() || !la::number::isInteger(tmp)) {
			la::message::error("invalid value");
		}
		this->integer = la::number::toInteger(tmp);
	}

	/* boolean */
	std::cout << "boolean [" << la::boolean::toString(this->boolean) << "]: " << std::flush;
	std::cin.clear();
	std::cin.getline(buffer, '\n');
	if (buffer[0] != '\0') {
		tmp = la::string::trim(buffer);
		if (tmp.empty() || !la::boolean::isBoolean(tmp)) {
			la::message::error("invalid value");
		}
		this->boolean = la::boolean::toBoolean(tmp);
	}

	/* main.sub */
	std::cout << "main.sub [" <<  this->main_sub << "]: " << std::flush;
	std::cin.clear();
	std::cin.getline(buffer, '\n');
	if (buffer[0] != '\0') {
		tmp = la::string::trim(buffer);
		this->main_sub = tmp;
	}
}

void test::setKey(const std::string &value) {
	if (value.length() > PARAMETER_VALUE_SIZE) {
		la::message::error("value to long");
	}

	this->key = value;
}

std::string test::getKey() {
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
	if (value.length() > PARAMETER_VALUE_SIZE) {
		la::message::error("value to long");
	}

	this->main_sub = value;
}

std::string test::getMainSub() {
	return this->main_sub;
}

int main(void) {
	test test;
	test.load("test.properties");

	test.edit();
	test.show();
//	test.open("test.properties");

	test.save("test.backup");

	return 0;
}
