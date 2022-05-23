
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

void user_config_init()
{
	user_config.mqtt_config = &user_mqtt_config;
	user_config.plug = user_plug_config;
}

// void get_user_config_str(char *res)
// {
// 	cJSON *json_send = cJSON_CreateObject();
// 	cJSON_AddStringToObject(json_send, "mac", strMac);
// 	cJSON_AddStringToObject(json_send, "IP", strIp);
// 	cJSON_AddStringToObject(json_send, "ssid", (char *)g_hf_config_file.sta_ssid);
// 	cJSON_AddStringToObject(json_send, "password", (char *)g_hf_config_file.sta_key);
// 	char time_s[20] = {0};
// 	get_time_string(time_s, 20);
// 	cJSON_AddStringToObject(json_send, "time", time_s);

// 	int i, j;
// 	char strTemp1[] = "plug_X";

// 	for (i = 0; i < PLUG_NUM; i++)
// 	{
// 		cJSON *json_plug_send = cJSON_CreateObject();
// 		cJSON_AddStringToObject(json_plug_send, "name", user_plug_config[i].name);
// 		cJSON_AddNumberToObject(json_plug_send, "status", plug_status[i]);


// 		cJSON *json_tasks_send = cJSON_CreateArray();
// 		for (j = 0; j < PLUG_TIME_TASK_NUM; j++)
// 		{
// 			char plug_task_config_str[100];
// 			sprintf(plug_task_config_str,
// 					"id: %d, time: %d:%d:%d, repeat:%d, days:%d,%d,%d,%d,%d,%d,%d, action:%d, enable:%d",
// 					j,
// 					user_plug_config[i].task[j].hour, user_plug_config[i].task[j].minute, user_plug_config[i].task[j].second,
// 					user_plug_config[i].task[j].repeat[0],
// 					1 * user_plug_config[i].task[j].repeat[1],
// 					2 * user_plug_config[i].task[j].repeat[2],
// 					3 * user_plug_config[i].task[j].repeat[3],
// 					4 * user_plug_config[i].task[j].repeat[4],
// 					5 * user_plug_config[i].task[j].repeat[5],
// 					6 * user_plug_config[i].task[j].repeat[6],
// 					7 * user_plug_config[i].task[j].repeat[7],
// 					user_plug_config[i].task[j].action,
// 					user_plug_config[i].task[j].enable);
// 			cJSON_AddItemToArray(json_tasks_send, cJSON_CreateString(plug_task_config_str));
// 		}
// 		cJSON_AddItemToObject(json_plug_send, "tasks", json_tasks_send);

// 		strTemp1[5] = i + '0';
// 		cJSON_AddItemToObject(json_send, strTemp1, json_plug_send);
// 	}

// 	cJSON *json_mqtt_send = cJSON_CreateObject();
// 	cJSON_AddStringToObject(json_mqtt_send, "ip", user_mqtt_config.seraddr);
// 	cJSON_AddStringToObject(json_mqtt_send, "sub_topic", user_mqtt_config.sub_topic);
// 	cJSON_AddItemToObject(json_send, "mqtt", json_mqtt_send);

// 	char *json_str = cJSON_Print(json_send);
// 	strcpy(res, json_str);

// 	hfmem_free(json_str);
// 	cJSON_Delete(json_send);
// }

// void get_user_config_simple_str(char *res)
// {
// 	cJSON *json_send = cJSON_CreateObject();
// 	cJSON_AddStringToObject(json_send, "mac", strMac);
// 	cJSON_AddStringToObject(json_send, "IP", strIp);
// 	cJSON_AddStringToObject(json_send, "ssid", (char *)g_hf_config_file.sta_ssid);
// 	cJSON_AddStringToObject(json_send, "password", (char *)g_hf_config_file.sta_key);
// 	char time_s[20] = {0};
// 	get_time_string(time_s, 20);
// 	cJSON_AddStringToObject(json_send, "time", time_s);

// 	cJSON *json_mqtt_send = cJSON_CreateObject();
// 	cJSON_AddStringToObject(json_mqtt_send, "ip", user_mqtt_config.seraddr);
// 	cJSON_AddStringToObject(json_mqtt_send, "sub_topic", user_mqtt_config.sub_topic);
// 	cJSON_AddItemToObject(json_send, "mqtt", json_mqtt_send);

// 	char *json_str = cJSON_Print(json_send);
// 	strcpy(res, json_str);

// 	hfmem_free(json_str);
// 	cJSON_Delete(json_send);
// }