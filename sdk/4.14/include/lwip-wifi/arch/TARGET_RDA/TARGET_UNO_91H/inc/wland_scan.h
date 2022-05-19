#ifndef _WLAND_SCAN_H_
#define _WLAND_SCAN_H_
#include "rda_def.h"
#include "rda_list_head.h"
#include "rda59xx_wifi_common.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define USER_SCAN                       BIT0

enum SITE_SURVEY_T
{
    SITE_SURVEY_1CH = 0,
    SITE_SURVEY_ALL_CH = 1,//channel 1~14
    SITE_SURVEY_OFF = 2,
    SITE_SURVEY_NA_CH = 4,// channel 1~11
    SITE_SURVEY_EU_CH = 5// chanenl 1~13
};
enum CHN__MAX_T
{
    ALL_CH_MAX = 14,//channel 1~14
    NA_CH_MAX = 11, // channel 1~11
    EU_CH_MAX = 13  // chanenl 1~13
};
typedef struct {
    r_u8 BSSID[ETH_ALEN];
    r_u8 SSID[MAX_SSID_LEN];
    r_u8 SSID_len;
    r_u8 channel;
    r_u8 secure_type;//0:none, BIT(0)~BIT(1):wpa, BIT(2)~BIT(3):wpa2,BIT(4):wep
    r_u8 wmm;
    r_s8 RSSI;
    //r_u8 *ie;
    r_u16 capability;
    //r_u32 ie_length;  /* byte length of Information Elements */
    r_u64 time;
    struct list_head list;
}wland_scan_result;

typedef struct {
    r_u8 bss_num;
    struct list_head scan_list;    
}wland_scan_list;

extern wland_scan_list scan_res;
extern r_void *wifi_scan_sem;
extern r_s32 wland_scan_handler(r_u8 *msg_buf);

#ifdef CONFIG_WPS
typedef void (*wps_probe_req_handler_t)(void);
extern void rda5981_set_wps_probereq_handler(wps_probe_req_handler_t handler);
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*_WLAND_SCAN_H_*/

