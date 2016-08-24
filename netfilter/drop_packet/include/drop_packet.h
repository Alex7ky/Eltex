#ifndef _DROP_H
#define _DROP_H

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>

#define NF_IP_PRE_ROUTING	0
#define NF_IP_POST_ROUTING	4

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alexandr Stolpovsky <alex7ky@yandex.ru>");
MODULE_DESCRIPTION("dropped incoming/outgoing packets");
MODULE_VERSION("1.0");

unsigned int drop_hook(void *,
	struct sk_buff *,
	const struct nf_hook_state *);

/* hook-функкия для входящих пакетов */
struct nf_hook_ops netfilter_ops_in = {
	.hook = drop_hook,
	.pf = PF_INET,
	.hooknum = NF_IP_PRE_ROUTING,
	.priority = NF_IP_PRI_FIRST,
};

/* hook-функкия для исходящих пакетов */
struct nf_hook_ops netfilter_ops_out = {
	.hook = drop_hook,
	.pf = PF_INET,
	.hooknum = NF_IP_POST_ROUTING,
	.priority = NF_IP_PRI_FIRST,
};

#endif