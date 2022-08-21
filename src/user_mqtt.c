
#include <hsf.h>
#include "user_mqtt.h"
#include "user_function.h"
#include "user_gpio.h"
#include "MQTTClient.h"
#include "cJSON.h"

#define MQTT_DEBUG_LEVEL 5

static Client MQTTCli;
// static int mqtt_is_connected = 0;

#define MQTTCliBufLen 1024
#define MQTTCliReadbufLen 1024
#define MQTTCli_Command_Timeout_MS 1000
#define MQTTCli_KeepAliveInterval_S 120

char mqtt_topic_buff[MQTT_TOPIC_MAX_LEN + 1] = {0};
char mqtt_content_buff[USER_BUFF_SIZE] = {0};

char plug_cmd_topic_list[PLUG_NUM][MQTT_TOPIC_MAX_LEN + 1];
char plug_status_topic_list[PLUG_NUM][MQTT_TOPIC_MAX_LEN + 1];
char plug_config_topic_list[PLUG_NUM][MQTT_TOPIC_MAX_LEN + 1];
char cmd_topic[MQTT_TOPIC_MAX_LEN + 1];

const char hass_plug_config[] = "{\"name\":\"%s_%d\",\"unique_id\":\"%s_%d\",\"~\":\"%s/plug_%d\",\"state_topic\":\"~/status\",\"command_topic\":\"~/cmd\"}";

static void mqtt_status_callback(int connect)
{
	if (connect)
	{
		if (mqtt_is_connected == 0)
			hfuart_send(HFUART0, "+MQSTATUS:MQTT CONNECTED", strlen("+MQSTATUS:MQTT CONNECTED"), 0);
		mqtt_is_connected = 1;
	}
	else
	{
		if (mqtt_is_connected)
			hfuart_send(HFUART0, "+MQSTATUS:MQTT CLOSED", strlen("+MQSTATUS:MQTT CLOSED"), 0);
		mqtt_is_connected = 0;
	}
}

static void topic_message_publish(char *topic, char *messagem, int message_len, int qos)
{
	MQTTMessage message;

	message.qos = qos;
	message.retained = FALSE;
	message.dup = FALSE;
	message.payload = (void *)messagem;
	message.payloadlen = message_len;
	MQTTPublish(&MQTTCli, topic, &message);
}

static void topic_message_publish_retained(char *topic, char *messagem, int message_len, int qos)
{
	MQTTMessage message;

	message.qos = qos;
	message.retained = TRUE;
	message.dup = FALSE;
	message.payload = (void *)messagem;
	message.payloadlen = message_len;
	MQTTPublish(&MQTTCli, topic, &message);
}

static void topic_message_callback(MessageData *md)
{
	char *data = mqtt_content_buff;
	if (data == NULL)
		return;

	sprintf(mqtt_topic_buff, "%.*s", md->topicName->lenstring.len, md->topicName->lenstring.data);
	sprintf(data, "%.*s", (int)md->message->payloadlen, (char *)md->message->payload);
	for (uint8_t i = 0; i < PLUG_NUM; i++)
	{
		if (strcmp(mqtt_topic_buff, plug_cmd_topic_list[i]) == 0)
		{
			if (strcmp(data, "ON") == 0)
			{
				user_relay_set(i, 1);
			}
			else if (strcmp(data, "OFF") == 0)
			{
				user_relay_set(i, 0);
			}
		}
	}

	if (strcmp(mqtt_topic_buff, cmd_topic) == 0)
	{
		char cmd_rsp[50] = {0};
		user_function_cmd_received((char *)md->message->payload, (int)md->message->payloadlen, cmd_rsp);
		user_mqtt_publish(cmd_rsp);
	}

	// hfuart_send(HFUART0, data, strlen(data), 0);
	// hfuart_send(HFUART0, "uart", strlen("uart"), 0);
	// hfmem_free(data);
}

