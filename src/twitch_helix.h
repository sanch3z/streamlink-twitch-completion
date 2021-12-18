#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>
#include <curl/curl.h>

#include "mystring.h"
#include "myconfig.h"

#ifndef TWITCH_HELIX_H
#define TWITCH_HELIX_H 1

#define HELIX_URL "https://api.twitch.tv/helix"

typedef struct twitch_channel
{
	const char *user_id;
	const char *user_name;
	const char *user_login;

	const char *title;
	const char *viewer_count;

	const char *game_id;
	const char *game_name;
} twitch_channel;

typedef struct twitch_node
{
	twitch_channel channel;
	struct twitch_node *next;
} twitch_node;

twitch_node *get_twitch_channels(void);

#endif
