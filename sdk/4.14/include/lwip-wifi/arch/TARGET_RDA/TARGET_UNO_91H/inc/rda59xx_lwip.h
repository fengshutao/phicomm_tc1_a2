#ifndef _RDA59XX_LWIP_H
#define _RDA59XX_LWIP_H

#include "rda_def.h"
#include "lwip/netif.h"
#include "lwip/ip_addr.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef rda59xx_netif_is_linkup
#define rda59xx_netif_is_linkup(netif)    \
        netif_is_link_up(netif)
#endif

#ifndef rda59xx_netif_is_up
#define rda59xx_netif_is_up(netif)    \
        netif_is_up(netif)
#endif

#ifndef rda59xx_arch_sem_wait
#define rda59xx_arch_sem_wait(s,t)    \
        sys_arch_sem_wait(s,t)
#endif
const ip_addr_t * rda59xx_get_netif_ip(struct netif *netif);
extern r_void rda59xx_add_dns_addr(struct netif *netif);
extern r_void rda59xx_netif_input(struct netif *netif, r_u8 *data, r_u32 len);
extern r_void rda59xx_sta_netif_init(struct netif *netif);
extern r_void rda59xx_ap_netif_init(struct netif *netif);
extern r_void rda59xx_sta_netif_link_failed();
extern r_void rda59xx_netif_link_down(r_u32 netif);
extern r_void rda59xx_netif_link_up(r_u32 netif);
extern r_void rda59xx_netif_down(r_u32 netif);
extern r_void rda59xx_netif_up(r_u32 netif);
extern void rda59xx_netif_data_input(r_u8 *data, r_u32 len, r_u32 idx, r_u32 netif);
extern char *rda59xx_get_ip_address(char *buf, size_t buflen, size_t module);
extern const char *rda59xx_get_netmask(char *buf, size_t buflen, size_t module);
extern char *rda59xx_get_gateway(char *buf, size_t buflen, size_t module);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _RDA59XX_LWIP_H */

