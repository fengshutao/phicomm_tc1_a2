#ifndef _RDA5981_FLASH_H_
#define _RDA5981_FLASH_H_

#include "rda_def.h"

/** This struct contains tx power parameter. */
typedef struct
{
    r_u8 b[14];
    r_u8 g[14];
    r_u8 n[14];
    r_u8 sum;
    r_u8 padding1;//alignment
} wland_tx_power_t;

/*
 *function: store reg and corresponding value related to test mode
 *@valid: 1 means data is valid
 *@flag: if biti(i=0~7) == 1, it means reg_val[i] is in use
 *@reg_val: store reg and value, reg_val[i][0] is reg, reg_val[i][1] ~ reg_val[i][14] is value
 */
typedef struct {
    r_u32 valid;
    r_u32 flag;
    r_u16 reg_val[8][2];
} wland_rf_t;

typedef struct {
    r_u32 valid;
    r_u32 flag;
    r_u16 reg_val[8][15];
} wland_rf_channels_t;

typedef struct {
    r_u32 valid;
    r_u32 flag;
    r_u32 reg_val[8][2];
} wland_phy_t;

typedef struct {
    r_u32 valid;
    r_u32 flag;
    r_u32 reg_val[8][15];
} wland_phy_channels_t;

/* if you add or delete any macros below, modify RDA5991H_USER_DATA_FLAG_UNINITIALIZED at the same time */
#define RDA5991H_USER_DATA_FLAG_MAC BIT0
#define RDA5991H_USER_DATA_FLAG_STA BIT1
#define RDA5991H_USER_DATA_FLAG_PMK BIT2
#define RDA5991H_USER_DATA_FLAG_IP BIT3
#define RDA5991H_USER_DATA_FLAG_PARTER_DATA_LEN BIT4
#define RDA5991H_USER_DATA_FLAG_TX_POWER BIT5
#define RDA5991H_USER_DATA_FLAG_XTAL_CAL BIT6
#define RDA5991H_USER_DATA_FLAG_TX_POWER_RF BIT7
#define RDA5991H_USER_DATA_FLAG_TX_POWER_PHY_GN BIT8
#define RDA5991H_USER_DATA_FLAG_TX_POWER_PHY_B BIT9
#define RDA5991H_USER_DATA_FLAG_AP BIT10
#define RDA5991H_USER_DATA_FLAG_APNET BIT11
#define RDA5991H_USER_DATA_FLAG_DHCP BIT12
#define RDA5991H_USER_DATA_FLAG_UART BIT13
#define RDA5991H_USER_DATA_FLAG_RF BIT14
#define RDA5991H_USER_DATA_FLAG_RF_CHANNELS BIT15
#define RDA5991H_USER_DATA_FLAG_PHY BIT16
#define RDA5991H_USER_DATA_FLAG_PHY_CHANNELS BIT17
#define RDA5991H_USER_DATA_FLAG_TX_POWER_OFFSET BIT18
#define RDA5991H_USER_DATA_FLAG_CAL BIT19

#define RDA5981_VBAT_CAL BIT0
#define RDA5981_GPADC0_CAL BIT1
#define RDA5981_GPADC1_CAL BIT2
#define RDA5981_PRODUCT_ID BIT3
#define RDA5981_POWER_CLASS BIT4

#ifdef __cplusplus
extern "C" {
#endif

/*
 * function: read VBAT Calibration value
 * y = k * x + b
 * @k: slope
 * @b: offset
 * return: 0:success, else:fail
*/
r_s32 rda5981_flash_read_vbat_cal(float *k, float *b);

#ifdef __cplusplus
}
#endif

#endif