static void MQTTClient_thread(void *arg)
{
	int ping_time = 0;
	int report_time = 0;
	HF_Debug(MQTT_DEBUG_LEVEL, "MQTTClient_thread start.\r\n");

	int ret;
	MQTTPacket_connectData MQTTConnectData = MQTTPacket_connectData_initializer;
	Network cliNetwork;
	NewNetwork(&cliNetwork);

	char *MQTTCliBuf;
	MQTTCliBuf = (char *)hfmem_malloc(MQTTCliBufLen);
	char *MQTTCliReadbuf;
	MQTTCliReadbuf = (char *)hfmem_malloc(MQTTCliReadbufLen);

	while (1)
	{
		HF_Debug(MQTT_DEBUG_LEVEL, "Wait WiFi link ...");
		while (!hfwifi_sta_is_connected())
		{
			msleep(1000);
		}
		HF_Debug(MQTT_DEBUG_LEVEL, "connected.\r\n");

		if (SUCCESS != ConnectNetwork(&cliNetwork, user_mqtt_config.seraddr, user_mqtt_config.port))
		{
			HF_Debug(MQTT_DEBUG_LEVEL, "TCPSocket connect fail!\r\n");
			goto MQTT_END;
		}
		HF_Debug(MQTT_DEBUG_LEVEL, "TCPSocket connect success.\r\n");

		MQTTClient(&MQTTCli, &cliNetwork, MQTTCli_Command_Timeout_MS, (unsigned char *)MQTTCliBuf, MQTTCliBufLen, (unsigned char *)MQTTCliReadbuf, MQTTCliReadbufLen);
		if (strlen(user_mqtt_config.will_topic) > 0 && strlen(user_mqtt_config.will_msg) > 0)
		{
			MQTTConnectData.willFlag = 1;
			MQTTConnectData.will.topicName.cstring = user_mqtt_config.will_topic;
			MQTTConnectData.will.message.cstring = user_mqtt_config.will_msg;
		}
		else
			MQTTConnectData.willFlag = 0;
		MQTTConnectData.MQTTVersion = user_mqtt_config.mqtt_ver;
		MQTTConnectData.clientID.cstring = user_mqtt_config.clientid;
		MQTTConnectData.username.cstring = user_mqtt_config.username;
		MQTTConnectData.password.cstring = user_mqtt_config.password;
		MQTTConnectData.keepAliveInterval = MQTTCli_KeepAliveInterval_S;
		MQTTConnectData.cleansession = 1;

		if (SUCCESS != MQTTConnect(&MQTTCli, &MQTTConnectData))
		{
			HF_Debug(MQTT_DEBUG_LEVEL, "MQTTConnect fail!\r\n");
			goto MQTT_END;
		}
		HF_Debug(MQTT_DEBUG_LEVEL, "MQTTConnect success.\r\n");

		if (user_mqtt_config.enable_sub)
		{
			if (SUCCESS == MQTTSubscribe(&MQTTCli, user_mqtt_config.sub_topic, user_mqtt_config.sub_qos, topic_message_callback))
				HF_Debug(MQTT_DEBUG_LEVEL, "Subscribe[%s] success.\r\n", user_mqtt_config.sub_topic);
			else
				HF_Debug(MQTT_DEBUG_LEVEL, "Subscribe[%s] fail!\r\n", user_mqtt_config.sub_topic);
			if (SUCCESS == MQTTSubscribe(&MQTTCli, "tc1_1", user_mqtt_config.sub_qos, topic_message_callback))
				HF_Debug(MQTT_DEBUG_LEVEL, "Subscribe[%s] success.\r\n", user_mqtt_config.sub_topic);
			else
				HF_Debug(MQTT_DEBUG_LEVEL, "Subscribe[%s] fail!\r\n", user_mqtt_config.sub_topic);
		}

		mqtt_status_callback(1);

		mqtt_report_config();

		ping_time = hfsys_get_time();
		report_time = hfsys_get_time();
		while (1)
		{
			ret = MQTTYield(&MQTTCli, 300);
			if (ret == FAILURE)
			{
				goto MQTT_END;
			}

			if (!hfwifi_sta_is_connected())
			{
				HF_Debug(MQTT_DEBUG_LEVEL, "MQTTClient_thread WiFi disconnected!\r\n");
				goto MQTT_END;
			}

			//定期上报一下开关状态
			if (hfsys_get_time() - report_time > 300 * 1000)
			{
				report_time = hfsys_get_time();
				mqtt_report_status();
			}

			//应用层实现ping功能 防止 mqtt异常断开
			if (hfsys_get_time() - ping_time > (MQTTConnectData.keepAliveInterval - 10) * 1000)
			{
				ping_time = hfsys_get_time();
				int len = MQTTSerialize_pingreq(MQTTCli.buf, MQTTCli.buf_size);
				if (len > 0)
				{
					MQTTCli.ipstack->mqttwrite(MQTTCli.ipstack, MQTTCli.buf, len, 100);
				}
			}
		}

	MQTT_END:
		HF_Debug(MQTT_DEBUG_LEVEL, "MQTT end!\r\n\r\n");
		cliNetwork.disconnect(&cliNetwork);
		mqtt_status_callback(0);
		msleep(3000);
	}

	if (MQTTCliBuf != NULL)
		hfmem_free(MQTTCliBuf);
	if (MQTTCliReadbuf != NULL)
		hfmem_free(MQTTCliReadbuf);

	HF_Debug(MQTT_DEBUG_LEVEL, "MQTTClient_thread destroy.\r\n");
	msleep(1000);
	hfthread_destroy(NULL);
}

static void mqtt_start(void)
{
	static int mqtt_init = 0;
	if (mqtt_init)
		return;

	if (hfthread_create(MQTTClient_thread, "MQTT_Cli", 512, NULL, HFTHREAD_PRIORITIES_LOW, NULL, NULL) != HF_SUCCESS)
	{
		HF_Debug(DEBUG_ERROR, "start MQTTClient_thread fail\n");
	}
	mqtt_init = 1;
}

