/*
 *
 *      Filename: ch_dns_rdata_spf.h
 *
 *        Author: shajf,csp001314@gmail.com
 *   Description: ---
 *        Create: 2018-05-03 19:38:45
 * Last Modified: 2018-05-03 19:38:45
 */

#ifndef CH_DNS_RDATA_SPF_H
#define CH_DNS_RDATA_SPF_H

#include "ch_dns_rdata.h"
#include "ch_dns_rdata_pool.h"
#include "ch_dns_txt_base.h"

typedef struct ch_dns_rdata_spf_t ch_dns_rdata_spf_t;


struct ch_dns_rdata_spf_t {

	ch_dns_rdata_t rdata;

	ch_dns_txt_base_t txt_base;

};


extern void ch_dns_rdata_spf_init(ch_dns_rdata_pool_t *rdata_pool);

#endif /*CH_DNS_RDATA_SPF_H*/
