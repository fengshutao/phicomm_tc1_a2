#ifndef _WLAND_TASK_H_
#define _WLAND_TASK_H_

#include "rda_def.h"
#include "csl_mbed.h"
#include "rda59xx_wifi_common.h"


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define WLAN_MAX_KEY_LEN        32

extern void *wifi_resp_sem;
extern maclib_func_t *maclib_func_p;
extern void *wland_queue;

#define FMW_HEADER_LEN                  4
#define WID_HEADER_LEN                  2

#define WLAND_MSGQ_SIZE    10
struct wland_dcmd {
    r_u16 wid_pkg_length;    /*
                 *  bit[0:11] : store pkg_length
                 *  bit[12:15]: store host_msg_type
                 *         PKT_TYPE_DATAOUT     0x1
                 *         PKT_TYPE_DATAIN      0x2
                 *         PKT_TYPE_CFG_RSP     0x3
                 *         PKT_TYPE_CFG_REQ     0x4
                 *         HOST_MSG_ASYNCEVENT  0x5
                 */
    r_u8 wid_msg_type;    /*
                 *'Q'  -- request  for wid query
                 *'W'  -- request  for wid write
                 *'R'  -- response for write or query
                 *'I'  -- mac status indication
                 *'N'  -- network info: scan AP list
                 */
    r_u8 wid_msg_id;        /* message id     */
    r_u16 wid_msg_length;    /* message length */
};
/* CDC flag definitions */
#define CDC_DCMD_LEN_MASK               0x0FFF    /* id an cmd pairing */
#define CDC_DCMD_LEN_SHIFT              12    /* ID Mask shift bits */
typedef enum {
    WLAND_STA_CONNECT = 0,
    WLAND_STA_DISCONNECT = 1,
    WLAND_STA_RECONNECT = 2,
    WLAND_STA_DISCONNECT_ERROR = 3,
    WLAND_STA_MAC_CONNECTED = 4,
    WLAND_STA_ADD_GTK = 5,
    WLAND_STA_ADD_PTK = 6,
    WLAND_STA_CON_FINISH = 7,
    WLAND_AP_START = 10,
    WLAND_AP_STOP = 11,
    WLAND_AP_MAC_CONNECTED = 12,
    WLAND_AP_ADD_GTK,
    WLAND_AP_EAPOL_3_OF_4,
    WLAND_AP_ADD_PTK,
    WLAND_AP_STA_JOINED,
    //WLAND_AP_STA_EXITED,
    WLAND_AP_STA_DEAUTH,
    WLAND_AUTO_RATE = 18,//
    WLAND_ARP_OFFLOAD = 19,//
    WLAND_SNIFFER_ENABLE = 20,
    WLAND_SNIFFER_DISABLE = 21,
    WLAND_WPS_CONNECT,
    WLAND_WPS_START,
    WLAND_WPS_DISCONNECT,
}WLAND_MSG;

enum AP_STATE_T {
    AP_CLOSE = 0,
    AP_INIT,
    AP_MACCONNECTED,
};

enum AP_STA_STATE_T {
    AP_DISCONNECT = 0,
    AP_ASSOC,
    AP_ASSOCED,
    AP_EAPOL_1,
    AP_EAPOL_3,
    AP_JOINED,
};

enum STA_STATE_T {
    STA_CLOSE = 0,
    STA_INIT,
    STA_CONNECT,
    STA_MACCONNECTED,
    STA_EAPOL_2,
    STA_EAPOL_4,
    STA_JOIND,
};

struct wland_ptkey {
    r_u8 ea[ETH_ALEN];    /* per station */
    r_u8 keyLen;        /* key length */
    r_u8 key[WLAN_MAX_KEY_LEN];    /* key data */
};

struct wland_gtkey {
    r_u8 ea[ETH_ALEN];    /* per station */
    r_u8 keyRSC[8];        /* key data */
    r_u8 keyIdx;        /* key index */
    r_u8 keyLen;        /* key length */
    r_u8 key[WLAN_MAX_KEY_LEN];    /* key data */
};

extern r_s32 wland_push_wid(r_u8 *buf, r_u16 wid, r_void *data, r_u16 len);
extern r_s32 wland_tx_msg(maclib_buf_t *msg, r_u16 msg_len, r_s32 mode);
extern r_s32 wland_fil_set_cmd_data(r_u16 cmd, r_void *data, r_u16 len, r_u8 mode);
extern r_s32 wland_fil_get_cmd_data(r_u16 cmd, r_void *data, r_u16 len, r_u8 mode);
extern r_void wland_tx_data(maclib_buf_t *msg, r_u32 msg_len, r_u8 mode);
extern r_void wland_get_buf(maclib_buf_t *msg);
extern r_u8* rda59xx_get_ver(void);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _WLAND_TASK_H_ */
