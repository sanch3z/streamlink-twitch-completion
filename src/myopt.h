#include "getopt.h"

#ifndef MYOPT_H
#define MYOPT_H 1

const struct option longopts[] = {
	{
		.name = "completion",
		.has_arg = no_argument,
		.flag = NULL,
		.val = 'c'
	},
	{ 0, 0, 0, 0 }
};

const char *shortopts = "c";

#endif
