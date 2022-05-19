/*****************************************************************************/
/*                                                                           */
/*  File Name         : csl_mbed.h                                           */
/*                                                                           */
/*  Description       : This file contains all declarations and functions    */
/*                      related to the chip support library.                 */
/*                                                                           */
/*  MbedOS Usage      : Call maclib_get_funcs_struct() to get MACLib funcs;  */
/*                      Define mbed_reg_func_t var, Register it by ml_init;  */
/*                                                                           */
/*****************************************************************************/

#ifndef CSL_MBED_H
#define CSL_MBED_H

#include "rda_def.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************/
/* Constants                                                                 */
/*****************************************************************************/
#define RDA_EXT_INT_MAC_HW_INDEX        8
#define RDA_EXT_INT_MAC_HW_PRI          0x80

/*****************************************************************************/
/* Enums                                                                     */
/*****************************************************************************/
typedef enum {
    MACLIB_EVENT_PEND       = 0,
    MACLIB_EVENT_PROCESS    = 1,
    MACLIB_EVENT_CLEANUP    = 2
} MACLIB_EVENT_HANDLE_T;

/*****************************************************************************/
/* Data Types                                                                */
/*****************************************************************************/
typedef struct
{
    r_u32   ml_id;      /* Buffer identification */
    r_u8* ml_data;   /* Pkt start address     */
    r_u16 ml_len;     /* Pkt length            */
} maclib_buf_t;

/* Structure that contains functions provided by MACLib */
typedef struct {
    /* Initialize MAC Library, input param: mbed_reg_func_t *reg_funcs */
    r_s32 (*ml_init)(r_void *reg_funcs);

    /* As a peroid task to process MAC Library background event */
    r_void (*ml_tasklet)(void);

    /* Get a new packet buffer, output param: maclib_buf_t *buf */
    r_void (*ml_get_pkt_buf)(r_void *buf);

    /* Mbed stack send packet to MAC Library, input param: maclib_buf_t *buf*/
    r_s32 (*ml_xmit_pkt)(r_void *buf);

    /* Mbed receive and processing packet done, input param: unsigned int buf_id */
    r_void (*ml_recv_pkt_comp)(r_u32 buf_id);
} maclib_func_t;

/* Structure that contains functions provided by MbedOS */
typedef struct {
    /* MAC Library send packet to mbed stack, input param: maclib_buf_t *buf */
    r_void (*recv_pkt)(r_void *buf);

    /* Critical section start realized in mbed */
    r_void (*critical_sec_start)(void);

    /* Critical section end realized in mbed */
    r_void (*critical_sec_end)(void);

    /* Create interrupt in mbed, input param: vector/priority/isr(function), */
    /* return: interrupt handle, non-zero is valid                           */
    r_void * (*create_interrupt)(r_u32 vec, r_u32 pri, r_void *isr);

    /* Delete interrupt in mbed, input param: vector */
    r_void (*delete_interrupt)(r_u32 vec);

    /* Enable interrupt in mbed, input param: vector */
    r_void (*enable_interrupt)(r_u32 vec);

    /* Disable interrupt in mbed, input param: vector */
    r_void (*disable_interrupt)(r_u32 vec);

    /* Get current time realized in mbed, return time in units of micro second */
    r_u32 (*get_cur_time_ms)(void);

    /* Create alarm in mbed, input param: func(callback)/data(pass to func), */
    /* return: alarm handle, non-zero is valid                               */
    r_void * (*create_alarm)(r_void *func, r_u32 data);

    /* Delete alarm in mbed, input param: alarm handle */
    r_void (*delete_alarm)(r_void **handle);

    /* Start alarm in mbed, input param: alarm handle/timeout(micro second)  */
    r_void (*start_alarm)(r_void *handle, r_u32 timeout_ms);

    /* Stop alarm in mbed, input param: alarm handle */
    r_void (*stop_alarm)(r_void *handle);

#if defined(MBED_MUTEX_INTERFACE)
    /* Create mutex */
    r_void (*mutex_create)(r_void);

    /* Delete mutex */
    r_u32 (*mutex_delete)(r_void *rdamutex);

    /* Wait mutex, timer unit : millisec */
    r_u32 (*mutex_wait)(r_void *rdamutex, r_u32 millisec);

    /* Release mutex */
    r_u32 (*mutex_release)(r_void *rdamutex);
#endif /* MBED_MUTEX_INTERFACE */

    /* Event post/get callback function, input param: event_type */
    r_void (*event_hdl_cb)(r_u32 event);

    /* maclib task sleep callback function */
    r_void (*task_sleep_cb)(void);

    /* maclib task wakeup callback function */
    r_void (*task_wakeup_cb)(void);
} maclib_import_func_t;

/*****************************************************************************/
/* Extern Function Declarations                                              */
/*****************************************************************************/
extern r_void maclib_get_funcs_struct(maclib_func_t *func_str);

#ifdef __cplusplus
}
#endif

#endif /* CSL_MBED_H */
