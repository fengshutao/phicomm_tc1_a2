
#include <hsf.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "app_main.h"
#include "user_httpd.h"

#include "cJSON/cJSON.h"
#include "user_function.h"
#include "user_mqtt.h"
#include "user_rtc.h"

#define HFEASY_VERSION_MAJOR 0
#define HFEASY_VERSION_MINOR 4

web_info_data_t web_config_info;

enum
{
	HFTIMER_ID_DEBOUNCE = 0,
	HFTIMER_ID_RECOVERY,
	HFTIMER_ID_BUZZER,
	HFTIMER_ID_RESET,
	HFTIMER_ID_LED,
};

#define HTTPD_MAX_PAGES 10

static const char *http_header = "HTTP/1.1 200 OK\r\n"
								 "Server: HTTPD\r\n"
								 "Connection: close\r\n\r\n";

struct httpd_page
{
	const char *url;
	void (*callback)(char *url, char *rsp);
} httpd_pages[HTTPD_MAX_PAGES] = {
	{.url = NULL},
};

static void USER_FUNC convert_ascii(char *str)
{
	char *in, *out, c, s[3] = {0, 0, 0};

	in = str;
	out = str;

	while (*in != '\0')
	{
		if (*in == '%')
		{
			memcpy(s, in + 1, 2);
			c = strtol(s, NULL, 16);
			*(out++) = c;
			in += 3;
		}
		else
		{
			*(out++) = *(in++);
		}
	}
	*out = '\0';
}

int USER_FUNC httpd_arg_find(char *url, char *slot0, char *slot1, char *slot2, char *slot3, char *slot4, char *slot5)
{
	char *a;
	char *zero = "0";
	char *one = "1";
	a = strchr(url, '?');
	if (a == NULL)
		return 1;

	if (strstr(url, "slot0=0"))
	{
		strcpy(slot0, zero);
	}
	else if (strstr(url, "slot0=1"))
	{
		strcpy(slot0, one);
	}
	else
	{
		return 1;
	}

	if (strstr(url, "slot1=0"))
	{
		strcpy(slot1, zero);
	}
	else if (strstr(url, "slot1=1"))
	{
		strcpy(slot1, one);
	}
	else
	{
		return 1;
	}

	if (strstr(url, "slot2=0"))
	{
		strcpy(slot2, zero);
	}
	else if (strstr(url, "slot2=1"))
	{
		strcpy(slot2, one);
	}
	else
	{
		return 1;
	}

	if (strstr(url, "slot3=0"))
	{
		strcpy(slot3, zero);
	}
	else if (strstr(url, "slot3=1"))
	{
		strcpy(slot3, one);
	}
	else
	{
		return 1;
	}

	if (strstr(url, "slot4=0"))
	{
		strcpy(slot4, zero);
	}
	else if (strstr(url, "slot4=1"))
	{
		strcpy(slot4, one);
	}
	else
	{
		return 1;
	}

	if (strstr(url, "slot5=0"))
	{
		strcpy(slot5, zero);
	}
	else if (strstr(url, "slot5=1"))
	{
		strcpy(slot5, one);
	}
	else
	{
		return 1;
	}
	return 0;
}

int USER_FUNC httpd_add_page(const char *url, void (*callback)(char *url, char *rsp))
{
	int i;

	for (i = 0; i < HTTPD_MAX_PAGES; i++)
	{
		if (httpd_pages[i].url == NULL)
			break;
	}
	if (i == HTTPD_MAX_PAGES)
		return -1;

	httpd_pages[i].url = url;
	httpd_pages[i].callback = callback;

	return 0;
}

static int USER_FUNC httpd_callback(char *url, char *rsp)
{
	struct httpd_page *p = httpd_pages;
	char buf[50], *a;
	char *r = rsp;

	strncpy(buf, url, 50);
	buf[49] = '\0';
	a = strchr(buf, '?');
	if (a != NULL)
		*a = '\0';

	while (p->url != NULL)
	{
		if (strcmp(p->url, buf) == 0)
		{
			strcpy(r, http_header);
			r += strlen(http_header);
			p->callback(url, r);
			return 0;
		}
		p++;
	}
	return -1;
}

