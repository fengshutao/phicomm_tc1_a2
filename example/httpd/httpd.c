#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hsf.h>
#include "httpd.h"

hftimer_handle_t heartbeat_timer=NULL; 
hftimer_handle_t scan_ap_timer=NULL; 
hftimer_handle_t reset_timer=NULL; 

web_info_data_t web_config_info;

static char *hfstrnstr(const char *s, const char *find, size_t slen)
{
	int i,flen;
	
	flen = strlen(find);
	if(flen>slen)
		return NULL;
	
	for(i=0;i<=slen-flen;i++)
	{
		if(s[i]==*find)
		{
			if(strncmp(s+i,find,slen-i)==0)
				return (char*)(s+i);
		}
	}
	
	return NULL;
}

void USER_FUNC write_web_para_data_to_flash(void)
{
	hfuflash_erase_page(CONFIG_FLASH_ADDR,1);
    	hfuflash_write(CONFIG_FLASH_ADDR,(char *)&web_config_info,sizeof(web_config_info));
}

int USER_FUNC set_heartbeat_rate(char *data)
{
	web_config_info.heartbeat_rate= atoi(data);
	if(web_config_info.heartbeat_rate <= 0)
		web_config_info.heartbeat_rate = DEFAULT_PARA_HB_RATE;
	printf("set heartbeat rate=%d\r\n",web_config_info.heartbeat_rate);
	return 0;
}
void USER_FUNC set_ap_scan_frequence(int frequence)
{
	if(frequence==0)
	{
		web_config_info.scan_frequence = DEFAULT_PARA_SCAN_RATE;
	}
	else
	{
	        web_config_info.scan_frequence=frequence;
	}
	printf("set scan frequence=%d\r\n",frequence);	
}
void USER_FUNC set_reset_frequence(int frequence)
{
	if((frequence<0) || (frequence>DEFAULT_PARA_RESET_RATE))
	{
		web_config_info.reset_frequence = DEFAULT_PARA_RESET_RATE;
	}
	else
	{
	        web_config_info.reset_frequence=frequence;
	}
	u_printf("set reset frequence=%d\r\n",web_config_info.reset_frequence);	
}

int  hfhttpd_yz_nvset(char *cfg_name,int name_len, char *value, int value_len)
{
	/*webserver set config info*/
	char tem_buf[100] = {0};

	if(hfstrnstr(cfg_name,"CFG_HEARTBEAT",name_len)!=NULL)
	{
		if(value_len == 0)
			return;
		bzero(tem_buf,sizeof(tem_buf));
		memcpy(tem_buf,value,value_len);
		set_heartbeat_rate(tem_buf);
	}
	else if(hfstrnstr(cfg_name,"CFG_AP_DATA_SCAN_FREQUENCE",name_len)!=NULL)
	{
		uint16_t temp = 0;
		if(value_len == 0)
			return;
		bzero(tem_buf,sizeof(tem_buf));
		memcpy(tem_buf,value,value_len);
		temp=(uint16_t)atoi(tem_buf);
		set_ap_scan_frequence(temp);
	}
	else if(hfstrnstr(cfg_name,"CFG_RESET_FREQUENCE",name_len)!=NULL)
	{
		uint16_t temp = 0;
		if(value_len == 0)
			return;
		bzero(tem_buf,sizeof(tem_buf));
		memcpy(tem_buf,value,value_len);
		temp=(uint16_t)atoi(tem_buf);
		set_reset_frequence(temp);
	}
	write_web_para_data_to_flash();
}


int hfhttpd_yz_nvget(char *cfg_name,int name_len, char *value, int value_len)
{
	/*webserver get config info*/
	char temp_buffer[32]={0};

	if(hfstrnstr(cfg_name,"CFG_HEARTBEAT",name_len)!=NULL)
	{
		bzero(temp_buffer,sizeof(temp_buffer));
		memcpy(temp_buffer,web_config_info.heartbeat_rate,sizeof(web_config_info.heartbeat_rate));
		*value = temp_buffer;
		value_len = strlen(temp_buffer);
	}
	else if(hfstrnstr(cfg_name,"CFG_AP_DATA_SCAN_FREQUENCE",name_len)!=NULL)
	{
		bzero(temp_buffer,sizeof(temp_buffer));
		memcpy(temp_buffer,web_config_info.scan_frequence,sizeof(web_config_info.scan_frequence));
		*value = temp_buffer;
		value_len = strlen(temp_buffer);
	}
	else if(hfstrnstr(cfg_name,"CFG_RESET_FREQUENCE",name_len)!=NULL)
	{
		bzero(temp_buffer,sizeof(temp_buffer));
		memcpy(temp_buffer,web_config_info.reset_frequence,sizeof(web_config_info.reset_frequence));
		*value = temp_buffer;
		value_len = strlen(temp_buffer);
	}
	else
	{
		value = NULL;
		value_len = 0;
	}
}

