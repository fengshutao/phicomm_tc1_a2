
#include <string.h>

#include "user_config.h"
#include "cJSON/cJSON.h"

unsigned char crc_calc(unsigned char *data, int len)
{
	unsigned int crc = 0;
	int i;
	for (i = 0; i < len; i++)
		crc += data[i];

	return crc;
}

void get_user_config_str(char *res, uint8_t x)
{
	cJSON *json_send = cJSON_CreateObject();
	cJSON_AddStringToObject(json_send, "mac", strMac);
	cJSON_AddStringToObject(json_send, "IP", strIp);
	char time_s[20] = {0};
	get_time_string(time_s, 20);
	cJSON_AddStringToObject(json_send, "time", time_s);

	uint8_t i, j;

	if (x >= PLUG_NUM)
	{
		i = 0;
	}
	else {
		i = x;
	}

	// for (i = 0; i < PLUG_NUM; i++)
	{
		cJSON *json_plug_send = cJSON_CreateObject();
		// cJSON_AddNumberToObject(json_plug_send, "status", plug_status.plug[i]);

		cJSON *json_tasks_send = cJSON_CreateArray();
		for (j = 0; j < PLUG_TIME_TASK_NUM; j++)
		// j = 0;
		{
			uint8_t repeat = user_plug_config.plug[i].task[j].repeat;
			char plug_task_config_str[100];
			sprintf(plug_task_config_str,
				"id: %d, time: %d:%d:%d, repeat:%d, days:%d,%d,%d,%d,%d,%d,%d, action:%d, enable:%d",
				j,
				user_plug_config.plug[i].task[j].hour, user_plug_config.plug[i].task[j].minute, user_plug_config.plug[i].task[j].second,
				repeat & 1,
				1 * (repeat & (1 << 1)) >> 1,
				2 * (repeat & (1 << 2)) >> 2,
				3 * (repeat & (1 << 3)) >> 3,
				4 * (repeat & (1 << 4)) >> 4,
				5 * (repeat & (1 << 5)) >> 5,
				6 * (repeat & (1 << 6)) >> 6,
				7 * (repeat & (1 << 7)) >> 7,
				user_plug_config.plug[i].task[j].action,
				user_plug_config.plug[i].task[j].enable);
			// sprintf(plug_task_config_str, "%s", "tttttt");
			cJSON_AddItemToArray(json_tasks_send, cJSON_CreateString(plug_task_config_str));
		}
		// cJSON_AddItemToObject(json_plug_send, "tasks", json_tasks_send);

		cJSON_AddItemToArray(json_tasks_send, cJSON_CreateString("test"));

		char strTemp1[] = "plug_X";
		strTemp1[5] = i + '0';
		cJSON_AddStringToObject(json_plug_send, "name", strTemp1);
		cJSON_AddItemToObject(json_plug_send, "tasks", json_tasks_send);
		cJSON_AddItemToObject(json_send, strTemp1, json_plug_send);
	}

	char *json_str = cJSON_Print(json_send);
	strcpy(res, json_str);

	hfmem_free(json_str);
	cJSON_Delete(json_send);
}

void get_user_config_simple_str(char *res)
{
	cJSON *json_send = cJSON_CreateObject();
	cJSON_AddStringToObject(json_send, "mac", strMac);
	cJSON_AddStringToObject(json_send, "IP", strIp);
	cJSON_AddStringToObject(json_send, "ssid", (char *)g_hf_config_file.sta_ssid);
	cJSON_AddStringToObject(json_send, "password", (char *)g_hf_config_file.sta_key);
	char time_s[20] = {0};
	get_time_string(time_s, 20);
	cJSON_AddStringToObject(json_send, "time", time_s);

	int i, j;
	char strTemp1[] = "plug_X";

	for (i = 0; i < PLUG_NUM; i++)
	{
		cJSON *json_plug_send = cJSON_CreateObject();
		cJSON_AddNumberToObject(json_plug_send, "status", plug_status.plug[i]);

		strTemp1[5] = i + '0';
		cJSON_AddItemToObject(json_send, strTemp1, json_plug_send);
	}

	cJSON *json_mqtt_send = cJSON_CreateObject();
	cJSON_AddStringToObject(json_mqtt_send, "ip", user_mqtt_config.seraddr);
	cJSON_AddStringToObject(json_mqtt_send, "sub_topic", user_mqtt_config.sub_topic);
	cJSON_AddNumberToObject(json_mqtt_send, "connected", mqtt_is_connected);

	cJSON_AddItemToObject(json_send, "mqtt", json_mqtt_send);
	cJSON_AddNumberToObject(json_send, "plug_config_loaded", plug_config_loaded);
	cJSON_AddNumberToObject(json_send, "plug_status_loaded", plug_status_loaded);
	cJSON_AddNumberToObject(json_send, "mqtt_config_loaded", mqtt_config_loaded);

	cJSON_AddNumberToObject(json_send, "version", version);

	// extern time_t timestamp_start;
	// extern time_t utc8ts;
	// extern time_t ntp_res;
	// extern time_t systime_now;
	// cJSON_AddNumberToObject(json_send, "ntp_res", ntp_res);
	// cJSON_AddNumberToObject(json_send, "sysnow", systime_now);
	// cJSON_AddNumberToObject(json_send, "start_time", timestamp_start);
	// cJSON_AddNumberToObject(json_send, "now_ts", utc8ts);


	char *json_str = cJSON_Print(json_send);
	strcpy(res, json_str);

	hfmem_free(json_str);
	cJSON_Delete(json_send);
}

