
#include "user_function.h"
#include "app_main.h"
#include "user_gpio.h"
//#include "user_ota.h"
#include "user_mqtt.h"
#include "user_udp.h"
#include <string.h>
uint32_t last_time = 0;

void user_function_set_last_time()
{
    // last_time = UpTicks( );
}

void user_send(int udp_flag, char *s)
{
    // if ( udp_flag || !user_mqtt_isconnect( ) )
    //  user_udp_send( s ); //发送数据
    // else
    // user_mqtt_send( s );
}

void USER_FUNC user_function_cmd_received(char *pusrdata, int datalen, char *rsp)
{
    if (datalen >= USER_BUFF_SIZE)
    {
        sprintf(rsp, "max datalen: 1024");
        return;
    }

    hfthread_mutext_lock(user_buff_lock);
    memset(user_buff, 0, USER_BUFF_SIZE);
    sprintf(user_buff, "%.*s", datalen, pusrdata);
    user_mqtt_publish(user_buff);
    cJSON *pJsonRoot = cJSON_Parse(user_buff);
    if (!pJsonRoot)
    {
        sprintf(rsp, "json invalid");
        hfthread_mutext_free(user_buff_lock);
        return;
    }

    cJSON *p_cmd = cJSON_GetObjectItem(pJsonRoot, "cmd");
    if (p_cmd && cJSON_IsString(p_cmd))
    {
        user_mqtt_publish(p_cmd->valuestring);
        if (strcmp(p_cmd->valuestring, "restart") == 0)
        {
            user_mqtt_publish("restart");
            hfsys_reset();
        }
        else if (strcmp(p_cmd->valuestring, "device_report") == 0)
        {
            user_mqtt_publish("devide_report");
            get_user_config_simple_str(user_buff);
            user_mqtt_publish(user_buff);
        }
        // plug config cmd
        else if (strcmp(p_cmd->valuestring, "init_plug_config") == 0)
        {
            user_mqtt_publish("init_plug_config");
            init_plug_config();
        }
        else if (strcmp(p_cmd->valuestring, "default_plug_config") == 0)
        {
            user_mqtt_publish("default_plug_config");
            default_plug_config(&user_plug_config);
        }
        else if (strcmp(p_cmd->valuestring, "save_plug_config") == 0)
        {
            user_mqtt_publish("save_plug_config");
            save_plug_config(&user_plug_config);
        }
        // plug status cmd
        else if (strcmp(p_cmd->valuestring, "init_plug_status") == 0)
        {
            user_mqtt_publish("init_plug_status");
            init_plug_status();
        }
        else if (strcmp(p_cmd->valuestring, "default_plug_status") == 0)
        {
            user_mqtt_publish("default_plug_status");
            default_plug_status(&plug_status);
        }
        else if (strcmp(p_cmd->valuestring, "save_plug_status") == 0)
        {
            user_mqtt_publish("save_plug_status");
            save_plug_status(&plug_status);
        }
    }

    //开始正式处理所有命令
    {
        //解析主机setting-----------------------------------------------------------------
        cJSON *p_setting = cJSON_GetObjectItem(pJsonRoot, "setting");
        if (p_setting)
        {
            cJSON *p_setting_name = cJSON_GetObjectItem(p_setting, "name");
            if (p_setting_name && cJSON_IsString(p_setting_name))
            {
                update_mqtt_config_flag = true;
                sprintf(user_mqtt_config.clientid, "%s", p_setting_name->valuestring);
            }

            cJSON *p_mqtt_hass = cJSON_GetObjectItem(p_setting, "hass_topic");
            if (p_mqtt_hass && cJSON_IsString(p_mqtt_hass))
            {
                update_mqtt_config_flag = true;
                sprintf(user_mqtt_config.hass_topic, "%s", p_mqtt_hass->valuestring);
            }

            //设置mqtt ip
            cJSON *p_mqtt_ip = cJSON_GetObjectItem(p_setting, "mqtt_uri");
            if (p_mqtt_ip && cJSON_IsString(p_mqtt_ip))
            {
                update_mqtt_config_flag = true;
                sprintf(user_mqtt_config.seraddr, p_mqtt_ip->valuestring);
            }

            //设置mqtt port
            cJSON *p_mqtt_port = cJSON_GetObjectItem(p_setting, "mqtt_port");
            if (p_mqtt_port && cJSON_IsNumber(p_mqtt_port))
            {
                update_mqtt_config_flag = true;
                user_mqtt_config.port = p_mqtt_port->valueint;
            }

            //设置mqtt user
            cJSON *p_mqtt_user = cJSON_GetObjectItem(p_setting, "mqtt_user");
            if (p_mqtt_user && cJSON_IsString(p_mqtt_user))
            {
                update_mqtt_config_flag = true;
                sprintf(user_mqtt_config.username, p_mqtt_user->valuestring);
            }

            //设置mqtt password
            cJSON *p_mqtt_password = cJSON_GetObjectItem(p_setting, "mqtt_password");
            if (p_mqtt_password && cJSON_IsString(p_mqtt_password))
            {
                update_mqtt_config_flag = true;
                sprintf(user_mqtt_config.password, p_mqtt_password->valuestring);
            }
        }

        //解析plug-----------------------------------------------------------------
        for (uint8_t i = 0; i < PLUG_NUM; i++)
        {
            if (json_plug_analysis(i, pJsonRoot))
                update_plug_config_flag = true;
                user_plug_config_enable = true;
        }
    }

    cJSON_Delete(pJsonRoot);
    hfthread_mutext_free(user_buff_lock);
    sprintf(rsp, "config changed");
    return;
}