void USER_FUNC heart_beat_timer_callback( hftimer_handle_t htimer ) 
{
	if(HEART_BEAT_TIMER_ID == hftimer_get_timer_id(htimer))
	{
		printf("\r\n----------heart_beat_timer_callback---------\r\n");
	}
}
void USER_FUNC create_heartBeat_timer(void)
{
	heartbeat_timer = hftimer_create("HEART BEAT TIMER",web_config_info.heartbeat_rate*1000,true,HEART_BEAT_TIMER_ID,heart_beat_timer_callback,0);
	if(NULL != heartbeat_timer) 
	{
		u_printf("heartbeat_timer_crteate ok:%ds\r\n",web_config_info.heartbeat_rate);
		hftimer_start(heartbeat_timer);
	}
	else
	{
		u_printf("create timer fail:%ds\n",web_config_info.heartbeat_rate); 
	}
}

#if 1
static int hfwifi_scan_process( PWIFI_SCAN_RESULT_ITEM scan_ret) 
{ 
	printf("%s,%d,%d,%d!\r\n",scan_ret->ssid,scan_ret->channel,scan_ret->encry,scan_ret->rssi_dbm); 
	return 0; 
}

void USER_FUNC scan_ap_timer_callback( hftimer_handle_t htimer ) 
{
	if(WIFI_SCAN_AP_TIMER_ID == hftimer_get_timer_id(htimer))
	{
		printf("-----scan_ap_timer_callback----\r\n");
            	hfwifi_scan(hfwifi_scan_process);
	}
}

void USER_FUNC create_scan_ap_timer(void)
{	
	scan_ap_timer = hftimer_create("WIFI WSCAN TIMER",web_config_info.scan_frequence*1000,true,WIFI_SCAN_AP_TIMER_ID,scan_ap_timer_callback,0);
	if(NULL!=scan_ap_timer) 
	{
		hftimer_start(scan_ap_timer);
		u_printf("create scan ap timer ok:%d s\n",web_config_info.scan_frequence); 
	}
	else
	{
		u_printf("create scan ap timer fail:%d s\n",web_config_info.scan_frequence); 
	}
}
#endif
void USER_FUNC reset_timer_callback( hftimer_handle_t htimer ) 
{
	if(WIFI_RESET_TIMER_ID == hftimer_get_timer_id(htimer))
	{
		printf("-----reset_timer_callback----\r\n");
            	hfsys_reset();
	}
}

void USER_FUNC create_reset_timer(void)
{	
	reset_timer = hftimer_create("WIFI RESET TIMER",web_config_info.reset_frequence*60*1000,false,WIFI_RESET_TIMER_ID,reset_timer_callback,0);
	if(NULL!=reset_timer) 
	{
		hftimer_start(reset_timer);
		u_printf("create reset timer ok:%d min\n",web_config_info.reset_frequence); 
	}
	else
	{
		u_printf("create reset timer fail:%d min\n",web_config_info.reset_frequence); 
	}
}

void wifi_scan_thread_func(void)
{
	create_heartBeat_timer();
	create_reset_timer();
	//create_scan_ap_timer();
	printf("scan ap timer :%d s\n",web_config_info.scan_frequence);
	while(1)
	{
		msleep(web_config_info.scan_frequence*1000);
		hfwifi_scan(hfwifi_scan_process);
	}
}

void  wifi_module_init(void)
{
	char rsp[32] = {0};
	char at_buf[60] = {0};
	hfuflash_read(CONFIG_FLASH_ADDR,(char *)&web_config_info,sizeof(web_config_info));
	u_printf("------------first_start_flag=%d-----------\r\n",web_config_info.first_start_flag);
	if(web_config_info.first_start_flag!=1)
	{
		hfat_send_cmd("AT+WMODE=AP\r\n",sizeof("AT+WMODE=AP\r\n"),rsp,32);
		memset((char *)&web_config_info, '\0', sizeof(web_config_info));
		web_config_info.first_start_flag = 1;
		web_config_info.heartbeat_rate = DEFAULT_PARA_HB_RATE;
		web_config_info.scan_frequence= DEFAULT_PARA_SCAN_RATE;
		web_config_info.reset_frequence = DEFAULT_PARA_RESET_RATE;

		hfuflash_erase_page(CONFIG_FLASH_ADDR,1);
		hfuflash_write(CONFIG_FLASH_ADDR,(char *)&web_config_info,sizeof(web_config_info));
		msleep(100);
		hfsys_softreset();
	}
}

