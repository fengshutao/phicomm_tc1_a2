#ifndef _HTTPSD_H_
#define _HTTPSD_H_

#define DEFAULT_PARA_HB_RATE			         (300)		//300s
#define DEFAULT_PARA_SCAN_RATE			  (600)		//600s
#define DEFAULT_PARA_RESET_RATE			  (1440)		//1440MIN=24H
#define HEART_BEAT_TIMER_ID       (1)
#define WIFI_SCAN_AP_TIMER_ID  (2)
#define WIFI_RESET_TIMER_ID       (3)
#define CONFIG_FLASH_ADDR 0

typedef struct _config_data
{
	int first_start_flag;
	int heartbeat_rate;
	int scan_frequence;
	int reset_frequence;
} web_info_data_t;

int hfhttpd_yz_nvget(char *cfg_name,int name_len, char *value, int valu_len);
int hfhttpd_yz_nvset(char *cfg_name,int name_len, char *value, int value_len);
void  wifi_module_init(void);
void wifi_scan_thread_func(void);


#endif