static void default_mqtt_config(MQTT_CONFIG *mqtt)
{
	memset((char *)mqtt, 0, sizeof(MQTT_CONFIG));
	strcpy(mqtt->seraddr, "114.114.114.114");
	mqtt->port = 1883;
	strcpy(mqtt->clientid, strMac);
	strcpy(mqtt->hass_topic, "homeassistant");
	strcpy(mqtt->username, "admin");
	strcpy(mqtt->password, "admin");
	mqtt->sub_qos = 1;
	mqtt->enable_sub = 1;
	mqtt->mqtt_ver = 4;
	update_mqtt_config(mqtt);
}

void save_mqtt_config(MQTT_CONFIG *mqtt)
{
	mqtt->magic_head = MQTT_MAGIC_HEAD;
	mqtt->crc = crc_calc((unsigned char *)mqtt, sizeof(MQTT_CONFIG) - 1);
	hffile_userbin_write(MQTT_CONFIG_USERBIN_ADDR, (char *)mqtt, sizeof(MQTT_CONFIG));
}

void update_mqtt_config(MQTT_CONFIG *mqtt)
{
	sprintf(mqtt->pub_topic, "%s/switch/%s", mqtt->hass_topic, mqtt->clientid);
	sprintf(mqtt->sub_topic, "%s/switch/%s/+/cmd", mqtt->hass_topic, mqtt->clientid);
	sprintf(cmd_topic, "%s/switch/%s/config/cmd", mqtt->hass_topic, mqtt->clientid);
	for (uint8_t i = 0; i < PLUG_NUM; i++)
	{
		sprintf(plug_cmd_topic_list[i], "%s/plug_%d/cmd", mqtt->pub_topic, i);
		sprintf(plug_status_topic_list[i], "%s/plug_%d/status", mqtt->pub_topic, i);
		sprintf(plug_config_topic_list[i], "%s/plug_%d/config", mqtt->pub_topic, i);
	}
}

void uart_message_publish(char *data, int len)
{
	if (mqtt_is_connected)
	{
		topic_message_publish(user_mqtt_config.pub_topic, data, len, 0);
	}
}

void user_mqtt_topic_publish(char *topic, char *data)
{
	if (mqtt_is_connected)
	{
		topic_message_publish(topic, data, strlen(data), 0);
	}
}

void user_mqtt_publish(char *data)
{
	if (mqtt_is_connected)
	{
		topic_message_publish(user_mqtt_config.pub_topic, data, strlen(data), 0);
	}
}

void mqtt_report_config(void)
{
	for (uint8_t i = 0; i < PLUG_NUM; i++)
	{
		sprintf(mqtt_content_buff, hass_plug_config, user_mqtt_config.clientid, i, user_mqtt_config.clientid, i, user_mqtt_config.pub_topic, i);
		topic_message_publish_retained(plug_config_topic_list[i], mqtt_content_buff, strlen(mqtt_content_buff), 0);
		// user_mqtt_topic_publish(plug_config_topic_list[i], mqtt_topic_buff);
	}
}

void mqtt_clear_config(void)
{
	for (uint8_t i = 0; i < PLUG_NUM; i++)
	{
		sprintf(mqtt_content_buff, "{}");
		topic_message_publish_retained(plug_config_topic_list[i], mqtt_content_buff, strlen(mqtt_content_buff), 0);
		// user_mqtt_topic_publish(plug_config_topic_list[i], mqtt_topic_buff);
	}
}

void mqtt_report_status(void)
{
	for (uint8_t i = 0; i < PLUG_NUM; i++)
	{
		mqtt_report_plug_status(i);
	}
}

void mqtt_report_plug_status(uint8_t i)
{
	if (i < PLUG_NUM)
	{
		if (plug_status.plug[i] > 0)
		{
			user_mqtt_topic_publish(plug_status_topic_list[i], "ON");
		}
		else
		{
			user_mqtt_topic_publish(plug_status_topic_list[i], "OFF");
		}
	}
}

void mqtt_para_init(void)
{
	unsigned char crc;
	memset((char *)&user_mqtt_config, 0, sizeof(MQTT_CONFIG));
	hffile_userbin_read(MQTT_CONFIG_USERBIN_ADDR, (char *)&user_mqtt_config, sizeof(MQTT_CONFIG));
	crc = crc_calc((unsigned char *)&user_mqtt_config, sizeof(MQTT_CONFIG) - 1);
	if (!(MQTT_MAGIC_HEAD == user_mqtt_config.magic_head && crc == user_mqtt_config.crc))
	{
		default_mqtt_config(&user_mqtt_config);
		save_mqtt_config(&user_mqtt_config);
		mqtt_config_loaded = 2;
	}
	else
	{
		mqtt_config_loaded = 1;
	}
	update_mqtt_config(&user_mqtt_config);
	mqtt_start();
}
