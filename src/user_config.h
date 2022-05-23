
#ifndef __USER_STRUCT_H_
#define __USER_STRUCT_H_

#include <hsf.h>

#define VERSION "v0.1"
#define TYPE 1
#define TYPE_NAME "TC1A2"

// #define SETTING_MQTT_STRING_LENGTH_MAX 32 //必须 4 字节对齐。

#define PLUG_NAME_LENGTH 32
#define PLUG_NUM 6           //插座数量
#define PLUG_TIME_TASK_NUM 5 //每个插座最多5组定时任务

#define NETWORK_ADDR_LEN (16) /* IP网络地址的长度 */
#define HAL_MAC_LEN (17 + 1)  /* MAC地址的长度 */

#define Relay_ON 1
#define Relay_OFF 0

#define Relay_NUM PLUG_NUM

#define MQTT_CONFIG_USERBIN_ADDR 0
#define MQTT_SERADDR_MAX_LEN 128
#define MQTT_CLIENTID_MAX_LEN 128
#define MQTT_USERNAME_MAX_LEN 128
#define MQTT_PASSWORD_MAX_LEN 128
#define MQTT_TOPIC_MAX_LEN 128
#define MQTT_MESSAGE_MAX_LEN 128
#define MQTT_MAGIC_HEAD 0x5A5AA5A5

typedef struct _TASK_CONFIG
{
    unsigned short hour;   //小时
    unsigned short minute; //分钟
    unsigned short second; //秒钟
    unsigned short action; //动作
    unsigned short enable; //开关
    uint8_t repeat[8]; 
} TASK_CONFIG;

typedef struct _PLUG_CONFIG
{
    char name[PLUG_NAME_LENGTH];
    // unsigned short status; //记录当前开关
    TASK_CONFIG task[PLUG_TIME_TASK_NUM];
} PLUG_CONFIG;

typedef struct _MQTT_CONFIG
{
    unsigned int magic_head;
    char seraddr[MQTT_SERADDR_MAX_LEN + 1];
    unsigned short port;
    char clientid[MQTT_CLIENTID_MAX_LEN + 1];
    char username[MQTT_USERNAME_MAX_LEN + 1];
    char password[MQTT_PASSWORD_MAX_LEN + 1];
    unsigned char enable_sub;
    char sub_topic[MQTT_TOPIC_MAX_LEN + 1]; // for at+mqautosub topic
    char topic[MQTT_TOPIC_MAX_LEN + 1];     // for at+mqsubscribe topic
    unsigned char sub_qos;
    unsigned char mqtt_ver;
    char will_topic[MQTT_TOPIC_MAX_LEN + 1];
    char will_msg[MQTT_MESSAGE_MAX_LEN + 1];
    char pub_topic[MQTT_TOPIC_MAX_LEN + 1];
    unsigned char crc;
} MQTT_CONFIG;

typedef struct _USER_CONFIG
{
    MQTT_CONFIG *mqtt_config;
    PLUG_CONFIG *plug;
} USER_CONFIG;

extern HF_CONFIG_FILE g_hf_config_file;
unsigned short plug_status[PLUG_NUM];

// TODO: 别的地方用这个 config 会导致编译崩溃
// USER_CONFIG user_config;
// USER_CONFIG test;
MQTT_CONFIG user_mqtt_config;
PLUG_CONFIG user_plug_config[PLUG_NUM];

///extern char rtc_init;
char strMac[13];
char strIp[32];
uint32_t power;
char deviceid[32];
char ntpserver[] = "ntp1.aliyun.com";

void user_config_init(void);
unsigned char crc_calc(unsigned char *, int);
void get_user_config_str(char *);
void get_user_config_simple_str(char *);

bool update_mqtt_config_flag = false;
bool update_plug_config_flag = false;
bool update_plug_status_flag = false;

#endif
