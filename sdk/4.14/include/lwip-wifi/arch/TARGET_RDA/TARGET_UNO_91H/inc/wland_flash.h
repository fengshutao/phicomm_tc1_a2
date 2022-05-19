#ifndef _WLAND_FLASH_H_
#define _WLAND_FLASH_H_
#include "rda_def.h"
#include "rda5981_flash.h"
#include "rda_ccfg_api.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

//#define FLASH_READ_CHECK

#ifndef min
#define min(a, b) ((a)<(b)?(a):(b))
#endif /*min*/

/** This struct contains all smartconfig mbed flash parameter. */
typedef struct
{
    char ssid[36];
    char key[64];
} wland_sta_data_t;

/** This struct contains ap data, include ssid key and channel. */
typedef struct
{
    r_u8 channel;
    char ssid[35];
    char key[64];
} wland_ap_data_t;

/** This struct contains ap net data. */
typedef struct
{
    r_u32 ip;
    r_u32 msk;
    r_u32 gw;
    r_u32 dhcps;
    r_u32 dhcpe;
} wland_ap_net_data_t;

typedef struct
{
    r_u32 fixip;
    r_u32 ip;
    r_u32 msk;
    r_u32 gw;
} wland_dhcp_t;

#define RDA5991H_USER_DATA_FLAG_UNINITIALIZED (0xFFFFFFFF & \
    (~(RDA5991H_USER_DATA_FLAG_MAC | \
    RDA5991H_USER_DATA_FLAG_STA | \
    RDA5991H_USER_DATA_FLAG_PMK | \
    RDA5991H_USER_DATA_FLAG_IP | \
    RDA5991H_USER_DATA_FLAG_PARTER_DATA_LEN | \
    RDA5991H_USER_DATA_FLAG_TX_POWER | \
    RDA5991H_USER_DATA_FLAG_XTAL_CAL | \
    RDA5991H_USER_DATA_FLAG_TX_POWER_RF | \
    RDA5991H_USER_DATA_FLAG_TX_POWER_PHY_GN | \
    RDA5991H_USER_DATA_FLAG_TX_POWER_PHY_B | \
    RDA5991H_USER_DATA_FLAG_AP | \
    RDA5991H_USER_DATA_FLAG_APNET | \
    RDA5991H_USER_DATA_FLAG_DHCP | \
    RDA5991H_USER_DATA_FLAG_UART | \
    RDA5991H_USER_DATA_FLAG_RF | \
    RDA5991H_USER_DATA_FLAG_RF_CHANNELS | \
    RDA5991H_USER_DATA_FLAG_PHY | \
    RDA5991H_USER_DATA_FLAG_PHY_CHANNELS | \
    RDA5991H_USER_DATA_FLAG_TX_POWER_OFFSET | \
    RDA5991H_USER_DATA_FLAG_CAL)))

#define RDA5991H_USER_DATA_IP_LENGTH 8

#define RDA5981_FLAG_FLAG "RDA5981"

typedef struct
{
    r_u32 flag;
    r_u8 rda5981_flag[8];
    r_u8 mac_addr[6];
    r_u8 tp_offset;
    r_u8 padding1;//alignment
    wland_sta_data_t sta_data;
    r_u8 pmk[32];
    r_u8 ip[RDA5991H_USER_DATA_IP_LENGTH];
    r_u32 parter_data_len;
    wland_tx_power_t tx_power;
    r_u16 xtal_cal;
    r_u8 padding2[2];//alignment
    r_u16 tx_power_rf[14];
    r_u8 tx_power_phy_gn;
    r_u8 tx_power_phy_b;
    r_u8 padding3[2];
    wland_ap_data_t ap_data;
    wland_ap_net_data_t ap_net_data;
    wland_dhcp_t dhcp;
    r_u32 uart;
    wland_rf_t rf;
    wland_rf_channels_t rf_channels;
    wland_phy_t phy;
    wland_phy_channels_t phy_channels;
    r_u32 cal_flag;
}rda5991h_user_data;

#define SECTOR_SIZE     4096
#define FLASH_SIZE 0x100000

#define RDA5991H_PARTITION_TABLE_END_ADDR 0x18001000 //partition table end addr

#define RDA5991H_USER_DATA_ADDR 0x180fb000 //4k(partition table)+500k+500k:
#define RDA5991H_3RDPARTER_DATA_ADDR 0x180fc000
#define RDA5991H_3RDPARTER_DATA_LEN 0x1000