static const char *css_page = "* {font-family:sans-serif; font-size:12pt;}\r\n"
							  "h1 {font-size:16pt; color:black;}\r\n"
							  "h6 {font-size:10pt; color:black; text-align:center;}\r\n"
							  ".button-menu {background-color:#ffffff; color:blue; margin: 10px; text-decoration:none}\r\n"
							  ".button-link {padding:5px 15px; background-color:#0077dd; color:#fff; border:solid 1px #fff; text-decoration:none}\r\n"
							  ".button-menu:hover {background:#ddddff;}\r\n"
							  ".button-link:hover {background:#369;}\r\n"
							  "th {padding:10px; background-color:black; color:#ffffff;}\r\n"
							  "td {padding:7px;}\r\n"
							  "table {color:black;}\r\n";

void styles_cbk(char *url, char *rsp)
{
	strcpy(rsp, css_page);
}

// void switch_cbk(char *url, char *rsp)
// {
// 	char slot0[10];
// 	char slot1[10];
// 	char slot2[10];
// 	char slot3[10];
// 	char slot4[10];
// 	char slot5[10];
// 	int ret = httpd_arg_find(url, slot0, slot1, slot2, slot3, slot4, slot5);
// 	if (ret > 0)
// 	{
// 		strcpy(rsp, "ret > 0");
// 		return;
// 	}

// 	char *send_buf = (char *)malloc(strlen("{\"plug_0\":{\"on\":1},\"plug_1\":{\"on\":0},\"plug_2\":{\"on\":1},\"plug_3\":{\"on\":1},\"plug_4\":{\"on\":1},\"plug_5\":{\"on\":1}}"));
// 	sprintf(send_buf, "{\"plug_0\":{\"on\":%s},\"plug_1\":{\"on\":%s},\"plug_2\":{\"on\":%s},\"plug_3\":{\"on\":%s},\"plug_4\":{\"on\":%s},\"plug_5\":{\"on\":%s}}", slot0, slot1, slot2, slot3, slot4, slot5);

// 	// strcpy(rsp, send_buf);
// 	// return;

// 	cJSON *pJsonRoot = cJSON_Parse(send_buf);
// 	cJSON *json_send = cJSON_CreateObject();
// 	unsigned char i;
// 	bool update_user_config_flag = false;
// 	for (i = 0; i < PLUG_NUM; i++)
// 	{
// 		if (json_plug_analysis(1, i, pJsonRoot, json_send))
// 			update_user_config_flag = true;
// 	}
// 	strcpy(rsp, send_buf);
// }

void status_cbk_all(char *url, char *rsp)
{
	uint8_t plug = 0;
	char *param = strstr(url, "?plug=");
	if (param != NULL)
	{
		plug = param[6] - '0';
	}
	get_user_config_str(rsp, plug);
}

void status_cbk(char *url, char *rsp)
{
	get_user_config_simple_str(rsp);
}

void restart_cbk(char *url, char *rsp)
{
	strcpy(rsp, "restart ready!");
	system_restart_flag = true;
}

void config_cbk(char *url, char *rsp)
{
	char *param = strstr(url, "?json=");
	if (param != NULL)
	{
		strrpc(param, "\%22", "\"");
		strrpc(param, "\%20", " ");
		user_function_cmd_received(param + 6, strlen(param + 6), rsp);
	}
	else
	{
		strcpy(rsp, "null");
	}
	// strcpy(rsp, "<html><body>Welcome!</body></html>");
}

void USER_FUNC httpd_init(void)
{
	/* register url handler callback */
	if (hfhttpd_url_callback_register(httpd_callback, 0) != HF_SUCCESS)
		u_printf("error registering url callback\r\n");

	httpd_add_page("/styles.css", styles_cbk);
	httpd_add_page("/status", status_cbk);
	httpd_add_page("/task", status_cbk_all);
	httpd_add_page("/config", config_cbk);
	httpd_add_page("/restart", restart_cbk);
}
