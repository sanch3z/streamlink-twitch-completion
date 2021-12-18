#include <stdio.h>
#include <stdlib.h>

#ifndef MYSTRING_H
#define MYSTRING_H 1

struct string
{
	char *ptr;
	size_t len;
};

void init_string(struct string *s);

#endif
