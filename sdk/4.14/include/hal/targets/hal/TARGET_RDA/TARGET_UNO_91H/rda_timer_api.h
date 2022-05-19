#ifndef __RDA_TIMER_API_H
#define __RDA_TIMER_API_H

#include <stdint.h>
#include <stddef.h>

#include "RDA5991H.h"

//#define US_TICK_USING_TIMER0

#define TIMER_RELOAD_AUTO  1  /* timer reload automatic */
#define TIMER_RELOAD_MANU  2  /* timer reload manual */

#ifdef __cplusplus
extern "C" {
#endif

typedef enum{
	eRDA_TIMER0 = 0,
	eRDA_TIMER1,
	eRDA_FREETIMER
}RDA_TIMER_PORT;

typedef void (*hal_timer_cb_t)(void *arg);
typedef void (*hal_timer_cb_no_para_t)(void);

typedef struct {
    uint32_t       period;
    uint8_t        reload_mode;
    union {
        hal_timer_cb_t cb;
        hal_timer_cb_no_para_t cb_v2;
    };
    void          *arg;
} timer_config_t;

typedef struct {
    int8_t         port;   /* timer port */
    timer_config_t config; /* timer config */
    void          *priv;   /* priv data */
} timer_dev_t;

/**
 * init a hardware timer
 *
 * @param[in]  tmr         timer struct
 * @param[in]  period      micro seconds for repeat timer trigger
 * @param[in]  auto_reoad  set to 0, if you just need one shot timer
 * @param[in]  cb          callback to be triggered after useconds
 * @param[in]  ch          timer channel
 * @param[in]  arg         passed to cb
 */
int32_t hal_timer_init(timer_dev_t *tim);

/**
 * start a hardware timer
 *
 * @return  0 == success, EIO == failure
 */
int32_t hal_timer_start(timer_dev_t *tim);

/**
 * stop a hardware timer
 *
 * @param[in]  tmr  timer struct
 * @param[in]  cb   callback to be triggered after useconds
 * @param[in]  arg  passed to cb
 */
void hal_timer_stop(timer_dev_t *tim);

/**
 * De-initialises an TIMER interface, Turns off an TIMER hardware interface
 *
 * @param[in]  timer  the interface which should be de-initialised
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t hal_timer_finalize(timer_dev_t *tim);

/*us delay*/
static inline void cpu_delay_us(uint32_t us){

    uint32_t idx = us;
   
    while( idx-- ) {
        for(int i = 0; i < 38; i++){
            __ASM volatile("nop");
        }
    }
}

void rda_timer_irq_set(void);

void sys_ms_timestamp_timer_start(void);
uint32_t sys_ms_timestamp_read(void);
void sys_ms_timestamp_set(uint32_t timestamp);
void sys_ms_timestamp_timer_disable_int(void);

#define rFREE_TIMER_INITVAL         (RDA_SCU->FTMRINITVAL)
#define rFREE_TIMER_PRESCALE        (RDA_SCU->FTMRPREVAL)
#define rFREE_TIMER_TIMESTAMP       (RDA_SCU->FTMRTS)
#define rRDA_POWER_CONTROL          (RDA_SCU->PWRCTRL)
#define FREE_TIMER_INIT_VAL         (0xFFFFFFFFUL)

static inline void sys_ms_timer_clear_int(void){
    RDA_SCU->PWRCTRL |= (0x01UL << 28);
    __DSB();
    while (RDA_SCU->PWRCTRL & (0x01UL << 28));
}

static inline void sys_ms_timer_disable_int(void){
    rRDA_POWER_CONTROL |= (uint32_t)(0x01UL << 8);
    rRDA_POWER_CONTROL &= ~(0x01UL << 18);
    rRDA_POWER_CONTROL &= (~(uint32_t)(1 << 8));
}

#ifdef __cplusplus
}
#endif

#endif
