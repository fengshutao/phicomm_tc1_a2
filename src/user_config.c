
#include "user_config.h"


unsigned char crc_calc(unsigned char *data, int len)
{
	unsigned int crc = 0;
	int i;
	for(i=0; i<len; i++)
		crc += data[i];

	return crc;
}

void user_config_init()
{
	user_config.mqtt_config = &user_mqtt_config;
	user_config.plug = user_plug_config;
}

void get_user_config_str(char* res)
{

}