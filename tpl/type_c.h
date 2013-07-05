/*  Properties2Code v.0.2.0 by Stephan Laukien */

#ifndef TEST_H
#define TEST_H

#include <la_boolean.h>

void test_show();
void test_load(const char *filename);
void test_save(const char *filename);
void test_open(const char *filename);
//void test_edit();

void test_setKey(const char *value);
char *test_getKey();
void test_setInteger(const int value);
int test_getInteger();
void test_setBoolean(const BOOL value);
BOOL test_getBoolean();
void test_setMainSub(const char *value);
char *test_getMainSub();

#endif
