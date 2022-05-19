#ifndef _WLAND_DEBUG_H
#define _WLAND_DEBUG_H

#include "rda_def.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#define RDA_WLAND_DEBUG
#define RDA_WPA_DEBUG

enum {
    D_NONE_LEVEL = 0,
    D_ERROR_LEVEL = 1,
    D_INFO_LEVEL = 2,
    D_DEBUG_LEVEL = 3,
};

extern r_u32 rda_wland_dbg_level;
extern r_u32 rda_wpa_dbg_level;
extern r_u32 rda_maclib_dbg_level;
extern r_u32 rda_wland_dump;
extern r_u32 rda_wpa_dump;
extern r_u32 rda_hut_dump;

//if frmae_len is zero, get len from frame header
static r_void rda_dump_frame(const r_s8* msg, r_u8 *data, r_u32 frame_len)
{
    r_u32 len,i;

    if(frame_len != 0)
        len = frame_len;
    else
        len = data[0] | ((data[1]&0x0f) << 8);
    r_printf("%s : ",msg);
    for(i=0; i<len; i++)
        r_printf("%02x ", *(data+i));
    r_printf("\r\nframe_len=%d\r\n", len);    
    return;
}

#ifdef RDA_WLAND_DEBUG
#define WLAND_DBG(level, fmt, ...)  do {\
        r_u32 dbg_level = D_##level##_LEVEL;\
        if((dbg_level <= rda_wland_dbg_level)){\
            r_printf("%s,"fmt, __func__, ##__VA_ARGS__);\
        }\
    } while (0)

#define wland_dump_frame(msg, data, len)    do{ if (rda_wland_dump) { (rda_dump_frame)(msg, data, len); }}while(0)
#else
#define WLAND_DBG(level, fmt, ...)
#define wland_dump_frame(msg, data, len) {}
#endif

#ifdef RDA_WPA_DEBUG
#define WPA_DEBUG(level, fmt, ...)  do {\
        r_u32 dbg_level = D_##level##_LEVEL;\
        if((dbg_level <= rda_wpa_dbg_level)){\
            r_printf("%s,"fmt, __func__, ##__VA_ARGS__);\
        }\
    } while (0)

#define wpa_dump_frame(msg, data, len)    do{ if (rda_wpa_dump) { (rda_dump_frame)(msg, data, len); }}while(0)
#else
#define WPA_DEBUG(level, fmt, ...)
#define wpa_dump_frame(msg, data, len) {}
#endif


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* _RDA59XX_DEBUG_H */
