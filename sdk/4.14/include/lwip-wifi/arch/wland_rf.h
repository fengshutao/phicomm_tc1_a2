#ifndef _WLAND_RF_H_
#define _WLAND_RF_H_


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define WLAND_TXP_NUM       2
#define WLAND_CHANNEL_NUM   14
#define WLAND_TX_POWER_PHY_GN_REG  0x11F
#define WLAND_TX_POWER_PHY_B_REG   0x120
#define WLAND_TX_POWER_RF_REG      0x8A
#define WLAND_XTAL_CAL_REG         0xDA

#define WLAND_TXP_11F_BASE  0x25
#define WLAND_TXP_11F_END   0x6A
#define WLAND_TXP_120_BASE  0x15
#define WLAND_TXP_120_END   0x54


#define MAKE_WORD16(lsb, msb)           (((r_u16)(msb) << 8)  & 0xFF00) | (lsb)
#define MAKE_WORD32(lsw, msw)           (((r_u32)(msw) << 16) & 0xFFFF0000) | (lsw)


/*
 * function: update tx power from efuse data, for reg 11F and 120
 * return: 0:success, else:fail
 */
int update_tx_power_from_efuse(void);

/*
 * function: update xtal calibration from efuse data, for reg DA
 * return: 0:success, else:fail
 */
int update_xtal_cal_from_efuse(void);

/*
 * function: update mac addr from flash data
 * return: 0:success, else:fail
 */
int update_mac_addr_from_efuse(void);

/*
 * function: update tx power from flash data, Deprecated version
 * return: 0:success, else:fail
 */
int update_tx_power_from_flash(void);

/*
 * function: update tx power from flash data, for reg 8A
 * return: 0:success, else:fail
 */
int update_tx_power_rf_from_flash(void);

/*
 * function: update tx power from flash data, for reg 11F and 120
 * return: 0:success, else:fail
 */
int update_tx_power_phy_from_flash(void);

/*
 * function: update xtal calibration from flash data
 * return: 0:success, else:fail
 */
int update_xtal_cal_from_flash(void);

/*
 * function: update mac addr from flash data
 * return: 0:success, else:fail
 */
int update_mac_addr_from_flash(void);

/*
 * function: write rf reg
 * @reg: rf reg data
 * @value: rf reg value
 * return: 0:success, else:fail
 * eg: 0x00DA:xtal calibration
 */
int wland_rf_write(unsigned short reg, unsigned short value);

/*
 * function: write rf reg
 * @reg: rf reg data
 * @value: rf reg value
 * @len : value length
 * return: 0:success, else:fail
 * eg: 0x008A:tx power rf
 */
int wland_rf_write_all_channels(unsigned short reg, unsigned short *value, unsigned short len);

/*
 * function: read rf reg
 * @reg: rf reg data
 * @value: rf reg value
 * return: 0:success, else:fail
 */
int wland_rf_read(unsigned short reg, unsigned short *value);

/*
 * function: read rf reg
 * @reg: rf reg data
 * @value: rf reg value
 * return: 0:success, else:fail
 * eg: 0x008A:tx power rf
 */
int wland_rf_read_all_channels(unsigned short reg, unsigned short *value);

/*
 * function: write phy reg
 * @reg: phy reg data
 * @value: phy reg value
 * return: 0:success, else:fail
 */
int wland_phy_write(unsigned int reg, unsigned int value);

/*
 * function: write phy reg
 * @reg: phy reg data
 * @value: phy reg value
 * @len : value length
 * return: 0:success, else:fail
 */
int wland_phy_write_all_channels(unsigned int reg, unsigned int *value, unsigned short len);

/*
 * function: read phy reg
 * @reg: phy reg data
 * @value: phy reg value
 * return: 0:success, else:fail
 */
int wland_phy_read(unsigned int reg, unsigned int *value);

/*
 * function: read phy reg
 * @reg: phy reg data
 * @value: phy reg value
 * return: 0:success, else:fail
 */
int wland_phy_read_all_channels(unsigned int reg, unsigned int *value);

/* efuse API start */
/* Efuse CAN ONLY WRITE ONCE! DO NOT CALL THESE API IF YOU DO KNOW WHAT THEY MEANS!!! */

/*
 * function: read all efuse
 * @value: buffer to store efuse data, 28 bytes
 * return: 0:success, else:fail
 */
int wland_read_efuse(unsigned char *value);

/*
 * function: read tx power from efuse
 * @tx_power: 2 bytes, first is mode g/n(range 0x25~0x54), second is mode b(range 0x15~0x54).
 * return: 0:success, else:fail
 */
int wland_read_tx_power_from_efuse(unsigned char *tx_power);

/*
 * function: read tx power from efuse
 * @tx_power: 2 bytes, first is mode g/n(range 0x25~0x54), second is mode b(range 0x15~0x54)
 * @len: must be 2
 * return: 0:success, else:fail
 */
