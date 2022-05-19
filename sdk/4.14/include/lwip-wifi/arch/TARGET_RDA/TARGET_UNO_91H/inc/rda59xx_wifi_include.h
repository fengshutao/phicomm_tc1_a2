#ifndef _RDA59XX_WIFI_INCLUDE_H_
#define _RDA59XX_WIFI_INCLUDE_H_

#include "rda_def.h"
#include "rda59xx_wifi_common.h"
#include "sys_arch.h"
#include "wland_flash.h"
#include "wland_scan.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*************************************common******************************************/
#define WIFI_TXP_NUM                2
#define WIFI_CHANNEL_NUM            14
#define WIFI_TX_POWER_PHY_GN_REG  0x11F
#define WIFI_TX_POWER_PHY_B_REG   0x120
#define WIFI_TX_POWER_RF_REG      0x8A
#define WIFI_XTAL_CAL_REG         0xDA
enum RUNNING_MODE_T {
    STA_MODE = 0,//mac 0
    AP_MODE = 1,//mac 1
    P2P_MODE = 1,//mac 1
};
extern r_void maclib_init(void);
extern r_void rda59xx_txip_data(r_void* data, r_u32 len, r_u8 mode);
extern r_void *rda59xx_get_databuf(void);
extern r_void rda59xx_set_cb_queue(r_void* queue);
/*
 * mode    0 - not 11n   1 - 11n
 *
 * -----------------11n(mode 1)(Mbps)-----------------
 * rate          HT20                   HT40
 *       GI(800ns)  GI(400ns)   GI(800ns)  GI(400ns)
 *  0       6.5       7.2          13.5       15
 *  1       13       14.2           27        30
 *  2      19.5      21.7          40.5       45
 *  3       26       28.9           54        60
 *  4       39       43.3           81        90
 *  5       52       57.8           108       120
 *  6      58.5      65            121.5      135  
 *  7       65       72             135       150  
 *
 * --------------not 11n(mode 0)(Mbps)-----------------
 * rate   data rate     rate   data rate
 *  0     autorate        9        9        
 *  1        1           12       12       
 *  2        2           18       18       
 *  5       5.5          24       24       
 *  11       11          36       36       
 *  6        6           48       48
 *
 */  
extern r_s32 rda59xx_set_data_rate(r_u8 mode, r_u8 rate);
extern r_void rda59xx_send_rawdata(r_u8* data, r_u32 len);
extern r_s32 rda59xx_send_nulldata(r_s32 power_save);
extern r_s32 rda59xx_set_retrans_policy(r_u8 count);
extern r_s32 rda59xx_set_channel(r_u8 channel);
//extern r_u8 rda59xx_get_channel();
extern r_s32 rda59xx_filter_multicast(r_u8 enable);
extern r_void rda59xx_wland_task(r_void *arg);
extern r_void rda59xx_set_lib_debuglevel();
extern r_u8* rda59xx_get_ver(void);
extern r_void *wland_queue;
extern r_void rda59xx_set_ipmac(char* mac, int mode, int enable);
/** set the mode,use the function, the mode of AP and STA all changed
 *  @param bgn_enable,1: 11bgn; 0: 11bg .Default is 1.
**/
extern void rda5981_set_mode(unsigned char bgn_enable);
int rda5981_sniffer_enable_fcs(void);//for hiflying

int get_driver_version(void);
/********************************************sniffer************************************************/

/* Enable filtering ACK frames (no support)*/
//#define RDA_RX_FILTER_DROP_ACK                  BIT0

/* Enable filtering CTS frames (no support)*/
//#define RDA_RX_FILTER_DROP_CTS                  BIT1

/* Enable filtering RTS frames (no support)*/
//#define RDA_RX_FILTER_DROP_RTS                  BIT2

/* Enable filtering beacon frames */
#define RDA_RX_FILTER_DROP_BEACON               BIT3

/* Enable filtering ATIM frames (no support)*/
//#define RDA_RX_FILTER_DROP_ATIM                 BIT4

/* Enable filtering CF_END frames (no support)*/
//#define RDA_RX_FILTER_DROP_CF_END               BIT5

