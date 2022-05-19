#ifndef _RDA_SYS_WRAPPER_H_
#define _RDA_SYS_WRAPPER_H_

#include "cmsis_os.h"
#include "rda_def.h"


#ifdef __cplusplus
extern "C" {
#endif

#define RDA_NO_WAIT         0
#define RDA_WAIT_FOREVER    osWaitForever    
#define NO_ERR  0
#define ERR     -1
#define RDA_PRIORITY  (osPriorityNormal)
#define RDA_THREAD_STACKSIZE DEFAULT_THREAD_STACKSIZE

/* Alarm */
// Timer definitions
#define osTimerInvalid  0
#define osTimerStopped  1
#define osTimerRunning  2

// Timer structures, same as os_timer_cb(rt_CMSIS.c)
typedef struct rda_ostmr_cb {
    struct rda_ostmr_cb *next;                    // Pointer to next active Timer
    uint8_t             state;                    // Timer State
    uint8_t              type;                    // Timer Type (Periodic/One-shot)
    uint16_t         reserved;                    // Reserved
    uint32_t             tcnt;                    // Timer Delay Count
    uint32_t             icnt;                    // Timer Initial Count
    void                 *arg;                    // Timer Function Argument
    const osTimerDef_t *timer;                    // Pointer to Timer definition
} rda_ostmr_cb_t;
/* Mail */
typedef struct{
    void* msgq;
    unsigned int msgsize;
}rda_mail_handle;
/* Queue */

//#define USING_STDLIB
#if !defined(USING_STDLIB)
typedef struct rda_tmr_node {
    rda_ostmr_cb_t cb;
    osTimerDef_t   def;
} rda_tmr_node_t;

typedef struct rda_tmr_ctrl {
    unsigned char *buff;
    unsigned char *state;
    unsigned char max_node_num;
    unsigned char node_size;
    unsigned char node_cntr;
    unsigned char last_freed_node_idx;
} rda_tmr_ctrl_t;

#define MAX_ALARM_NUM       (28)
#define WORD_ALIGN(n)       (((n) + 0x03UL) & ~0x03UL)
#define MAX_ALARM_STAT_SIZE WORD_ALIGN(MAX_ALARM_NUM >> 3)
#define MAX_ALARM_MEM_SIZE  (MAX_ALARM_NUM * WORD_ALIGN(sizeof(rda_tmr_node_t)) + MAX_ALARM_STAT_SIZE)
#endif
//#define CONFIG_DISABLE_ALL_INT
#define CRI_SEC_START_PRI_LEVEL 0xF8
#define CRI_SEC_END_PRI_LEVEL   0x00
static unsigned int g_critical_sec_counter = 0U;
#if defined(CONFIG_DISABLE_ALL_INT)
static unsigned int g_critical_ctxt_saved  = 0U;
#endif /* CONFIG_DISABLE_ALL_INT */



#define rda_msleep(ms) sys_msleep(ms)
            
#define rda_thread_yeild() osThreadYield()
/* Alarm */
/**
 * @brief     : get current time in units of micro second
 * @param[in] :
 * @param[out]:
 * @return    : return time value with uint32 type
 */
extern r_u32 rda_get_cur_time_ms(void);

/**
 * @brief     : create an alarm with given function, return timer handle
 * @param[in] : func(callback)/data(pass to func)/mode(once or periodic)
 * @param[out]:
 * @return    : return timer handle, a pointer to the timer structure, non-zero is valid
 */

extern r_void * rda_alarm_create_v2(r_void *func, r_u32 data, r_u32 mode);
extern r_void * rda_alarm_create(r_void *func, r_u32 data);

/**
 * @brief     : delete an alarm with given handle, then reset the handle
 * @param[in] : *handle(pointer to the timer structure)
 * @param[out]: *handle(address of the handle variable)
 * @return    :
 */
extern r_s32 rda_alarm_delete(r_void **handle);

/**
 * @brief     : start an alarm, raise a function call after given timeout delay
 * @param[in] : handle(pointer to the timer structure)/timeout(micro second)
 * @param[out]:
 * @return    :
 */

extern r_s32 rda_alarm_start(r_void *handle, r_u32 timeout_ms);

/**
 * @brief     : stop an alarm, will not raise a function call any more
 * @param[in] : handle(pointer to the timer structure)
 * @param[out]:
 * @return    :
 */
extern r_s32 rda_alarm_stop(r_void *handle);


/* Semaphore */
/**
 * @brief     : create a semaphore
 * @param[in] : name and initial valve of semaphore
 * @param[out]:
 * @return    : return ERR or NO_ERR
 */
extern r_void* rda_sem_create(r_u32 count);

/**
 * @brief     : wait a semaphore
 * @param[in] : name of semaphore
 * @param[out]:
 * @return    : return ERR or NO_ERR
 */
extern r_s32 rda_sem_wait(r_void *sem, r_u32 millisec);

/**
 * @brief     : release a semaphore
 * @param[in] : name of semaphore
 * @param[out]:
 * @return    : return ERR or NO_ERR
 */
extern r_s32 rda_sem_release(r_void *sem);

/**
 * @brief     : delete a semaphore
 * @param[in] : name of semaphore
 * @param[out]:
 * @return    : return ERR or NO_ERR
 */
extern r_s32 rda_sem_delete(r_void *sem);


/* Queue */
/**
 * @brief     : create a message queue
 * @param[in] : size of message queue
 * @param[out]:
 * @return    : return message queue id or NULL if error
 */
extern void* rda_msgQ_create(r_u32 msg_size);

/**
 * @brief     : put a message to queue
 * @param[in] : message queue id, message value and wait time
 * @param[out]:
 * @return    : return ERR or NO_ERR
 */
r_s32 rda_msg_put(r_void *msgQId, r_u32 msg, r_u32 millisec);

/**
 * @brief     : get a message from queue
 * @param[in] : message queue id, message value and wait time
 * @param[out]:
 * @return    : return ERR or NO_ERR
 */
r_s32 rda_msg_get(r_void *msgQId, r_u32 *value, r_u32 millisec);

/* Mail */
/**
 * @brief     : create a mail
 * @param[in] : mail count/size
 * @param[out]:
 * @return    : return mail handle
 */
void* rda_mail_create(r_u32 msgcnt, r_u32 msgsize);

/**
 * @brief     : get a msg from mail
 * @param[in] : handler name of mail/mail/wait time
 * @param[out]:
 * @return    : return ERR or NO_ERR
 */
r_s32 rda_mail_get(r_void *rdahandle, r_void *evt, r_u32 wait);

/**
 * @brief     : put a msg to mail
 * @param[in] : handler of mail/mail/wait time
 * @param[out]:
 * @return    : return ERR or NO_ERR
 */

r_s32 rda_mail_put(r_void *rdahandle, r_void *evt, r_u32 wait);

/* Mutex */
/**
 * @brief     : create a mutex
 * @param[in] :
 * @param[out]:
 * @return    : return ERR or NO_ERR
 */
extern r_void* rda_mutex_create(void);

/**
 * @brief     : wait a mutex
 * @param[in] : id of mutex and wait time
 * @param[out]:
 * @return    : return ERR or NO_ERR
 */
extern r_s32 rda_mutex_wait(r_void *rdamutex, r_u32 millisec);

/**
 * @brief     : release a mutex
 * @param[in] : id of mutex
 * @param[out]:
 * @return    : return ERR or NO_ERR
 */
extern r_s32 rda_mutex_realease(r_void *rdamutex);

/**
 * @brief     : delete a mutex
 * @param[in] : id of mutex
 * @param[out]:
 * @return    : return ERR or NO_ERR
 */
extern r_s32 rda_mutex_delete(r_void *rdamutex);

/* Thread */
/**
 * @brief     : creat a thread
 * @param[in] : thread name/thread function/thread fuction argument/stacksize/thread priority
 * @param[out]:
 * @return    : return thread id
 */
r_void* rda_thread_new(const r_u8 *pcName, r_void (*thread)(r_void *arg), r_void *arg, r_u32 stacksize, r_s32 priority);

/**
 * @brief     : delete a thread
 * @param[in] : thread id
 * @param[out]:
 * @return    : return ERR or NO_ERR
 */
extern r_s32 rda_thread_delete(r_void* id);

/**
 * @brief     : get current thread id
 * @param[in] :
 * @param[out]:
 * @return    : return thread id
 */
extern r_void* rda_thread_get_id(void);

/**
 * @brief     : set thread to sleep 
 * @param[in] :
 * @param[out]:
 * @return    : return thread id
 */
extern r_void rda_msleep(r_u32 ms);

extern r_void rda_critical_sec_start(void);
extern r_void rda_critical_sec_end(void);
extern r_void *rda_create_interrupt(r_u32 vec, r_u32 pri, r_void *isr);
extern r_void rda_delete_interrupt(r_u32 vec);
extern r_void rda_enable_interrupt(r_u32 vec);
extern r_void rda_disable_interrupt(r_u32 vec);
#ifdef __cplusplus
}
#endif

#endif /* _RDA_SYS_WRAPPER_H_ */

