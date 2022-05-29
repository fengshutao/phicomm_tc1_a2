
#include "app_main.h"
#include "user_gpio.h"
#include "user_rtc.h"
#include "user_mqtt.h"
#include "user_udp.h"
#include "user_config.h"
#include <string.h>

// 按键的回调函数里不能发送 mqtt, 不然会重启

#define TIMER1_ID (1) //定时器id

hfthread_mutex_t keythread_lock = NULL_MUTEX;
uint8_t key_time = 0;
// time_t key_press_time = 0;
#define BUTTON_LONG_PRESS_TIME 10 //       //时间

int relay[] = {SW1, SW2, SW3, SW4, SW5, SW6}; //对应每路继电器GPIO

void USER_FUNC user_led_set(char x)
{
    if (x == -1)
    {
        if (hfgpio_fpin_is_high(LED))
        {
            hfgpio_fset_out_low(LED);
        }
        else
            hfgpio_fset_out_high(LED);
    }
    else if (x)
        hfgpio_fset_out_high(LED);
    else
        hfgpio_fset_out_low(LED);
}

bool USER_FUNC relay_out(void)
{
    uint8_t i;
    for (i = 0; i < PLUG_NUM; i++)
    {
        if (plug_status.plug[i] != 0)
        {
            return true;
        }
    }
    return false;
}

/*user_relay_set
 * 设置继电器开关
 * x:编号 0-5
 * y:开关 0:关 1:开
 */
void USER_FUNC user_relay_set(uint8_t x, uint8_t y)
{

    if (x >= PLUG_NUM)
        return;

    if (y == 1)
        hfgpio_fset_out_high(relay[x]);
    else
        hfgpio_fset_out_low(relay[x]);

    plug_status.plug[x] = y;
    update_plug_status_flag[x] = true;
    if (relay_out())
        user_led_set(1);
    else
        user_led_set(0);
}

uint8_t USER_FUNC key_status(void)
{
    if (hfgpio_fpin_is_high(KEY))
    {
        return KEY_UP;
    }
    else
    {
        return KEY_DOWN;
    }
}

/*
 * 设置所有继电器开关
 * y:0:全部关   1:根据记录状态开关所有
 *
 */
void USER_FUNC user_relay_set_all(uint8_t y)
{
    uint8_t i;
    for (i = 0; i < PLUG_NUM; i++)
        user_relay_set(i, y);
}

void USER_FUNC key_long_press(void)
{
    // user_mqtt_publish("long_press");
    // mqtt_report_status();
}

void USER_FUNC key_short_press(void)
{
    if (relay_out())
    {
        user_relay_set_all(0);
    }
    else
    {
        user_relay_set_all(1);
    }
}

void USER_FUNC key_timeout_handler(void *arg)
{
    // any button pressed
    if (key_status() == KEY_DOWN)
    {
        // 100ms 每次
        key_time++;

        if (key_time == 30)
        {
            key_long_press();
        }
        // else if (key_time == 35)
        // {
        //     user_led_set(0);
        // }
        // else if (key_time == 40)
        // {
        //     user_led_set(1);
        // }
        // else if (key_time == 45)
        // {
        //     user_led_set(0);
        // }
        // else if (key_time == 50)
        // {
        //     user_led_set(1);
        // }
        // else if (key_time == 55)
        // {
        //     user_led_set(0);
        // }
        // else if (key_time == 60)
        // {
        //     user_led_set(1);
        // }
        // else if (key_time == 65)
        // {
        //     user_led_set(0);
        // }
        else if (key_time == 70)
        {
            // user_function_cmd_received(1, "{\"cmd\":\"device report\"}");
            user_led_set(0);
        }
        else if (key_time == 75)
        {
            user_led_set(1);
        }
        else if (key_time == 80)
        {
            user_led_set(0);
            // user_mqtt_publish("smartlink start");
            // user_function_cmd_received(1, "{\"cmd\":\"device report\"}");
            hfsmtlk_start();
        }

        if (key_time > 80)
        {
            key_time = 74;
        }
    }
    else
    {
        if (!relay_out())
        {
            user_led_set(0);
        }
    }
}

void USER_FUNC key_rising_irq_handler(uint32_t arg1, uint32_t arg2)
{
    if (key_time < 2)
    {
    }
    else if (key_time < 10)
    {
        if (key_status() == KEY_UP)
        {
            key_short_press();
            // release_flag = 1;
            // last_release_time = hfsys_get_time();
            // // user_mqtt_publish("key_release");
        }
    }

    key_time = 0;
}

void USER_FUNC key_init()
{
    if (hfthread_mutext_new(&keythread_lock) != 0)
    {
        HF_Debug(DEBUG_LEVEL, "create mutex fail\n");
        return;
    }
    hftimer_handle_t user_key_timer = hftimer_create("TIMER1", 100, 1, TIMER1_ID, key_timeout_handler, 0);
    hfgpio_configure_fpin_interrupt(KEY, HFPIO_IT_EDGE, key_rising_irq_handler, 1);
    hftimer_start(user_key_timer);
}