/* Enable filtering QCF_POLL frames (no support)*/
//#define RDA_RX_FILTER_DROP_QCF_POLL             BIT6

/* Filter Management frames which are not directed to current STA */
#define RDA_RX_FILTER_DROP_ND_MGMT              BIT7

/* Filter BC/MC MGMT frames belonging to other BSS */
#define RDA_RX_FILTER_DROP_BC_MC_MGMT_OTHER_BSS BIT8

/* Enable filtering of duplicate frames */
#define RDA_RX_FILTER_DROP_DUPLICATE            BIT9

/* Enable filtering of frames whose FCS has failed */
#define RDA_RX_FILTER_DROP_FCS_FAILED           BIT10

/* Enable filtering of De-authentication frame */
#define RDA_RX_FILTER_DROP_DEAUTH               BIT11

/* Filter BA frames which are not received as SIFS response (no support)*/
//#define RDA_RX_FILTER_DROP_NSIFS_RESP_BA        BIT12

/* Filter BA frames which are received as SIFS response (no support)*/
//#define RDA_RX_FILTER_DROP_SIFS_RESP_BA         BIT13

/* Filter frames which are received in secondary channel (20 MHz PPDU from Secondary channel) */
#define RDA_RX_FILTER_DROP_SEC_CHANNEL          BIT14

/* Filter BC/MC DATA frames belonging to other BSS */
#define RDA_RX_FILTER_DROP_BC_MC_DATA_OTHER_BSS BIT15

/* Filter DATA frames not directed to this station */
#define RDA_RX_FILTER_DROP_ND_DATA              BIT16

/* Filter Control frames which are not directed to current STA (no support)*/
//#define RDA_RX_FILTER_DROP_ND_CONTROL           BIT17

/* Filter Beacon frames (in IBSS mode) which are not used for adoption because the timestamp field is lower than TSF timer */
#define RDA_RX_FILTER_DROP_IBSS_BEACON          BIT18

typedef int (*sniffer_handler_t)(void *data, unsigned short data_len);

extern r_s32 rda59xx_sniffer_set_filter(r_u8 to_ds, r_u8 from_ds, r_u32 mgm_filter);
extern r_s32 rda59xx_sniffer_enable_internal(sniffer_handler_t handler);
extern r_s32 rda59xx_sniffer_disable_internal();




/********************************************scan**************************************************/
#define SCAN_TIMES          3
#define RECONN_TIMES        0
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
}rda59xx_scan_result;

typedef struct {
    r_u8 SSID[MAX_SSID_LEN];
    r_u8 SSID_len;
    r_u8 channel;
    r_u8 scan_mode;
    r_u8 scan_time;
    r_u8 country_code;
}rda59xx_scan_info;
extern wland_scan_list scan_res;
extern r_void rda59xx_set_country_code(r_u8 c_code);
extern r_void rda59xx_set_expired_time(r_u32 expired_time);
extern r_s32 rda59xx_del_scan_result(rda59xx_scan_result *del_result);
extern r_s32 rda59xx_del_scan_all_result(void);
extern r_s32 rda59xx_check_scan_result(const r_u8 *ssid, const r_u8 *bssid, const r_u8 channel);
extern r_s32 rda59xx_get_scan_result_special(rda59xx_scan_result *buf, const r_u8 *ssid, const r_u8 *bssid, const r_u8 channel);
extern r_s32 rda59xx_get_scan_result_index(rda59xx_scan_result *buf, const r_u8 index);
extern r_u32 rda59xx_get_scan_result(rda59xx_scan_result *buf, const r_u8 num);
extern r_u32 rda59xx_get_scan_num();
extern r_u32 rda59xx_scan_internal(rda59xx_scan_info *scan_info);
int rda5981_check_scan_result_name(const unsigned char *name);
int rda5981_check_scan_result(const char *ssid, const char *bssid, const unsigned char channel);
int rda5981_check_scan_result_name_bssid(const unsigned char *name, const unsigned char *bssid);
int rda5981_check_scan_result_chn(unsigned char chn);



/*****************************************sta**********************************************/
typedef struct{
    r_u8 ssid[MAX_SSID_LEN];
    r_u8 pw[MAX_PW_LEN];
    r_u8 bssid[ETH_ALEN];
    r_u8 channel;
    r_u8 dhcp;
    r_u32 ip;
    r_u32 netmask;
    r_u32 gateway;
}rda59xx_sta_info;

