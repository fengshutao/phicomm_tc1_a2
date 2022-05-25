
#include "app_main.h"
#include "user_gpio.h"
#include "user_rtc.h"
#include <time.h>
#include <hsf.h>

#include "user_function.h"

#define TIMER2_ID (2) // timer2 for rtctime

time_t rtctimes = 1640966400; // 当前时间戳, init：2022-01-1 0:0:0

// 临时变量
struct tm *current_time; // 当前时间结构体
time_t utc8ts;           // UTC8 时间戳
time_t last_ntp_time = 0;
bool ntptime_succeed = false;

void USER_FUNC rtc_thread_func(void *arg)
{
    int i, j;

    while (1)
    {
        if ((!ntptime_succeed && rtctimes - last_ntp_time > 10) || (ntptime_succeed && rtctimes - last_ntp_time > 3600))
        {
            update_rtctime();
            last_ntp_time = rtctimes;
        }

        update_time();

        // 处理定时任务
        for (i = 0; i < PLUG_NUM; i++)
        {
            for (j = 0; j < PLUG_TIME_TASK_NUM; j++)
            {
                if (user_plug_config.plug[i].task[j].enable != 0)
                {
                    uint8_t *repeat = user_plug_config.plug[i].task[j].repeat;
                    if (current_time->tm_sec == user_plug_config.plug[i].task[j].minute &&
                        current_time->tm_min == user_plug_config.plug[i].task[j].minute &&
                        current_time->tm_hour == user_plug_config.plug[i].task[j].hour &&
                        repeat[current_time->tm_wday + 1] != 0)
                    {
                        if (plug_status.plug[i] != user_plug_config.plug[i].task[j].action)
                        {
                            user_relay_set(i, user_plug_config.plug[i].task[j].action);
                            update_plug_status_flag = true;
                        }
                        if (repeat[0] == 0)
                        {
                            user_plug_config.plug[i].task[j].enable = 0;
                            update_plug_config_flag = true;
                        }
                    }
                }
            }
        }

    }
    u_printf("rtc_thread_func exit..");
}

void USER_FUNC rtctime_timeout_handler(void *arg)
{
    rtctimes = rtctimes + 1;
}

void USER_FUNC rtctime_init(void)
{
    hfthread_create(rtc_thread_func, "rtccontrol", 1024, (void *)1, 1, NULL, NULL);
    hftimer_handle_t user_rtc_timer = hftimer_create("TIMER2", 1000, 1, TIMER2_ID, rtctime_timeout_handler, 0); //每一秒加一,模拟时钟在跑
    hftimer_start(user_rtc_timer);
}

void update_rtctime(void)
{
    int time_mm = hfntp_get_time(ntpserver, 123, 200);
    if (time_mm < 0)
    {
        ntptime_succeed = false;
    }
    else
    {
        rtctimes = time_mm;
        ntptime_succeed = true;
    }
}

int printtime()
{
    time_t t;

    t = rtctimes + 8 * 3600;   //东八区
    current_time = gmtime(&t); //时间结构体
    u_printf("hour:%d,min:%d\n", current_time->tm_hour, current_time->tm_min);
    char s[30];
    strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", current_time);
    u_printf("%d: %s\n", (int)t, s);

    return 1;
}

void USER_FUNC update_time()
{
    utc8ts = rtctimes + UTC8;
    current_time = gmtime(&utc8ts);
}

void get_time_string(char *res, int len)
{
    update_time();
    strftime(res, len, "%Y-%m-%d %H:%M:%S", current_time);
    return;
}

void get_user_config_info(cJSON *json_send)
{
    int i, j;
    char task_flag[PLUG_NUM] = {-1, -1, -1, -1, -1};
    cJSON_AddStringToObject(json_send, "mac", strMac);

    for (i = 0; i < PLUG_NUM; i++)
    {
        char strTemp1[] = "plug_X";
        strTemp1[5] = i + '0';
        cJSON *json_send_plug = cJSON_CreateObject();
        cJSON_AddNumberToObject(json_send_plug, "on", plug_status.plug[i]);

        if (task_flag[i] >= 0)
        {
            cJSON *json_send_plug_setting = cJSON_CreateObject();

            j = task_flag[i];
            char strTemp2[] = "task_X";
            strTemp2[5] = j + '0';
            // u_printf("[strTemp2] task_X=>%s\n",strTemp2);
            cJSON *json_send_plug_task = cJSON_CreateObject();
            cJSON_AddNumberToObject(json_send_plug_task, "hour", user_plug_config.plug[i].task[j].hour);
            cJSON_AddNumberToObject(json_send_plug_task, "minute", user_plug_config.plug[i].task[j].minute);
            // cJSON_AddNumberToObject(json_send_plug_task, "repeat", user_plug_config.plug[i].task[j].repeat);
            cJSON_AddNumberToObject(json_send_plug_task, "action", user_plug_config.plug[i].task[j].action);
            cJSON_AddNumberToObject(json_send_plug_task, "on", user_plug_config.plug[i].task[j].enable);
            cJSON_AddItemToObject(json_send_plug_setting, strTemp2, json_send_plug_task);
            cJSON_AddItemToObject(json_send_plug, "setting", json_send_plug_setting);

            task_flag[i] = -1;
        }
        cJSON_AddItemToObject(json_send, strTemp1, json_send_plug);
    }
}