#define FLASH_CTL_REG_BASE 0x17fff000
#define FLASH_CTL_TX_CMD_ADDR_REG (FLASH_CTL_REG_BASE + 0x00)
#define CMD_64KB_BLOCK_ERASE (0x000000d8UL)
#define WRITE_REG32(REG, VAL)    ((*(volatile unsigned int*)(REG)) = (unsigned int)(VAL))

#define FLASH_ERASE_FUN_ADDR 0x21f1//smartlink_erase_for_mbed
#define FLASH_WRITE_FUN_ADDR 0x2241//smartlink_write_for_mbed
#define FLASH_READ_FUN_ADDR 0x2243//smartlink_read_for_mbed
#define FLASH_ERASE_PARTITION_FUN_ADDR 0x2139//spi_flash_erase_partition
#define SPI_FLASH_READ_DATA_FOR_MBED_ADDR 0x2007//spi_flash_read_data_for_mbed
#define spi_flash_disable_cache_addr 0x1eb7//spi_flash_disable_cache
#define spi_flash_flush_cache_addr 0x1ecd//spi_flash_flush_cache
#define spi_flash_cfg_cache_addr 0x1e9f//spi_flash_cfg_cache
#define spi_flash_erase_4KB_sector_addr 0x23a3
#define spi_wip_reset_addr 0x1d8b
#define spi_write_reset_addr 0x1d9f
#define wait_busy_down_addr 0x1d81

#define FLASH_ERASE_FUN_ADDR_4 0x2221//smartlink_erase_for_mbed
#define FLASH_WRITE_FUN_ADDR_4 0x2271//smartlink_write_for_mbed
#define FLASH_READ_FUN_ADDR_4 0x2273//smartlink_read_for_mbed
#define FLASH_ERASE_PARTITION_FUN_ADDR_4 0x2169//spi_flash_erase_partition
#define SPI_FLASH_READ_DATA_FOR_MBED_ADDR_4 0x2037//spi_flash_read_data_for_mbed
#define spi_flash_disable_cache_addr_4 0x1ee7//spi_flash_disable_cache
#define spi_flash_flush_cache_addr_4 0x1efd//spi_flash_flush_cache
#define spi_flash_cfg_cache_addr_4 0x1ecf//spi_flash_cfg_cache
#define spi_flash_erase_4KB_sector_addr_4 0x23d3
#define spi_wip_reset_addr_4 0x1dbb
#define spi_write_reset_addr_4 0x1dcf
#define wait_busy_down_addr_4 0x1db1

/*
 * return 0 if find
 */
int rda5981_flash_read_pmk(r_u8 *pmk);
int rda5981_flash_read_sta_data(char *ssid, char *passwd);
void rda5981_spi_erase_partition(void*src, r_u32 counts);

//@len must be 4k aligment
//r_s32 rda5981_write_flash(r_u32 addr, r_s8 *buf, r_u32 len);
//r_s32 rda5981_read_flash(r_u32 addr, r_s8 *buf, r_u32 len);

#define VERSION_SZ          24
struct firmware_info
{
    r_u32 magic;
    r_u8 version[VERSION_SZ];

    r_u32 addr;
    r_u32 size;
    r_u32 crc32;
    r_u32 bootaddr;//add for rf_test
    r_u32 bootmagic;
};

#define RDA5981_FIRMWARE_MAGIC      0xEAEA

static inline void wait_busy_down_2(void)
{
    ((void(*)(void))wait_busy_down_addr)();
}

static inline void spi_write_reset_2(void)
{
    ((void(*)(void))spi_write_reset_addr)();
}

static inline void spi_wip_reset_2(void)
{
    ((void(*)(void))spi_wip_reset_addr)();
}

static inline void wait_busy_down_4(void)
{
    ((void(*)(void))wait_busy_down_addr_4)();
}

static inline void spi_write_reset_4(void)
{
    ((void(*)(void))spi_write_reset_addr_4)();
}

static inline void spi_wip_reset_4(void)
{
    ((void(*)(void))spi_wip_reset_addr_4)();
}

static void wait_busy_down(void)
{
    if (rda_ccfg_hwver() >= 4)
        wait_busy_down_4();
    else
        wait_busy_down_2();
}

