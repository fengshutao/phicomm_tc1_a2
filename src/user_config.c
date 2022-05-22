
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

void get_user_config_str(char *res)
{
	cJSON *json_send = cJSON_CreateObject();
	cJSON_AddStringToObject(json_send, "mac", strMac);

	int i, j;

	for (i = 0; i < PLUG_NUM; i++)
	{
		char strTemp1[] = "plug_X";
		strTemp1[5] = i + '0';
		cJSON *json_send_plug = cJSON_CreateObject();
		cJSON_AddNumberToObject(json_send_plug, "status", plug_status[i]);

		cJSON *json_send_tasks = cJSON_CreateArray();
		for (j = 0; j < PLUG_TIME_TASK_NUM; j++)
		{
			char plug_task_config_str[100];
			sprintf(plug_task_config_str,
					"time: %d:%d:%d, repeat:%d, days:%d,%d,%d,%d,%d,%d,%d, action:%d, enable:%d",
					user_plug_config[i].task[j].hour, user_plug_config[i].task[j].minute, user_plug_config[i].task[j].second,
					user_plug_config[i].task[j].repeat[0],
					1 * user_plug_config[i].task[j].repeat[1],
					2 * user_plug_config[i].task[j].repeat[2],
					3 * user_plug_config[i].task[j].repeat[3],
					4 * user_plug_config[i].task[j].repeat[4],
					5 * user_plug_config[i].task[j].repeat[5],
					6 * user_plug_config[i].task[j].repeat[6],
					7 * user_plug_config[i].task[j].repeat[7],
					user_plug_config[i].task[j].action,
					user_plug_config[i].task[j].enable)
				cJSON *json_send_plug_task = cJSON_CreateObject();
			cJSON_AddNumberToObject(json_send_plug_task, "hour", user_config.plug[i].task[j].hour);
			cJSON_AddNumberToObject(json_send_plug_task, "minute", user_config.plug[i].task[j].minute);
			cJSON_AddNumberToObject(json_send_plug_task, "repeat", user_config.plug[i].task[j].repeat);
			cJSON_AddNumberToObject(json_send_plug_task, "action", user_config.plug[i].task[j].action);
			cJSON_AddNumberToObject(json_send_plug_task, "on", user_config.plug[i].task[j].enable);
			cJSON_AddItemToObject(json_send_plug_setting, strTemp2, json_send_plug_task);
			cJSON_AddItemToObject(json_send_plug, "setting", json_send_plug_setting);

			task_flag[i] = -1;

			cJSON_AddItemToObject(json_send, strTemp1, json_send_plug);
		}
	}

	char *json_str = cJSON_Print(json_send);
	user_send(false, json_str); //发送数据
	hfmem_free(json_str);
	cJSON_Delete(json_send);
}

void get_user_config_simple_str(char *res)
{
	char *power_temp_buf = malloc(16);

	sprintf(power_temp_buf, "%ld.%ld", power / 10, power % 10);

	char *send_buf = (char *)malloc(strlen("{\"slot0\":0,\"slot1\":0,\"slot2\":0,\"slot3\":0,\"slot4\":0,\"slot5\":0,\"power\":}") + strlen(power_temp_buf));

	sprintf(send_buf, "{\"slot0\":%d,\"slot1\":%d,\"slot2\":%d,\"slot3\":%d,\"slot4\":%d,\"slot5\":%d,\"power\":%s}", user_config.plug[0].status, user_config.plug[1].status, user_config.plug[2].status, user_config.plug[3].status, user_config.plug[4].status, user_config.plug[5].status, power_temp_buf);

	cJSON *json_plug_send = cJSON_CreateObject();
	cJSON *json_mqtt_send = cJSON_CreateObject();
	cJSON *json_wifi_send = cJSON_CreateObject();
	// cJSON *json_config_send = cJSON_CreateObject();
	cJSON *json_send = cJSON_CreateObject();

	cJSON_AddStringToObject(json_send, "plug", send_buf);

	char mac[21] = {0};
	hfnet_get_mac_address(mac);

	char ip_str[NETWORK_ADDR_LEN] = {0};
	int ip, mask, gw;
	hfnet_get_dhcp_ip(&ip, &mask, &gw);
	sprintf(ip_str, "%d.%d.%d.%d", ((ip >> (8 * 0)) & 0xFF), ((ip >> (8 * 1)) & 0xFF), ((ip >> (8 * 2)) & 0xFF), ((ip >> (8 * 3)) & 0xFF));

	cJSON_AddStringToObject(json_wifi_send, "mac", mac);
	cJSON_AddStringToObject(json_wifi_send, "ip", ip_str);
	cJSON_AddStringToObject(json_wifi_send, "ssid", (char *)g_hf_config_file.sta_ssid);
	cJSON_AddStringToObject(json_wifi_send, "password", (char *)g_hf_config_file.sta_key);

	char time_s[20] = {0};
	get_time_string(time_s, 20);
	cJSON_AddStringToObject(json_wifi_send, "time", time_s);
	cJSON_AddItemToObject(json_send, "wifi", json_wifi_send);

	cJSON_AddStringToObject(json_mqtt_send, "ip", user_mqtt_config.seraddr);
	cJSON_AddStringToObject(json_mqtt_send, "sub_topic", user_mqtt_config.sub_topic);
	cJSON_AddItemToObject(json_send, "mqtt", json_mqtt_send);
	// hfwifi_sta_get_current_bssid

	// get_user_config_info(json_config_send);
	// cJSON_AddItemToObject(json_send, "config", json_config_send);

	char *out = cJSON_Print(json_send);
	strcpy(rsp, out);
	cJSON_Delete(json_send);
	free(power_temp_buf);
	free(send_buf);
}