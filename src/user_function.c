
#include "app_main.h"
#include "user_gpio.h"
#include "cJSON/cJSON.h"
//#include "user_ota.h"
//#include "user_mqtt_client.h"
#include "user_udp.h"
#include <string.h>
uint32_t last_time = 0;

void user_function_set_last_time( )
{
    //last_time = UpTicks( );
}



bool json_plug_analysis( int udp_flag, unsigned char x, cJSON * pJsonRoot, cJSON * pJsonSend );
bool json_plug_task_analysis(unsigned char x, unsigned char y, cJSON * pJsonRoot, cJSON * pJsonSend );

void user_send( int udp_flag, char *s )
{
    //if ( udp_flag || !user_mqtt_isconnect( ) )
        user_udp_send( s ); //发送数据
   // else
        //user_mqtt_send( s );
}

void USER_FUNC user_function_cmd_received( int udp_flag,  char *pusrdata )
{

    unsigned char i;
    bool update_user_config_flag = false;   //标志位,记录最后是否需要更新储存的数据
    bool return_flag = true;    //为true时返回json结果,否则不返回

    cJSON * pJsonRoot = cJSON_Parse( pusrdata );
    if ( !pJsonRoot )
    {
        u_printf( "this is not a json data:\r\n%s\r\n", pusrdata );
        return;
    } else {
        u_printf( "this is a json data:\r\n%s\r\n", pusrdata );
    }

    //解析UDP命令device report(MQTT同样回复命令)
    cJSON *p_cmd = cJSON_GetObjectItem( pJsonRoot, "cmd" );
    if ( p_cmd && cJSON_IsString( p_cmd ) && strcmp( p_cmd->valuestring, "device report" ) == 0 )
    {
        cJSON *pRoot = cJSON_CreateObject( );
        cJSON_AddStringToObject( pRoot, "name", deviceid );
        cJSON_AddStringToObject( pRoot, "mac", strMac );
        cJSON_AddNumberToObject( pRoot, "type", TYPE );
        cJSON_AddStringToObject( pRoot, "type_name", TYPE_NAME );
        cJSON_AddStringToObject( pRoot, "ip", strIp );

        char *s = cJSON_Print( pRoot );

        user_send( udp_flag, s ); //发送数据
        u_printf( "udp_flag:%d, send json pRoot: %s\r\n",udp_flag, s );
        hfmem_free( (void *) s );
        cJSON_Delete( pRoot );
        //          cJSON_Delete(p_cmd);
    }

    //以下为解析命令部分
    cJSON *p_name = cJSON_GetObjectItem( pJsonRoot, "name" );
    cJSON *p_mac = cJSON_GetObjectItem( pJsonRoot, "mac" );

    //开始正式处理所有命令
    if ( (p_name && cJSON_IsString( p_name ) )    //name
         || (p_mac && cJSON_IsString( p_mac ) && strcmp( p_mac->valuestring, strMac ) == 0)   //mac
         )
    {
        cJSON *json_send = cJSON_CreateObject( );
        cJSON_AddStringToObject( json_send, "mac", strMac );

        //解析版本
        cJSON *p_version = cJSON_GetObjectItem( pJsonRoot, "version" );
        if ( p_version )
        {
            u_printf("version:%s",VERSION);
            cJSON_AddStringToObject( json_send, "version", VERSION );
        }
        //解析主机setting-----------------------------------------------------------------
        cJSON *p_setting = cJSON_GetObjectItem( pJsonRoot, "setting" );
        if ( p_setting )
        {
            //解析ota
            cJSON *p_ota = cJSON_GetObjectItem( p_setting, "ota" );
            if ( p_ota )
            {
                if ( cJSON_IsString( p_ota ) ){}
                    //user_ota_start( p_ota->valuestring, NULL );
            }

            cJSON *json_setting_send = cJSON_CreateObject( );
            //设置设备名称/deviceid
            cJSON *p_setting_name = cJSON_GetObjectItem( p_setting, "name" );
            if ( p_setting_name && cJSON_IsString( p_setting_name ) )
            {
                update_user_config_flag = true;
                //u_printf("user_config.plug.name:%s\n",user_config.plug[1].name);
                sprintf( deviceid, p_setting_name->valuestring );
                //u_printf("p_setting_name->valuestring:%s\n",deviceid);
            }

            //设置mqtt ip
            cJSON *p_mqtt_ip = cJSON_GetObjectItem( p_setting, "mqtt_uri" );
            if ( p_mqtt_ip && cJSON_IsString( p_mqtt_ip ) )
            {
                update_user_config_flag = true;
                sprintf( user_config.mqtt_config->seraddr, p_mqtt_ip->valuestring );
            }

            //设置mqtt port
            cJSON *p_mqtt_port = cJSON_GetObjectItem( p_setting, "mqtt_port" );
            if ( p_mqtt_port && cJSON_IsNumber( p_mqtt_port ) )
            {
                update_user_config_flag = true;
                user_config.mqtt_config->port = p_mqtt_port->valueint;
            }

            //设置mqtt user
            cJSON *p_mqtt_user = cJSON_GetObjectItem( p_setting, "mqtt_user" );
            if ( p_mqtt_user && cJSON_IsString( p_mqtt_user ) )
            {
                update_user_config_flag = true;
                sprintf( user_config.mqtt_config->username, p_mqtt_user->valuestring );
            }

            //设置mqtt password
            cJSON *p_mqtt_password = cJSON_GetObjectItem( p_setting, "mqtt_password" );
            if ( p_mqtt_password && cJSON_IsString( p_mqtt_password ) )
            {
                update_user_config_flag = true;
                sprintf( user_config.mqtt_config->password, p_mqtt_password->valuestring );
            }


            //开发返回数据
            //返回设备ota
            if ( p_ota ) cJSON_AddStringToObject( json_setting_send, "ota", p_ota->valuestring );

            //返回设备名称/deviceid
            if ( p_setting_name ) cJSON_AddStringToObject( json_setting_send, "name", deviceid);
            //返回mqtt ip
            if ( p_mqtt_ip ) cJSON_AddStringToObject( json_setting_send, "mqtt_uri", user_config.mqtt_config->seraddr );
            //返回mqtt port
            if ( p_mqtt_port ) cJSON_AddNumberToObject( json_setting_send, "mqtt_port", user_config.mqtt_config->port );
            //返回mqtt user
            if ( p_mqtt_user ) cJSON_AddStringToObject( json_setting_send, "mqtt_user", user_config.mqtt_config->username );
            //返回mqtt password
            if ( p_mqtt_password ) cJSON_AddStringToObject( json_setting_send, "mqtt_password", user_config.mqtt_config->password );

            cJSON_AddItemToObject( json_send, "setting", json_setting_send );
        }

        //解析plug-----------------------------------------------------------------
        for ( i = 0; i < PLUG_NUM; i++ )
        {
            if ( json_plug_analysis( udp_flag, i, pJsonRoot, json_send ) )
                update_user_config_flag = true;
        }

        cJSON_AddStringToObject( json_send, "name", deviceid );


        if ( return_flag == true )
        {
            char *json_str = cJSON_Print( json_send );
            //u_printf( "[user_function_cmd_received] pRoot: %s\r\n", json_str );
            user_send( udp_flag, json_str ); //发送数据
            hfmem_free( (void *) json_str );
        }
        cJSON_Delete( json_send );
    }

    if ( update_user_config_flag )
    {
        //mico_system_context_update( sys_config );
        update_user_config_flag = false;
    }
    
    cJSON_Delete( pJsonRoot );

}

