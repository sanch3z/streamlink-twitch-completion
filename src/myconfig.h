#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef MYCONFIG_H
#define MYCONFIG_H 1

#define PATH_BUFFSIZE 1024
#define MYCONF_KEYNAME_MAX_LENGTH 127
#define TWITCH_TOKEN_LENGTH 30

typedef struct helix_auth
{
	char user_id[TWITCH_TOKEN_LENGTH+1];
	char client_id[TWITCH_TOKEN_LENGTH+1];
	char bearer_token[TWITCH_TOKEN_LENGTH+1];
} helix_auth;

helix_auth *read_config(void);

#endif
