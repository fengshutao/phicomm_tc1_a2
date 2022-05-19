/* LWIP implementation of NetworkInterfaceAPI
 * Copyright (c) 2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef LWIP_STACK_H
#define LWIP_STACK_H

#include "nsapi.h"
#include "emac_api.h"
#include <stdbool.h>
#include "rda59xx_wifi_include.h"
#ifdef __cplusplus
extern "C" {
#endif

// Access to lwip through the nsapi
nsapi_error_t mbed_lwip_init(emac_interface_t *emac);
#if !defined(TARGET_UNO_91H)
nsapi_error_t mbed_lwip_bringup(bool dhcp, const char *ip, const char *netmask, const char *gw);
const char *mbed_lwip_get_mac_address(void);
#else
nsapi_error_t mbed_lwip_scan_inf(rda59xx_scan_info* r_scan_info);
size_t mbed_lwip_get_scan_result(rda59xx_scan_result *bss_list, const u8_t num);
size_t mbed_lwip_get_ap_join_info(rda59xx_apsta_info_t *sta_list, const size_t num);
size_t mbed_lwip_get_joined_AP(rda59xx_scan_result *bss);
nsapi_error_t mbed_lwip_bringup(const char *ssid, const char *pass, const char *bssid, bool dhcp, const char *ip, const char *netmask, const char *gw);
nsapi_error_t mbed_lwip_bringup_inf(rda59xx_sta_info* r_sta_info);

nsapi_error_t mbed_lwip_startap(const char *ssid, const char *pass, const char *ip, const char *netmask, const char *gw,
                        const char *dhcp_start, const char *dhcp_end, int channel, char mode);
nsapi_error_t mbed_lwip_startap_inf(rda59xx_ap_info* r_ap_info);


nsapi_error_t mbed_lwip_stopap(unsigned char state);

nsapi_error_t mbed_lwip_bringdownlink(void);
nsapi_error_t mbed_lwip_status(void);
nsapi_error_t mbed_lwip_ap_status(void);
const char *mbed_lwip_get_mac_address(int mac_no);
#ifdef _HSF_
nsapi_error_t mbed_lwip_set_hostname(const char* hostname, int netif_no);
#endif

int mbed_lwip_same_mac(const char *mac1, const char *mac2);
int mbed_lwip_zero_mac(const char *mac);
int mbed_lwip_set_channel(int chn);
#endif
nsapi_error_t mbed_lwip_bringdown(void);

char *mbed_lwip_get_ip_address(char *buf, int buflen);
char *mbed_lwip_get_ip_address_ap(char *buf, int buflen);
char *mbed_lwip_get_netmask(char *buf, int buflen);
char *mbed_lwip_get_gateway(char *buf, int buflen);

extern nsapi_stack_t lwip_stack;


#ifdef __cplusplus
}
#endif

#endif

