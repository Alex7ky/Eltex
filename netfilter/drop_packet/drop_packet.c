#include "include/drop_packet.h"

unsigned int drop_hook(void *priv,
	struct sk_buff *skb,
	const struct nf_hook_state *state)
{
	/* отбрасываем все пакеты */
	return NF_DROP;
}

int init_module(void)
{
	pr_info("Module started ...\n");

	/* установка фильтра на входящий трафик */
	nf_register_hook(&netfilter_ops_in);

	/* установка фильтра на выходящий трафик */
	nf_register_hook(&netfilter_ops_out);

	return 0;
}

void cleanup_module(void)
{
	nf_unregister_hook(&netfilter_ops_in);
	nf_unregister_hook(&netfilter_ops_out);
	pr_info("Module stopped ...\n");
}
