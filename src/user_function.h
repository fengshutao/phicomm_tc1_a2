
#ifndef __USER_FUNCTION_H_
#define __USER_FUNCTION_H_


#include <hsf.h>
#include "cJSON/cJSON.h"


bool json_plug_analysis(unsigned char x, cJSON *pJsonRoot);
bool json_plug_task_analysis(unsigned char x, unsigned char y, cJSON *pJsonRoot);
void user_send( int udp_flag, char *s );
void USER_FUNC user_function_cmd_received(char *pusrdata, int datalen);
unsigned char strtohex(char a, char b);



#endif
