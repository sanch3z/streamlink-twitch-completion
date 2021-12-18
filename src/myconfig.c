#include "myconfig.h"

helix_auth ha;

const char cfgfile[] = "twitch-live/auth.conf";


static void build_cfg_path(char *cfgpath);

helix_auth *read_config(void) 
{
	FILE *fp;

	char cfgpath[PATH_BUFFSIZE];
	char name[MYCONF_KEYNAME_MAX_LENGTH+1];
	char value[TWITCH_TOKEN_LENGTH+1];


	build_cfg_path(cfgpath);

	fp = fopen(cfgpath, "r");

	if (fp == NULL)
	{
		fprintf(stderr, "Could not open auth.conf");
		exit(EXIT_FAILURE);
	}

	while (fscanf(fp, " %127[^= ] =%30s", name, value) == 2)
	{
		if (!strcmp("user_id", name))
		{
			strncpy(ha.user_id, value, sizeof(ha.user_id));
		}
		if (!strcmp("client_id", name))
		{
			strncpy(ha.client_id, value, sizeof(ha.client_id));
		}
		if (!strcmp("bearer_token", name))
		{
			strncpy(ha.bearer_token, value, sizeof(ha.bearer_token));
		}
	}

	fclose(fp);

	return &ha;
}

static void build_cfg_path(char *cfgpath)
{
	if (getenv("XDG_CONFIG_HOME"))
	{
		snprintf(cfgpath, PATH_BUFFSIZE, "%s/%s", getenv("XDG_CONFIG_HOME"), cfgfile);
	}
	else if (getenv("HOME"))
	{
		snprintf(cfgpath, PATH_BUFFSIZE, "%s/.config/%s", getenv("HOME"), cfgfile);
	}
	else
	{
		strncpy(cfgpath, "./auth.conf", PATH_BUFFSIZE);
	}
}
