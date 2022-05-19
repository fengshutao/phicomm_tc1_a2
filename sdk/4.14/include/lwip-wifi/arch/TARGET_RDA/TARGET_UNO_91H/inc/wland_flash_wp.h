#ifndef _WLAND_FLASH_WP_H_
#define _WLAND_FLASH_WP_H_

#ifdef __cplusplus
extern "C" {
#endif
//#define FLASH_PROTECT_ENABLE

#define FLASH_CTL_REG_BASE              0x17fff000
#define FLASH_CTL_TX_CMD_ADDR_REG       (FLASH_CTL_REG_BASE + 0x00)
#define FLASH_CTL_TX_BLOCK_SIZE_REG     (FLASH_CTL_REG_BASE + 0x04)
#define FLAHS_CTL_TX_FIFO_DATA_REG      (FLASH_CTL_REG_BASE + 0x08)
#define FLASH_CTL_STATUS_REG            (FLASH_CTL_REG_BASE + 0x0c)
#define FLAHS_CTL_RX_FIFO_DATA_REG      (FLASH_CTL_REG_BASE + 0x10)

#define WRITE_REG32(REG, VAL) ((*(volatile unsigned int*)(REG)) = (unsigned int)(VAL))
#define WRITE_REG8(REG, VAL) ((*(volatile unsigned char*)(REG)) = (unsigned char)(VAL))
#define MREAD_WORD(addr)      *((volatile unsigned int *)(addr))

#define MID_GD      0xC8
#define MID_WINBOND 0xEF

#define FLASH_1M    0x100000
#define FLASH_2M    0x200000
#define FLASH_4M    0x400000
extern unsigned int flash_size;

#ifdef FLASH_PROTECT_ENABLE

#if 0
#define FLASH_4M_WP_MASK        0x407c
#define FLASH_4M_WP_NONE        0x0000
#define FLASH_4M_WP_4K          0x0044
#define FLASH_4M_WP_8K          0x0048
#define FLASH_4M_WP_16K         0x004c
#define FLASH_4M_WP_32K         0x0050
#define FLASH_4M_WP_1_64        0x0024
#define FLASH_4M_WP_1_32        0x0028
#define FLASH_4M_WP_1_16        0x002c
#define FLASH_4M_WP_1_8         0x0030
#define FLASH_4M_WP_1_4         0x0034
#define FLASH_4M_WP_1_2         0x0038
#define FLASH_4M_WP_3_4         0x4014
#define FLASH_4M_WP_7_8         0x4010
#define FLASH_4M_WP_15_16       0x400c
#define FLASH_4M_WP_31_32       0x4008
#define FLASH_4M_WP_63_64       0x4004
#define FLASH_4M_WP_ALL         0x1c
#endif

#define FLASH_WP_MASK           0x407c
#define FLASH_WP_NONE           0x0000
#define FLASH_WP_ALL            0x1c

#define FLASH_4M_WP_4K          0x0064
#define FLASH_4M_WP_8K          0x0068
#define FLASH_4M_WP_16K         0x006c
#define FLASH_4M_WP_32K         0x0070
#define FLASH_4M_WP_1_64        0x0024
#define FLASH_4M_WP_1_32        0x0028
#define FLASH_4M_WP_1_16        0x002c
#define FLASH_4M_WP_1_8         0x0030
#define FLASH_4M_WP_1_4         0x0034
#define FLASH_4M_WP_1_2         0x0038
#define FLASH_4M_WP_3_4         0x4014
#define FLASH_4M_WP_7_8         0x4010
#define FLASH_4M_WP_15_16       0x400c
#define FLASH_4M_WP_31_32       0x4008
#define FLASH_4M_WP_63_64       0x4004
#define FLASH_4M_WP_127_128     0x4058
#define FLASH_4M_WP_255_256     0x404C
#define FLASH_4M_WP_1023_1024   0x4044

#define FLASH_2M_WP_4K          0x0064
#define FLASH_2M_WP_8K          0x0068
#define FLASH_2M_WP_16K         0x006c
#define FLASH_2M_WP_32K         0x0070
#define FLASH_2M_WP_1_32        0x0024
#define FLASH_2M_WP_1_16        0x0028
#define FLASH_2M_WP_1_8         0x002c
#define FLASH_2M_WP_1_4         0x0050
#define FLASH_2M_WP_1_2         0x0051
#define FLASH_2M_WP_3_4         0x4010
#define FLASH_2M_WP_7_8         0x400c
#define FLASH_2M_WP_15_16       0x4004
#define FLASH_2M_WP_31_32       0x4000
#define FLASH_2M_WP_63_64       0x4050
#define FLASH_2M_WP_127_128     0x404c
#define FLASH_2M_WP_255_256     0x4048
#define FLASH_2M_WP_511_512     0x4044


#define FLASH_1M_WP_4K          0x0064
#define FLASH_1M_WP_8K          0x0068
#define FLASH_1M_WP_16K         0x006c
#define FLASH_1M_WP_32K         0x0070
#define FLASH_1M_WP_1_16        0x0024
#define FLASH_1M_WP_1_8         0x0028
#define FLASH_1M_WP_1_4         0x002c
#define FLASH_1M_WP_1_2         0x0050
#define FLASH_1M_WP_3_4         0x400C
#define FLASH_1M_WP_7_8         0x4008
#define FLASH_1M_WP_15_16       0x4004
#define FLASH_1M_WP_31_32       0x4050
#define FLASH_1M_WP_63_64       0x404C
#define FLASH_1M_WP_127_128     0x4048
#define FLASH_1M_WP_255_256     0x4044

#endif

extern void flash_wrtie_protect_all();
extern void flash_wrtie_protect_none();
extern void flash_wrtie_protect(unsigned int offset);
extern void rda5981_flash_init();

#ifdef __cplusplus
}
#endif

#endif /*_WLAND_FLASH_WP_H_*/
