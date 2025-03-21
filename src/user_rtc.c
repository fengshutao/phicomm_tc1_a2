
#include "app_main.h"
#include "user_gpio.h"
#include "user_rtc.h"
#include <time.h>
#include <hsf.h>

#include "user_function.h"
#include "user_mqtt.h"

#define TIMER2_ID (2) // timer2 for rtctime

time_t rtctimes = 1640966400; // 当前时间戳, init：2022-01-1 0:0:0

// 临时变量
struct tm *current_time; // 当前时间结构体
time_t utc8ts;           // UTC8 时间戳
time_t systime_last_ntp;
time_t systime_now;
time_t systime_wifi_link = 0;
time_t timestamp_start = 0;
bool ntptime_succeed = false;
char tmp_rsp[32] = {0};

// time_t ntp_res;

void USER_FUNC do_update_rtc_time()
{
    if ((!ntptime_succeed && systime_now - systime_last_ntp > 10000) ||
        (systime_now - systime_last_ntp > 36000000))
    {
        int time_mm = hfntp_get_time(ntpserver, 123, 200);
        // ntp_res = time_mm;
        if (time_mm < 0)
        {
            ntptime_succeed = false;
        }
        else
        {
            timestamp_start = time_mm - systime_now / 1000;
            ntptime_succeed = true;
        }
        systime_last_ntp = systime_now;
    }
}

void USER_FUNC do_update_wifi_time()
{
    if (user_plug_config.enable_auto_restart == 0)
    {
        return;
    }
    // if (systime_now - systime_wifi_link < 29000)
    // {
    //     return;
    // }
    // if (ping(user_mqtt_config.seraddr) == HF_SUCCESS)
    // {
    //     systime_wifi_link = systime_now;
    // }
    if (hfwifi_sta_is_connected())
    {
        systime_wifi_link = systime_now;
    }
    else if (user_plug_config.enable_auto_restart == 1 && systime_now - systime_wifi_link > 600000)
    {
        system_soft_restart_flag = true;
    }
    else if (user_plug_config.enable_auto_restart > 1 && systime_now - systime_wifi_link > 600000)
    {
        system_restart_flag = true;
    }
}

void USER_FUNC do_schedule_tasks()
{
    if (!user_plug_config_enable)
    {
        return;
    }

    update_time();
    bool has_tasks = false;
    for (uint8_t i = 0; i < PLUG_NUM; i++)
    {
        for (uint8_t j = 0; j < PLUG_TIME_TASK_NUM; j++)
        {
            if (user_plug_config.plug[i].task[j].enable != 0)
            {
                if (current_time->tm_sec == user_plug_config.plug[i].task[j].second &&
                    current_time->tm_min == user_plug_config.plug[i].task[j].minute &&
                    current_time->tm_hour == user_plug_config.plug[i].task[j].hour &&
                    ((1 << (current_time->tm_wday + 1)) & user_plug_config.plug[i].task[j].repeat) > 0)
                {
                    if (plug_status.plug[i] != user_plug_config.plug[i].task[j].action)
                    {
                        user_relay_set(i, user_plug_config.plug[i].task[j].action);
                        update_plug_status_flag[i] = true;
                    }
                    if ((user_plug_config.plug[i].task[j].repeat & 1) == 0)
                    {
                        user_plug_config.plug[i].task[j].enable = 0;
                        update_plug_config_flag = true;
                    }
                    else
                        has_tasks = true;
                }
                else
                {
                    has_tasks = true;
                }
            }
        }
    }
    user_plug_config_enable = has_tasks;
}

void USER_FUNC do_update_plug_config()
{
    if (update_plug_config_flag)
    {
        update_plug_config_flag = false;
        save_plug_config(&user_plug_config);
    }
}

void USER_FUNC do_update_plug_status()
{
    bool need_update = false;
    for (uint8_t i = 0; i < PLUG_NUM; i++)
    {
        if (update_plug_status_flag[i])
        {
            need_update = true;
            update_plug_status_flag[i] = false;
            mqtt_report_plug_status(i);
        }
    }
    if (need_update)
    {
        save_plug_status(&plug_status);
    }
    return;
}

void USER_FUNC do_update_mqtt_config()
{
    if (update_mqtt_config_flag)
    {
        update_plug_config_flag = false;
        update_mqtt_config(&user_mqtt_config);
        save_mqtt_config(&user_mqtt_config);
    }
}

void USER_FUNC rtc_thread_func(void *arg)
{
    while (1)
    {
        msleep(100);
        systime_now = hfsys_get_time();
        if (system_restart_flag)
        {
            system_restart_flag = false;
            hfsys_reset();
        }

        if (system_soft_restart_flag)
        {
            system_soft_restart_flag = false;
            hfsys_softreset();
        }

        if (system_start_ap_flag)
        {
            system_start_ap_flag = false;
            hfat_send_cmd("AT+WMODE=AP\r\n",sizeof("AT+WMODE=AP\r\n"),tmp_rsp,32);
            system_soft_restart_flag = true;
        }

        do_update_rtc_time();

        do_update_wifi_time();

        do_schedule_tasks();

        do_update_plug_status();

        do_update_plug_config();

        do_update_mqtt_config();
    }
}

void USER_FUNC rtc_timer_func(void *arg)
{
    systime_now = hfsys_get_time();
    if (system_restart_flag)
    {
        system_restart_flag = false;
        hfsys_reset();
    }

    if (system_soft_restart_flag)
    {
        system_soft_restart_flag = false;
        hfsys_softreset();
    }

    if (system_start_ap_flag)
    {
        system_start_ap_flag = false;
        hfat_send_cmd("AT+WMODE=AP\r\n",sizeof("AT+WMODE=AP\r\n"),tmp_rsp,32);
        system_soft_restart_flag = true;
    }

    do_update_rtc_time();

    do_update_wifi_time();

    do_schedule_tasks();

    do_update_plug_status();

    do_update_plug_config();

    do_update_mqtt_config();
}

// void USER_FUNC rtctime_timeout_handler(void *arg)
// {
//     rtctimes = rtctimes + 1;
// }

void USER_FUNC rtctime_init(void)
{
    systime_last_ntp = hfsys_get_time();
    systime_now = systime_last_ntp;
    hftimer_handle_t user_rtc_timer = hftimer_create("TIMER2", 500, 1, TIMER2_ID, rtc_timer_func, 0); //每一秒加一,模拟时钟在跑
    // hfthread_create(rtc_thread_func, "rtccontrol", 1024, NULL, HFTHREAD_PRIORITIES_LOW, NULL, NULL);
    // hftimer_handle_t user_rtc_timer = hftimer_create("TIMER2", 1000, 1, TIMER2_ID, rtctime_timeout_handler, 0); //每一秒加一,模拟时钟在跑
    hftimer_start(user_rtc_timer);
}

void USER_FUNC update_time()
{
    utc8ts = timestamp_start + systime_now / 1000 + UTC8;
    current_time = gmtime(&utc8ts);
}

void USER_FUNC get_time_string(char *res, int len)
{
    update_time();
    strftime(res, len, "%Y-%m-%d %H:%M:%S", current_time);
    return;
}
