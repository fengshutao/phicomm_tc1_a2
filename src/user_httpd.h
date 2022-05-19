#ifndef _WEBUI_H_
#define _WEBUI_H_

#include <hsf.h>

typedef struct _config_data
{
	int first_start_flag;
	int heartbeat_rate;
	int scan_frequence;
	int reset_frequence;
} web_info_data_t;

int USER_FUNC httpd_add_page(const char *url, void (*callback)(char *url, char *rsp));
int USER_FUNC httpd_arg_find(char *url,char *slot0,char *slot1,char *slot2,char *slot3,char *slot4,char *slot5);

int hfhttpd_yz_nvget(char *cfg_name,int name_len, char *value, int valu_len);
int hfhttpd_yz_nvset(char *cfg_name,int name_len, char *value, int value_len);

void USER_FUNC httpd_init(void);

#endif
