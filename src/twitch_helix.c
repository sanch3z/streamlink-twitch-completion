#include "twitch_helix.h"

static twitch_node *create_twitch_node();

static void curl_twitch_channels(struct string *s);
static size_t curl_writefunc(void *ptr, size_t size, size_t nmemb, struct string *s);

twitch_node *get_twitch_channels(void)
{
	struct string s;

	json_object *json_followed, *json_followed_arr;

	init_string(&s);
	curl_twitch_channels(&s);

	enum json_tokener_error jerr = json_tokener_error_parse_null;

	json_followed = json_tokener_parse_verbose(s.ptr, &jerr);
	// printf("jobj from str:\n---\n%s\n---\n", json_object_to_json_string_ext(json_followed, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY));
	if (jerr != json_tokener_success) {
		fprintf(stderr, "json_tokener_parse_verbose() failed\n");
		exit(jerr);
	}

	json_object_object_get_ex(json_followed, "data", &json_followed_arr);

	twitch_node *head = NULL;
	twitch_node *prev = NULL; 

	for (int i=0; i<json_object_array_length(json_followed_arr); i++)
	{
		twitch_node *node = create_twitch_node();

		json_object_object_foreach(
			json_object_array_get_idx(json_followed_arr, i),
			key, val)
		{
			if (!strcmp("user_id", key))
			{
				node->channel.user_id = json_object_get_string(val);
			}
			if (!strcmp("user_login", key))
			{
				node->channel.user_login = json_object_get_string(val);
			}
			if (!strcmp("user_name", key))
			{
				node->channel.user_name = json_object_get_string(val);
			}
			if (!strcmp("game_id", key))
			{
				node->channel.game_id = json_object_get_string(val);
			}
			if (!strcmp("game_name", key))
			{
				node->channel.game_name = json_object_get_string(val);
			}
			if (!strcmp("title", key))
			{
				node->channel.title = json_object_get_string(val);
			}
			if (!strcmp("viewer_count", key))
			{
				node->channel.viewer_count = json_object_get_string(val);
			}
		}

		if (prev)
			prev->next = node;
		else
			head = node;

		prev = node;
	}

	free(s.ptr);

	return head;
}

static void curl_twitch_channels(struct string *s)
{
	CURL *curl;
	CURLcode res;

	helix_auth *ha;
	char helix_followed_url[128];
	char header_clientid[64];
	char header_bearertoken[64];

	struct curl_slist *headers = NULL;


	curl = curl_easy_init();

	if (curl == NULL) {
		fprintf(stderr, "curl_easy_init() failed\n");
		exit (EXIT_FAILURE);
	}

	ha = read_config();

	snprintf(helix_followed_url, sizeof(helix_followed_url),
		"%s/streams/followed?user_id=%s&first=100", HELIX_URL, ha->user_id);
	snprintf(header_clientid, sizeof(header_clientid),
		"Client-ID: %s", ha->client_id);
	snprintf(header_bearertoken, sizeof(header_bearertoken),
		"Authorization: Bearer %s", ha->bearer_token);

	headers = curl_slist_append(headers, "Accept: application/json");
	headers = curl_slist_append(headers, "Content-Type: application/json");
	headers = curl_slist_append(headers, header_clientid);
	headers = curl_slist_append(headers, header_bearertoken);

	curl_easy_setopt(curl, CURLOPT_URL, helix_followed_url);
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_writefunc);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, s);

	res = curl_easy_perform(curl);
	if (res != CURLE_OK) {
		fprintf(stderr, "curl_easy_perform() failed\n");
		exit(res);
	}

	curl_easy_cleanup(curl);
}

static size_t curl_writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
	size_t new_len = s->len + size*nmemb;
	s->ptr = realloc(s->ptr, new_len+1);
	if (s->ptr == NULL) {
		fprintf(stderr, "realloc() failed\n");
		exit(EXIT_FAILURE);
	}
	memcpy(s->ptr+s->len, ptr, size*nmemb); // copy ptr at end of str (s->ptr + s->len)
	s->ptr[new_len] = '\0';
	s->len = new_len;

	return size*nmemb;
}


static twitch_node *create_twitch_node()
{
	twitch_node *node = malloc(sizeof(*node));
	if (node == NULL) {
		fprintf(stderr, "malloc() failed\n");
		exit(EXIT_FAILURE);
	}
	node->next = NULL;
	return node;
}