static void spi_write_reset(void)
{
    if (rda_ccfg_hwver() >= 4)
        spi_write_reset_4();
    else
        spi_write_reset_2();
}

static void spi_wip_reset(void)
{
    if (rda_ccfg_hwver() >= 4)
        spi_wip_reset_4();
    else
        spi_wip_reset_2();
}

static inline void spi_flash_enable_cache(void)
{
    r_u32 func = spi_flash_cfg_cache_addr;
    if (rda_ccfg_hwver() >= 4) {
        func = spi_flash_cfg_cache_addr_4;
    }
    ((void(*)(void))func)();
}

static inline void spi_flash_disable_cache(void)
{
    r_u32 func = spi_flash_disable_cache_addr;
    if (rda_ccfg_hwver() >= 4) {
        func = spi_flash_disable_cache_addr_4;
    }
    ((void(*)(void))func)();
}

static inline void spi_flash_flush_cache(void)
{
    r_u32 func = spi_flash_flush_cache_addr;
    if (rda_ccfg_hwver() >= 4) {
        func = spi_flash_flush_cache_addr_4;
    }
    ((void(*)(void))func)();
}

static inline void rda5981_spi_flash_erase_4KB_sector(r_u32 addr)
{
    r_u32 func = spi_flash_erase_4KB_sector_addr;
    if (rda_ccfg_hwver() >= 4) {
        func = spi_flash_erase_4KB_sector_addr_4;
    }
    ((void(*)(r_u32))func)(addr);
}

static inline void RDA5991H_ERASE_FLASH(void*addr, r_u32 len)
{
    r_u32 func = FLASH_ERASE_FUN_ADDR;
    if (rda_ccfg_hwver() >= 4) {
        func = FLASH_ERASE_FUN_ADDR_4;
    }
    ((void(*)(void*, r_u32))func)(addr, len);
}

static inline void RDA5991H_WRITE_FLASH(r_u32 addr, r_u8 *data, r_u32 len)
{
    r_u32 func = FLASH_WRITE_FUN_ADDR;
    if (rda_ccfg_hwver() >= 4) {
        func = FLASH_WRITE_FUN_ADDR_4;
    }
    ((void(*)(r_u32, r_u8 *, r_u32))func)(addr, data, len);
}

static inline void RDA5991H_READ_FLASH(r_u32 addr, r_u8 *buf, r_u32 len)
{
    r_u32 func = FLASH_READ_FUN_ADDR;
    if (rda_ccfg_hwver() >= 4) {
        func = FLASH_READ_FUN_ADDR_4;
    }
    ((void(*)(r_u32, r_u8 *, r_u32))func)(addr, buf, len);
}

static inline void SPI_FLASH_READ_DATA_FOR_MBED(void*addr, void*buf, r_u32 len)
{
    r_u32 func = SPI_FLASH_READ_DATA_FOR_MBED_ADDR;
    if (rda_ccfg_hwver() >= 4) {
        func = SPI_FLASH_READ_DATA_FOR_MBED_ADDR_4;
    }
    ((void(*)(void*, void*, r_u32))func)(buf, addr, len);
}

/*
 * return 0:ok, else:error.
 */
int rda5981_flash_read_mac_addr(unsigned char *mac_addr);
int rda5981_flash_write_mac_addr(unsigned char *mac_addr);

/*
 * return 0:ok, else:error.
 */
int rda5981_flash_erase_uart(void);
int rda5981_flash_read_uart(unsigned int *uart);
int rda5981_flash_write_uart(unsigned int *uart);

/*
 * return 0:ok, else:error.
 */
int rda5981_flash_erase_cal_flag(void);
int rda5981_flash_read_cal_flag(unsigned int *flag);
int rda5981_flash_write_cal_flag(unsigned int *flag);


/*
 * return 0:ok, else:error.
 */
int rda5981_flash_read_ip_addr(unsigned char *ip_addr, unsigned char *server_addr);
int rda5981_flash_write_ip_addr(unsigned char *ip_add, unsigned char *server_addr);

/*
 * return 0:ok, else:error.
 */
int rda5981_flash_erase_dhcp_data(void);
int rda5981_flash_read_dhcp_data(unsigned int *enable, unsigned int *ip, unsigned int *msk, unsigned int *gw);
int rda5981_flash_write_dhcp_data(unsigned int enable, unsigned int ip, unsigned int msk, unsigned int gw);

