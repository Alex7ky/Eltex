#include "include/drop_packet.h"

unsigned int hook_packet_udp(void *priv,
	struct sk_buff *skb,
	const struct nf_hook_state *state)
{

	struct iphdr *ip_header;
	struct udphdr *udp_header;

	/* Проверяем что это IP пакет */
	if (skb->protocol == htons(ETH_P_IP)) {
		ip_header = ip_hdr(skb);
		/** Aлтернатива ip_hdr(skb);
		 *  ip_header = (struct iphdr *)skb_network_header(skb);
		 */

		if (ip_header->version == 4
		    && ip_header->protocol == IPPROTO_UDP) {

			udp_header = udp_hdr(skb);
			/** Альтернатива udp_hdr(skb);
			 *  udp_header = (struct udphdr *) skb_transport_header(skb);
			 */

			if (udp_header->dest == htons(32001)) {
				pr_info("port chenged!\n");
				udp_header->dest = htons(32002);
			}
		}
	}

	/* Пропускаем дальше все пакеты */
	return NF_ACCEPT;
}

int init_module(void)
{
	pr_info("Module started ...\n");

	/* установка фильтра на входящий трафик */
	nf_register_hook(&netfilter_ops_in);

	return 0;
}

void cleanup_module(void)
{
	nf_unregister_hook(&netfilter_ops_in);

	pr_info("Module stopped ...\n");
}