/*
 *解析处理定时任务json
 *udp_flag:发送udp/mqtt标志位,此处修改插座开关状态时,需要实时更新给domoticz
 *x:插座编号
 */
bool json_plug_analysis(unsigned char x, cJSON *pJsonRoot)
{
    if (!pJsonRoot)
        return false;
    char i;
    bool return_flag = false;
    char plug_str[] = "plug_X";
    plug_str[5] = x + '0';

    cJSON *p_plug = cJSON_GetObjectItem(pJsonRoot, plug_str);
    if (!p_plug)
        return false;

    {
        cJSON *p_plug_on = cJSON_GetObjectItem(p_plug, "status");
        if (p_plug_on)
        {
            if (cJSON_IsNumber(p_plug_on))
            {
                user_relay_set(x, p_plug_on->valueint);
            }
        }

        //解析plug中setting项目----------------------------------------------
        cJSON *p_plug_setting = cJSON_GetObjectItem(p_plug, "setting");
        if (p_plug_setting)
        {
            //解析plug中setting中task----------------------------------------
            for (i = 0; i < PLUG_TIME_TASK_NUM; i++)
            {
                if (json_plug_task_analysis(x, i, p_plug_setting))
                    return_flag = true;
            }
        }
    }
    return return_flag;
}

/*
 *解析处理定时任务json
 *x:插座编号 y:任务编号
 */
bool json_plug_task_analysis(unsigned char x, unsigned char y, cJSON *pJsonRoot)
{
    if (!pJsonRoot)
        return false;
    bool return_flag = false;

    char plug_task_str[] = "task_X";
    plug_task_str[5] = y + '0';

    cJSON *p_plug_task = cJSON_GetObjectItem(pJsonRoot, plug_task_str);
    if (!p_plug_task)
        return false;

    cJSON *p_plug_task_hour = cJSON_GetObjectItem(p_plug_task, "hour");
    cJSON *p_plug_task_minute = cJSON_GetObjectItem(p_plug_task, "minute");
    cJSON *p_plug_task_second = cJSON_GetObjectItem(p_plug_task, "second");
    cJSON *p_plug_task_repeat = cJSON_GetObjectItem(p_plug_task, "repeat");
    cJSON *p_plug_task_action = cJSON_GetObjectItem(p_plug_task, "action");
    cJSON *p_plug_task_enable = cJSON_GetObjectItem(p_plug_task, "enable");

    if (p_plug_task_hour && p_plug_task_minute && p_plug_task_repeat &&
        p_plug_task_action && p_plug_task_enable)
    {

        if (cJSON_IsNumber(p_plug_task_hour) && cJSON_IsNumber(p_plug_task_minute) && cJSON_IsNumber(p_plug_task_second) && cJSON_IsNumber(p_plug_task_repeat) && cJSON_IsNumber(p_plug_task_action) && cJSON_IsNumber(p_plug_task_enable))
        {
            return_flag = true;
            user_plug_config.plug[x].task[y].hour = p_plug_task_hour->valueint;
            user_plug_config.plug[x].task[y].minute = p_plug_task_minute->valueint;
            user_plug_config.plug[x].task[y].second = p_plug_task_second->valueint;
            user_plug_config.plug[x].task[y].repeat = p_plug_task_repeat->valueint;
            user_plug_config.plug[x].task[y].action = p_plug_task_action->valueint;
            user_plug_config.plug[x].task[y].enable = p_plug_task_enable->valueint;
        }
    }
    return return_flag;
}

char *strrpc(char *str, char *oldstr, char *newstr)
{

    char bstr[strlen(str)]; //转换缓冲区

    memset(bstr, 0, sizeof(bstr));

    for (int i = 0; i < strlen(str); i++)
    {

        if (!strncmp(str + i, oldstr, strlen(oldstr)))
        { //查找目标字符串

            strcat(bstr, newstr);

            i += strlen(oldstr) - 1;
        }
        else
        {

            strncat(bstr, str + i, 1); //保存一字节进缓冲区
        }
    }

    strcpy(str, bstr);

    return str;
}

