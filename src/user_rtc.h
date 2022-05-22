
#ifndef __USER_RTC_H_
#define __USER_RTC_H_

#include "cJSON/cJSON.h"

#define UTC8 28800
void USER_FUNC rtctime_init(void);
void USER_FUNC key_timeout_handler( void* arg );
	
USER_FUNC void rtc_thread_func(void * arg);
int printtime();

void update_rtctime();
void update_time();
void get_time_string(char*,int);
void get_user_config_info(cJSON*);


#endif
