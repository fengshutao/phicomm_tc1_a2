
#ifndef _MQTT_H_
#define _MQTT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hsf.h"
#include "user_config.h"

#pragma pack(push)
#pragma pack(1)

#pragma pack(pop)


void save_mqtt_config(MQTT_CONFIG *mqtt);
void update_mqtt_config(MQTT_CONFIG *mqtt);
void mqtt_para_init(void);

void user_mqtt_topic_publish(char* topic, char *data);
void user_mqtt_publish(char *data);

void mqtt_report_config(void);
void mqtt_report_status(void);
void mqtt_report_plug_status(uint8_t);

#ifdef __cplusplus
}
#endif

#endif

