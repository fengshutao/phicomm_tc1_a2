
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


void mqtt_para_init(void);

void user_mqtt_topic_publish(char* topic, char *data);
void user_mqtt_publish(char *data);

void mqtt_report_status(void);
void mqtt_report_plug_status(uint8_t);

//MQTT AT cmd
int hf_atcmd_mqclientid(pat_session_t s,int argc,char *argv[],char *rsp,int len);

int hf_atcmd_mqipport(pat_session_t s,int argc,char *argv[],char *rsp,int len);

int hf_atcmd_mquserpwd(pat_session_t s,int argc,char *argv[],char *rsp,int len);

int hf_atcmd_mqstatus(pat_session_t s,int argc,char *argv[],char *rsp,int len);

int hf_atcmd_mqstart(pat_session_t s,int argc,char *argv[],char *rsp,int len);

int hf_atcmd_mqpublish(pat_session_t s,int argc,char *argv[],char *rsp,int len);

int hf_atcmd_mqsubscribe(pat_session_t s,int argc,char *argv[],char *rsp,int len);

int hf_atcmd_mqunsubscribe(pat_session_t s,int argc,char *argv[],char *rsp,int len);

int hf_atcmd_mqautosub(pat_session_t s,int argc,char *argv[],char *rsp,int len);

int hf_atcmd_mqres(pat_session_t s,int argc,char *argv[],char *rsp,int len);

int hf_atcmd_mqver(pat_session_t s,int argc,char *argv[],char *rsp,int len);

int hf_atcmd_mqwill(pat_session_t s,int argc,char *argv[],char *rsp,int len);

//THROUGH mode uart message 
void uart_message_publish(char *data, int len);

#ifdef __cplusplus
}
#endif

#endif