void default_plug_config(PLUG_CONFIG *config)
{
	memset((char *)config, 0, sizeof(MQTT_CONFIG));
	for (uint8_t i = 0; i < PLUG_NUM; i++)
	{
		for (uint8_t j = 0; j < PLUG_TIME_TASK_NUM; j++)
		{
			config->plug[i].task[j].enable = 0;
			config->plug[i].task[j].enable = 0;
			config->plug[i].task[j].hour = 24;
			config->plug[i].task[j].minute = 60;
			config->plug[i].task[j].second = 60;
			config->plug[i].task[j].repeat = 0;
		}
	}
}

void save_plug_config(PLUG_CONFIG *config)
{
	config->magic_head = PLUG_CONFIG_MAGIC_HEAD;
	config->crc = crc_calc((unsigned char *)config, sizeof(PLUG_CONFIG) - 1);
	hffile_userbin_write(PLUG_CONFIG_USERBIN_ADDR, (char *)config, sizeof(PLUG_CONFIG));
}

void init_plug_config(void)
{
	unsigned char crc;
	memset((char *)&user_plug_config, 0, sizeof(PLUG_CONFIG));
	hffile_userbin_read(PLUG_CONFIG_USERBIN_ADDR, (char *)&user_plug_config, sizeof(PLUG_CONFIG));
	crc = crc_calc((unsigned char *)&user_plug_config, sizeof(PLUG_CONFIG) - 1);
	if (!(PLUG_CONFIG_MAGIC_HEAD == user_plug_config.magic_head && crc == user_plug_config.crc))
	{
		default_plug_config(&user_plug_config);
		save_plug_config(&user_plug_config);
		plug_config_loaded = 2;
	}
	else
	{
		plug_config_loaded = 1;
	}
}

void default_plug_status(PLUG_STATUS *config)
{
	memset((char *)config, 0, sizeof(PLUG_STATUS));
	for (uint8_t i = 0; i < PLUG_NUM; i++)
	{
		config->plug[i] = 1;
	}
}

void save_plug_status(PLUG_STATUS *config)
{
	config->magic_head = PLUG_STATUS_MAGIC_HEAD;
	config->crc = crc_calc((unsigned char *)config, sizeof(PLUG_STATUS) - 1);
	hffile_userbin_write(PLUG_STATUS_USERBIN_ADDR, (char *)config, sizeof(PLUG_STATUS));
}

void init_plug_status(void)
{
	unsigned char crc;
	memset((char *)&plug_status, 0, sizeof(PLUG_STATUS));
	hffile_userbin_read(PLUG_STATUS_USERBIN_ADDR, (char *)&plug_status, sizeof(PLUG_STATUS));
	crc = crc_calc((unsigned char *)&plug_status, sizeof(PLUG_STATUS) - 1);
	if (!(PLUG_STATUS_MAGIC_HEAD == plug_status.magic_head && crc == plug_status.crc))
	{
		default_plug_status(&plug_status);
		save_plug_status(&plug_status);
		plug_status_loaded = 2;
	}
	else
	{
		plug_status_loaded = 1;
	}
}

void user_config_init()
{
	uint8_t i = 0;
	user_plug_config_enable = false;
	strcpy(ntpserver, "ntp1.aliyun.com");
	update_mqtt_config_flag = false;
	update_plug_config_flag = false;
	for (i = 0; i < PLUG_NUM; i++)
	{
		update_plug_status_flag[i] = false;
	}

	plug_config_loaded = 0;
	plug_status_loaded = 0;
	mqtt_config_loaded = 0;
	mqtt_is_connected = 0;

	version = VER;

	// press_flag = 0;
	// release_flag = 0;
	// last_press_time = 0;
	// last_release_time = 0;

	// init_plug_config();
	// init_plug_status();
	// default_plug_config(&user_plug_config);
}