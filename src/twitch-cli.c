#include <getopt.h>

#include "twitch_helix.h"
#include "mystring.h"
#include "myopt.h"
#include "myconfig.h"

int main(int argc, char *argv[])
{
	twitch_node *channels_ptr = get_twitch_channels();

	if (channels_ptr == NULL)
	{
		fprintf(stderr, "Failed to retrieve twitch channels.");
		exit(EXIT_FAILURE);
	}

	int opt;
	int longindex = -1;
	for (;;)
	{
		opt = getopt_long(argc, argv, shortopts, longopts, &longindex);

		if (opt == -1)
			break;

		switch (opt) {
			case 'c':
				do
				{
					printf("%s ", channels_ptr->channel.user_login);
					channels_ptr = channels_ptr->next;
				}
				while (channels_ptr != NULL);
				break;

			default:
		}
	}

	if (longindex == -1)
	{
		do 
		{
			printf("%-25s - %s\n",	channels_ptr->channel.user_name,
									channels_ptr->channel.title);
			channels_ptr = channels_ptr->next;
		}
		while (channels_ptr != NULL);
	}

	exit(EXIT_SUCCESS);
}
