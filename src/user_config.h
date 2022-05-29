
#ifndef __USER_STRUCT_H_
#define __USER_STRUCT_H_

#include <hsf.h>

#define VERSION "v0.1"
#define VER 1
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

#define PLUG_STATUS_USERBIN_ADDR 2048
#define PLUG_STATUS_MAGIC_HEAD 0x6B6BB6B6

#define PLUG_CONFIG_USERBIN_ADDR 3072
#define PLUG_CONFIG_MAGIC_HEAD 0x7C7CC7C7

typedef struct _TASK_CONFIG
{
    unsigned short hour;   //小时
    unsigned short minute; //分钟
    unsigned short second; //秒钟
    unsigned short action; //动作
    unsigned short enable; //开关
    uint8_t repeat;
} TASK_CONFIG;

typedef struct _PLUG_CONFIG_ONE
{
    TASK_CONFIG task[PLUG_TIME_TASK_NUM];
} PLUG_CONFIG_ONE;

typedef struct _PLUG_CONFIG
{
    unsigned int magic_head;
    PLUG_CONFIG_ONE plug[PLUG_NUM];
    unsigned char crc;
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

typedef struct _PLUG_STATUS
{
    unsigned int magic_head;
    unsigned short plug[PLUG_NUM];
    unsigned char crc;
} PLUG_STATUS;

extern HF_CONFIG_FILE g_hf_config_file;

PLUG_STATUS plug_status;
MQTT_CONFIG user_mqtt_config;
PLUG_CONFIG user_plug_config;

/// extern char rtc_init;
char strMac[13];
char strIp[32];
uint32_t power;
char deviceid[32];
char ntpserver[50];
uint8_t version;

void user_config_init(void);
unsigned char crc_calc(unsigned char *, int);
void get_user_config_str(char *, uint8_t);
void get_user_config_simple_str(char *);
void save_user_config(void);

void init_plug_status(void);
void default_plug_status(PLUG_STATUS *);
void save_plug_status(PLUG_STATUS *);

void init_plug_config(void);
void default_plug_config(PLUG_CONFIG *);
void save_plug_config(PLUG_CONFIG *);

bool update_mqtt_config_flag;
bool update_plug_config_flag;
bool update_plug_status_flag[PLUG_NUM];

bool user_plug_config_enable;

uint8_t plug_status_loaded;
uint8_t plug_config_loaded;
uint8_t mqtt_config_loaded;
uint8_t mqtt_is_connected;

// uint8_t press_flag;
// uint8_t release_flag;
// time_t last_press_time;
// time_t last_release_time;
#endif