/*
 * return 0:ok, else:error.
 */
int rda5981_flash_read_ap_data(char *ssid, char *passwd, unsigned char *channel);
int rda5981_flash_write_ap_data(const char *ssid, const char *passwd, unsigned char channel);
int rda5981_flash_erase_ap_data(void);

/*
 * return 0:ok, else:error.
 */
int rda5981_flash_read_ap_net_data(unsigned int *ip, unsigned int *msk, unsigned int *gw,
        unsigned int *dhcps, unsigned int *dhcpe);
int rda5981_flash_write_ap_net_data(unsigned int ip, unsigned int msk, unsigned int gw,
            unsigned int dhcps, unsigned int dhcpe);
int rda5981_flash_erase_ap_net_data(void);

/*
 * return 0:ok, else:error.
 */
int rda5981_flash_read_sta_data(char *ssid, char *passwd);
int rda5981_flash_write_sta_data(const char *ssid, const char *passwd);
int rda5981_flash_erase_sta_data(void);


/*
 * read 3rd parter data length from flash
 * return user data length
 */
int rda5981_flash_read_3rdparter_data_length(void);

/*
 * read 3rd parter data from flash
 * @buf, buf to store user data
 * @buf_len, length of buf
 * return user data length
 */
int rda5981_flash_read_3rdparter_data(unsigned char *buf, unsigned int buf_len);

/*
 * write 3rd parter data from flash
 * @buf, data to write
 * @buf_len, length of buf.
 * return 0:ok, else:fail
 */
int rda5981_flash_write_3rdparter_data(const unsigned char *buf, unsigned int buf_len);

/*
 * erase 3rd parter data from flash
 * return 0:ok, else:fail
 */
int rda5981_flash_erase_3rdparter_data(void);

/*
 * set flash size
 * @size, 1MB:0x100000, 2MB:0x200000, 4MB:0x400000. default size: 1MB
 * return 0:ok, else:fail
 */
int rda5981_set_flash_size(const unsigned int size);
int rda5981_get_flash_size(unsigned int *size);
/*
 * set userdata location on flash
 * @sys_data_addr, data to save system parameter, user can not operate this area directly.
 *         size:4KB. default location:0x180fb000
 * @user_data_addr, data to save user data. user can save own data in this area
 *         by @rda5981_flash_read_3rdparter_data
 *         and @rda5981_flash_write_3rdparter_data
 *         default location:0x180fc000
 * @user_data_len, user data length, default:4KB
 * return 0:ok, else:fail
 */
int rda5981_set_user_data_addr(const unsigned int sys_data_addr,
    const unsigned int user_data_addr, const unsigned int user_data_len);
int rda5981_get_user_data_addr(unsigned int *sys_data_addr,
    unsigned int *user_data_addr, unsigned int *user_data_len);
/*
 * function: erase flash
 * @addr: mast be 4k alignment
 * @len:  must be 4k alignment. (package 64KB erase and 4KB erase for different condition automatically)
 * return: 0:success, else:fail
 */
int rda5981_erase_flash(unsigned int addr, unsigned int len);

/*
 * function: write flash
 * @addr: mast be 256 alignment
 * @buf: data to be written, best be 4 alignment
 * @len: buffer len, mast be 4 alignment
 * return: 0:success, else:fail
 */
int rda5981_write_flash(unsigned int addr, char *buf, unsigned int len);

/*
 * function: read flash to @buf
 * @addr: best be 4 alignment
 * @buf: best be 4 alignment
 * @len: buffer len
 * return: 0:success, else:fail
 */
int rda5981_read_flash(unsigned int addr, char *buf, unsigned int len);

/*
 * function: read user data
 * @data: data to read
 * @len: length of data in byte
 * @flag: user data flag
 * return: 0:success, else:fail
 */
int rda5981_read_user_data(unsigned char *data, unsigned short len, unsigned int flag);

/*
 * function: write user data
 * @data: data to write
 * @len: length of data in byte
 * @flag: user data flag
 * return: 0:success, else:fail
 */
int rda5981_write_user_data(unsigned char *data, unsigned short len, unsigned int flag);

/*
 * function: erase user data
 * @flag: user data flag
 * return: 0:success, else:fail
 */
int rda5981_erase_user_data(unsigned int flag);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*_WLAND_FLASH_H_*/

