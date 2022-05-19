#ifndef _RDA59XX_DAEMON_H
#define _RDA59XX_DAEMON_H

#include "rda59xx_wifi_include.h"
#include "sys_arch.h"
#include "wland_debug.h"
#include "netif.h"


#define WLAND_DBG_DUMP    0
#define WPA_DBG_DUMP      0
#define HUT_DBG_DUMP      0
#define WLAND_DBG_LEVEL D_NONE_LEVEL
#define WPA_DBG_LEVEL   D_NONE_LEVEL
#define WLANDLIB_DBG_LEVEL D_NONE_LEVEL

//#define WIFISTACK_DEBUG
#ifdef WIFISTACK_DEBUG
#define WIFISTACK_PRINT(fmt, ...) do {\
            printf(fmt, ##__VA_ARGS__);\
    } while (0)
#else
#define WIFISTACK_PRINT(fmt, ...)
#endif

extern r_void* daemon_queue;

#define DAEMON_MAILQ_SIZE   10

typedef enum {
    DAEMON_SCAN = 0,
    DAEMON_STA_CONNECT = 1,
    DAEMON_STA_DISCONNECT = 2,
    DAEMON_STA_RECONNECT = 3,
    DAEMON_AP_ENABLE = 10,
    DAEMON_AP_DISABLE= 11,
    DAEMON_SNIFFER_ENABLE,
    DAEMON_SNIFFER_DISABLE,
    DAEMON_RSSI_GET,
}DAEMON_MSG;

typedef enum {
    RDA59XX_WLAND_STA_CONNECT = 0,
    RDA59XX_WLAND_STA_DISCONNECT = 1,
    RDA59XX_WLAND_AP_START = 10,
    RDA59XX_WLAND_AP_STOP = 11,
    RDA59XX_WLAND_ARP_OFFLOAD = 19,
}RDA59XX_WLAND_MSG;

typedef enum {
    ERR_DEVICE = -1,
    ERR_SEND_MSG = -2,
    ERR_AUTH = -3,
    ERR_CONNECTION = -4,
    ERR_DHCP = -5,
}RDA_ERR_TYPE;

typedef enum {
    DISCONNECT_ACTIVE = 1,
    DISCONNECT_PASSIVE = 2,
}DISCONNECT_MODE;

typedef enum 
{
    EVENT_STA_DISCONNECTTED           = 0x00,
    EVENT_STA_NOFOUND_AP,
    EVENT_STA_CONNECT_FAIL,
    EVENT_STA_CONNECTTED,
    EVENT_STA_GOT_IP,
    EVENT_AP_STARTED                  = 0x10,
    EVENT_AP_START_FAIL,
    EVENT_AP_STOPED,
}WIFI_EVENT;

typedef void (*wifi_event_cb_t)(WIFI_EVENT evt, void* info);

#define STATE_DSIABLE       (0)
#define STATE_INIT          BIT0
#define STATE_SNIFFER       BIT1
#define STATE_STA           BIT2
#define STATE_STA_RC        BIT3
#define STATE_P2P           BIT4
#define STATE_AP            BIT5

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/* Mbed interface mac address
 * if MBED_MAC_ADD_x are zero, interface uid sets mac address,
 * otherwise MAC_ADD_x are used.
 */
__STATIC_INLINE int mac_is_valid(char* mac)
{
    return (mac[0] | mac[1] | mac[2] | mac[3] | mac[4] | mac[5]);
}

typedef void (*wifi_event_cb_t)(WIFI_EVENT evt, void* info);

extern r_s32 rda59xx_wifi_init();
extern r_s32 rda59xx_wifi_set_event_cb(wifi_event_cb_t cb);
extern r_s32 rda59xx_sta_connect(rda59xx_sta_info *sta_info);
extern r_s32 rda59xx_sta_disconnect();
extern r_s32 rda59xx_sta_get_ip(r_u32* ip_addr);
extern r_s32 rda59xx_ap_enable(rda59xx_ap_info *ap_info);
extern r_s32 rda59xx_ap_disable();
extern r_s32 rda59xx_scan(rda59xx_scan_info *scan_info);
extern r_s32 rda59xx_sniffer_enable(sniffer_handler_t handler);
extern r_s32 rda59xx_sniffer_disable();
extern r_u32 rda59xx_get_module_state();
extern r_u32 rda59xx_get_wifi_state();
extern r_u32 rda59xx_get_wifi_state_ap();
extern r_void rda59xx_sta_get_bss_info(rda59xx_bss_info *bss_info);
extern r_void rda59xx_get_macaddr(r_u8 *macaddr, r_u32 mode);
extern r_void rda59xx_set_macaddr(r_u8 *macaddr, r_u32 mode);
extern r_s32 rda59xx_send_daemon_msg(rda_msg *msg, r_u32 wait_time);
extern r_s32 rda59xx_rssi_get(unsigned char* value);
void rda59xx_write_rf_DA(unsigned short val);
void rda59xx_write_rf_8B(unsigned short val);
void rda59xx_set_highTemp_rfRecalib(bool enable);

#ifdef _HSF_
void sta_disconnect_callback(void);
void sta_connected_callback(void);
void sta_dhcp_gotip_callback(unsigned int ip);
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif/*_RDA59XX_DAEMON_H*/