typedef struct {
    r_u8 bssid[ETH_ALEN];
    r_u8 ssid[MAX_SSID_LEN];
    /* Refer to dna_wlan_sec_type_e */
    r_u8 channel;
    r_u8 secure;
    r_s8 rssi;
    r_u32 ipaddr;
    r_u32 mask;
    r_u32 gateway;
    r_u32 dns1;
    r_u32 dns2;
} rda59xx_bss_info;
extern r_u8 rda59xx_get_STA_mode_chn(void);
extern r_s32 rda59xx_get_joined_AP(rda59xx_scan_result *bss);
extern void rda5981_set_auth_timeout(r_u8 timeout_enable);
extern r_u8 *rda_get_bssinfo_BSSID(void);
extern void rda5981_set_sta_link_loss_time(unsigned char time);
extern void rda5981_set_sta_listen_interval(unsigned char interval);
/*get sta_state in wland_task_thread*/
extern r_u8 rda59xx_get_wland_sta_state(void);
extern r_s32 rda59xx_set_txpower(r_u8 mode, r_u32 value);
extern r_s32 rda59xx_enter_CE_MODE(r_u8 enable);
extern r_s32 rda59xx_rssi_get(unsigned char* value);
extern r_s32 rda59xx_set_40Mmode(r_u8 enable);
extern r_s32 rda59xx_hw_recovery(r_u8 enable);
extern r_s32 rda59xx_rf_recalib(char* recalib_param,int num, int enable);
extern unsigned short get_rf_89val();
extern unsigned short get_rf_52val();
/****************************************ap************************************************/
typedef struct {
    r_u8 mac[ETH_ALEN];
    r_u32 ip;
}rda59xx_apsta_info_t;

typedef struct{
    r_u8 ssid[MAX_SSID_LEN];
    r_u8 pw[MAX_PW_LEN];
    r_u8 channel;
    r_u8 secure;
    r_u8 hidden;//add
    r_u8 mode;
    r_u8 link_num;
    r_u16 beacon;
    r_u32 ip;
    r_u32 netmask;
    r_u32 gateway;
    r_u32 dhcps;
    r_u32 dhcpe;
}rda59xx_ap_info;

extern r_u32 rda59xx_get_ap_join_info(rda59xx_apsta_info_t *buf, const r_u32 len);

extern r_void rda59xx_set_AP_white_list(r_u32 flag, r_u8 *mac);
extern r_void rda59xx_set_AP_black_list(r_u32 flag, r_u8 *mac);
extern r_u8 rda59xx_get_AP_mode_chn(void);
/*
**AP mode,set the delay time before wpa_start_eapol
**default is 15
*/
extern void rda5981_set_ap_eapol_delay(unsigned int ms);

extern int wland_set_beacon_info_padding(unsigned char enable, unsigned char* info, unsigned int len);
extern r_s32 wland_deauth_sta(char *mac);
/****************************************daemon************************************************/
extern r_s32 rda59xx_sta_connect(rda59xx_sta_info *sta_info);
extern r_s32 rda59xx_sta_disconnect();
extern r_s32 rda59xx_sta_get_ip(r_u32* ip_addr);
extern r_s32 rda59xx_ap_enable(rda59xx_ap_info *ap_info);
extern r_s32 rda59xx_ap_disable();
extern r_s32 rda59xx_scan(rda59xx_scan_info *scan_info);
extern r_s32 rda59xx_sniffer_enable(sniffer_handler_t handler);
extern r_s32 rda59xx_sniffer_disable();
extern r_u32 rda59xx_get_module_state();
extern r_void rda59xx_sta_get_bss_info(rda59xx_bss_info *bss_info);
extern r_void rda59xx_get_macaddr(r_u8 *macaddr, r_u32 mode);
extern void* cb_queue;

#ifdef _HSF_
int rda59xx_set_ap_hide(unsigned char hide);
int rda59xx_set_ap_link_num(unsigned char num);
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _RDA59XX_WIFI_INCLUDE_H_ */

