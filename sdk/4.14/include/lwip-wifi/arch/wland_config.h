#ifndef _RDA59XX_CONFIG_H
#define _RDA59XX_CONFIG_H

#include "rda59xx_wifi_common.h"

//COMMON
#define WLAND_QOS_ENABLE
//#define WLAND_40M_ENABLE
//#define WLAND_TXAMSDU_ENABLE
#define WLAND_RXAMSDU_ENABLE
#define WIFI_LINK_LOSS_THRESHOLD                50//50 beacon
#define WIFI_LISTEN_INTERVAL                    1//multiple of router's period
//SCAN
#define SCAN_EXPIRED_TIME           30*1000  //default 30s
#define SCAN_CHANNEL_TIME           20      /* ms */
#define SCAN_ACTIVE_TIME            20      /* ms */
#define SCAN_PASSIVE_TIME           102     /* ms */
#define DEFAULT_COUNTRY_CODE        JP

//AP
#define MAX_STA_NUM                 8//max num is 8

#endif /* _RDA59XX_CONFIG_H */