/*
 *解析处理定时任务json
 *udp_flag:发送udp/mqtt标志位,此处修改插座开关状态时,需要实时更新给domoticz
 *x:插座编号
 */
bool json_plug_analysis( int udp_flag, unsigned char x, cJSON * pJsonRoot, cJSON * pJsonSend )
{
    if ( !pJsonRoot ) return false;
    if ( !pJsonSend ) return false;
    char i;
    bool return_flag = false;
    char plug_str[] = "plug_X";
    plug_str[5] = x + '0';
     
    cJSON *p_plug = cJSON_GetObjectItem( pJsonRoot, plug_str );
    if ( !p_plug ) return_flag = false;

    cJSON *json_plug_send = cJSON_CreateObject( );
    //u_printf("plug_str: %s\n",plug_str);
    //解析plug on------------------------------------------------------
    if ( p_plug )
    {
        cJSON *p_plug_on = cJSON_GetObjectItem( p_plug, "on" );
        if ( p_plug_on )
        {
            if ( cJSON_IsNumber( p_plug_on ) )
            {
                user_relay_set( x, p_plug_on->valueint );
                                u_printf("[json_plug_analysis]  p_plug_on->valueint:%d",p_plug_on->valueint);
                return_flag = true;
            }
            //user_mqtt_send_plug_state(x);
        }

        //解析plug中setting项目----------------------------------------------
        cJSON *p_plug_setting = cJSON_GetObjectItem( p_plug, "setting" );
        if ( p_plug_setting )
        {
            cJSON *json_plug_setting_send = cJSON_CreateObject( );
            //解析plug中setting中name----------------------------------------
            cJSON *p_plug_setting_name = cJSON_GetObjectItem( p_plug_setting, "name" );
            if ( p_plug_setting_name )
            {
                if ( cJSON_IsString( p_plug_setting_name ) )
                {
                    return_flag = true;
                                        u_printf("user_config.plug[%s].name；%s\n",x,user_config.plug[x].name);
                                        sprintf( user_config.plug[x].name, p_plug_setting_name->valuestring );
                    //user_mqtt_hass_auto(x);
                }
                cJSON_AddStringToObject( json_plug_setting_send, "name", user_config.plug[x].name );
            }

            //解析plug中setting中task----------------------------------------
            for ( i = 0; i < PLUG_TIME_TASK_NUM; i++ )
            {
                if ( json_plug_task_analysis( x, i, p_plug_setting, json_plug_setting_send ) )
                    return_flag = true;
            }

            cJSON_AddItemToObject( json_plug_send, "setting", json_plug_setting_send );
        }
    }
   // cJSON *p_nvalue = cJSON_GetObjectItem( pJsonRoot, "nvalue" );
   // if ( p_plug || p_nvalue )
    cJSON_AddNumberToObject( json_plug_send, "on", plug_status[x] );

    cJSON_AddItemToObject( pJsonSend, plug_str, json_plug_send );
    return return_flag;
}

