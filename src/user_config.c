
#include "user_config.h"

void user_config_init()
{
	memset((char *)&user_config, 0, sizeof(USER_CONFIG));
	hffile_userbin_read(USER_CONFIG_USERBIN_ADDR, (char *)&user_config, sizeof(USER_CONFIG));

    unsigned char crc;
	crc = crc_calc((unsigned char *)&user_config, sizeof(USER_CONFIG)-1);
	if(MQTT_MAGIC_HEAD == user_config.mqtt_config.magic_head && crc == user_config.crc)
	{
		// u_printf("[MQTT] mqtt config read success\r\n");
		// u_printf("[MQTT] mqtt version: %d\r\n", user_config.mqtt_config.mqtt_ver);
		// u_printf("[MQTT] mqtt ip: %s, port: %d\r\n", user_config.mqtt_config.seraddr, user_config.mqtt_config.port);
		// u_printf("[MQTT] mqtt clientid: %s\r\n", user_config.mqtt_config.clientid);
		// u_printf("[MQTT] mqtt username: %s\r\n", user_config.mqtt_config.username);
		// u_printf("[MQTT] mqtt password: %s\r\n", user_config.mqtt_config.password);
		// if(user_config.mqtt_config.enable_sub)
		// {
		// 	u_printf("[MQTT] mqtt subscribe topic: %s\r\n", user_config.mqtt_config.sub_topic);
		// 	u_printf("[MQTT] mqtt subscribe qos: %d\r\n", user_config.mqtt_config.sub_qos);
		// }
		// if(strlen(user_config.mqtt_config.will_topic) > 0 && strlen(user_config.mqtt_config.will_msg) > 0)
		// {
		// 	u_printf("[MQTT] mqtt will topic: %s\r\n", user_config.mqtt_config.will_topic);
		// 	u_printf("[MQTT] mqtt will message: %s\r\n", user_config.mqtt_config.will_msg);
		// }
	}
	else
	{
		// u_printf("[MQTT] mqtt config read failed, reinit\r\n");
		
		default_user_config(&user_config);
		save_user_config(&user_config);
	}
}

static unsigned char crc_calc(unsigned char *data, int len)
{
	unsigned int crc = 0;
	int i;
	for(i=0; i<len; i++)
		crc += data[i];

	return crc;
}

static void save_user_config(USER_CONFIG *user_config)
{
	user_config->plug->magic_head = MQTT_MAGIC_HEAD;
	mqtt->crc = crc_calc((unsigned char *)mqtt, sizeof(struct MQTT_CONFIG)-1);
	hffile_userbin_write(USER_CONFIG_USERBIN_ADDR, (char *)mqtt, sizeof(struct MQTT_CONFIG));
}

static void set_plug_para(PLUG_CONFIG *plug_config, char* name) {
    strcpy(plug_config->name, name);
    plug_config->on
}