int wland_write_tx_power_to_efuse(unsigned char *tx_power, unsigned char len);

/*
 * function: read xtal cal from efuse
 * @xtal_cal: 1 byte, maximum 0x7F
 * return: 0:success, else:fail
 */
int wland_read_xtal_cal_from_efuse(unsigned char *cal_val);

/*
 * function: write xtal cal to efuse
 * @xtal_cal: 1 byte, maximum 0x7F
 * @len : must be 1
 * return: 0:success, else:fail
 */
int wland_write_xtal_cal_to_efuse(unsigned char *xtal_cal, unsigned char len);

/*
 * function: write mac to efuse
 * @xtal_cal: 6 bytes
 * return: 0:success, else:fail
 */
int wland_read_mac_addr_from_efuse(unsigned char *mac_addr);

/*
 * function: write mac to efuse
 * @xtal_cal: 6 bytes
 * @len : must be 6
 * return: 0:success, else:fail
 */
int wland_write_mac_addr_to_efuse(unsigned char*mac_addr, unsigned char len);
/* efuse API end */

/*
 * function: start rf test
 * @argc: number of argv
 * @argv: args for test, 6 elements for tx test, 4 elements for rx test
 * @is_tx: 1 for tx test, 0 for rx test
 * return: 0:success, else:fail
 */
int wland_start_rf_test(unsigned int argc, unsigned int *argv, unsigned int is_tx);

/*
 * function: stop rx test
 * return: 0:success, else:fail
 */
int wland_stop_rx_test(void);

/*
 * function: get rf test result
 * @result buffer to store rx result
 * return: 0:success, else:fail
 */
int wland_get_rx_result(char *result);

/*
 * function: restart rx test, use last rx test args
 * return: 0:success, else:fail
 */
int wland_restart_rx_test(void);

/*
 * function: stop tx test
 * return: 0:success, else:fail
 */
int wland_stop_tx_test(void);

/*
 * function: restart tx test, use last tx test args
 * return: 0:success, else:fail
 */
int wland_restart_tx_test(void);

#define RDA5981_FIRMWARE_INFO_ADDR 0x18003000
/*
 * function: reboot to assigned addr (onece).
 * reboot to rf test mode, not for OTA
 * @firmware_info_addr: firmware info addr, depend on your flash layout
 * @reboot_addr: reboot addr, 0x18001000-0x1840000
 * return: 0:success, else:fail
 */
int rda5981_reboot_to_addr(unsigned int firmware_info_addr, unsigned int reboot_addr);

/*
 * function: read reg and corresponding value related to test mode stored in flash
 * @reg: reg to read
 * @value: buffer to store value
 * @flag: user data flag
 * return: 0:success, else:fail
 */
int rda5981_read_user_data_regs(unsigned char *reg, unsigned char *value, unsigned int flag);

/*
 * function: write reg and corresponding value related to test mode stored in flash
 * @reg: reg to write
 * @value: buffer that stores value
 * @flag: user data flag
 * return: 0:success, else:fail
 */
int rda5981_write_user_data_regs(unsigned char *reg, unsigned char *value, unsigned int flag);

/*
 * function: erase reg and corresponding value related to test mode stored in flash
 * @reg: reg to erase
 * @flag: user data flag
 * return: 0:success, else:fail
 */
int rda5981_erase_user_data_regs(unsigned char *reg, unsigned int flag);

/*
 * function: get flash Manufacturer ID
 * @mid:
 * return: 0:success, else:fail
 */
int rda5981_flash_get_mid(unsigned char *mid);

/*
 * function: get flash Device ID
 * @did:
 * return: 0:success, else:fail
 */
int rda5981_flash_get_did(unsigned char *did);

/*
 * function: get flash ID
 * @mid:
 * return: 0:success, else:fail
 */
int rda5981_flash_get_jdid(unsigned short *jdid);

/*
 * function: read mac reg
 * @reg: mac reg data
 * @value: mac reg value
 * return: 0:success, else:fail
 */
int wland_mac_reg_read(unsigned short  reg, unsigned int *value);

/*
 * function: write mac reg
 * @reg: mac reg data
 * @value: mac reg value
 * return: 0:success, else:fail
 */
int wland_mac_reg_write(unsigned short reg, unsigned int value);

int wland_set_memory_32bit(unsigned int addr, unsigned int val);
int wland_get_memory_32bit(unsigned int addr, unsigned int *val);

/*set wid's value, include wid type: char/short/integer*/
/*success,return 0; fai,return -1*/
int wland_set_wid(r_u16 wid, r_u32 val);

/*get wid's value, include wid type: char/short/integer*/
/*success,return 0; fai,return -1*/
/*value is the result:the wid's value*/
int wland_get_wid(r_u16 wid, r_u32 *value);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _WLAND_RF_H_ */