/*
 *解析处理定时任务json
 *x:插座编号 y:任务编号
 */
bool json_plug_task_analysis( unsigned char x, unsigned char y, cJSON * pJsonRoot, cJSON * pJsonSend )
{
    if ( !pJsonRoot ) return false;
    bool return_flag = false;

    char plug_task_str[] = "task_X";
    plug_task_str[5] = y + '0';
        
    cJSON *p_plug_task = cJSON_GetObjectItem( pJsonRoot, plug_task_str );
    if ( !p_plug_task ) return false;
      
    cJSON *json_plug_task_send = cJSON_CreateObject( );

    cJSON *p_plug_task_hour = cJSON_GetObjectItem( p_plug_task, "hour" );
    cJSON *p_plug_task_minute = cJSON_GetObjectItem( p_plug_task, "minute" );
    // cJSON *p_plug_task_repeat = cJSON_GetObjectItem( p_plug_task, "repeat" );
    cJSON *p_plug_task_action = cJSON_GetObjectItem( p_plug_task, "action" );
    cJSON *p_plug_task_on = cJSON_GetObjectItem( p_plug_task, "on" );

    if ( p_plug_task_hour && p_plug_task_minute && //p_plug_task_repeat &&
         p_plug_task_action
         && p_plug_task_on )
    {

        if ( cJSON_IsNumber( p_plug_task_hour )
             && cJSON_IsNumber( p_plug_task_minute )
            //  && cJSON_IsNumber( p_plug_task_repeat )
             && cJSON_IsNumber( p_plug_task_action )
             && cJSON_IsNumber( p_plug_task_on )
                                )
        {
            return_flag = true;
            user_config.plug[x].task[y].hour = p_plug_task_hour->valueint;
            user_config.plug[x].task[y].minute = p_plug_task_minute->valueint;
            // user_config.plug[x].task[y].repeat = p_plug_task_repeat->valueint;
            user_config.plug[x].task[y].action = p_plug_task_action->valueint;
            user_config.plug[x].task[y].enable = p_plug_task_on->valueint;
        }

    }
    cJSON_AddNumberToObject( json_plug_task_send, "hour", user_config.plug[x].task[y].hour );
    cJSON_AddNumberToObject( json_plug_task_send, "minute", user_config.plug[x].task[y].minute );
    // cJSON_AddNumberToObject( json_plug_task_send, "repeat", user_config.plug[x].task[y].repeat );
    cJSON_AddNumberToObject( json_plug_task_send, "action", user_config.plug[x].task[y].action );
    cJSON_AddNumberToObject( json_plug_task_send, "on", user_config.plug[x].task[y].enable );

    cJSON_AddItemToObject( pJsonSend, plug_task_str, json_plug_task_send );
        u_printf("plug task set done.  plug_task_str:%s\n",plug_task_str);
    return return_flag;
}

unsigned char strtohex( char a, char b )
{
    if ( a >= 0x30 && a <= 0x39 )
        a -= 0x30;
    else if ( a >= 0x41 && a <= 0x46 )
    {
        a = a + 10 - 0x41;
    } else if ( a >= 0x61 && a <= 0x66 )
    {
        a = a + 10 - 0x61;
    }

    if ( b >= 0x30 && b <= 0x39 )
        b -= 0x30;
    else if ( b >= 0x41 && b <= 0x46 )
    {
        b = b + 10 - 0x41;
    } else if ( b >= 0x61 && b <= 0x66 )
    {
        b = b + 10 - 0x61;
    }

    return a * 16 + b;
}