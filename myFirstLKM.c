#include <linux/module.h>
#include <linux/netlink.h>
#include <linux/sock.h>

#define MY_NETLINK_PROTOCOL_NUM 31

static struct sock *nl_sk;

struct netlink_kernel_cfg *cfg = {
	.input = nlh_recv_msg
}

int nlh_recv_msg(struct sk_buff *skb_in) {

	struct nlhmsghdr *nlh_recv, *nlh_reply;
	char *user_space_data;
	char kernel_reply[256];
	struct sk_buff* skb_out;

	nlh_recv = (struct nlmsghdr*) skb_in->data;
	nlmsg_dump(nlh_recv);

	user_space_process_port_id = nlh_recv->nlmsg_pid;
	printk(KERN_INFO "%s(%d) invoked from user space process with port id %u", __FUNCTION__, __LINE__, nlh_recv->nlmsg_pid);

	user_space_data = (char*) nlmsg_data(nlh_recv);
	printk(KERN_INFO "%s(%d) : msg received from user space=%s skb_in->len= %d, mlh_recv->nlmsg_len", __FUNCTION__, __LINE__, user_space_data, skb_in->len, nlh_recv->nlmsg_len);

	if(nlh_recv->nlmsg_flags & NLM_F_ACK) {
		memset(kernel_reply, 0, sizeof(kernel_reply));
		snprintf(kernel_reply, sizeof(kernel_reply), "MSG FROM PROCESS %d received and processed by kernel", nlh_recv->nlmsg_pid);
		skb_out = nlmsg_new(sizeof(kernel_reply), 0);
		nlh_reply = nlmsg_put(skb_out, 0, nlh_recv->nlmsg_seq, NLMSG_DONE, sizeof(kernel_reply), 0);
		strncpy(nlmsg_data(nlh_reply), kernel_reply, sizeof(kernel_reply));
		res = nlmsg_unicast(nl_sk, skb_out, nlh_recv->nlmsg_pid);
		if(res < 0) {
			printk(KERN_INFO "error sending data back to user space");
			kfree_skb(skb_out);
		}
	}

}

int hello_world_init_module(void) {

	printk(KERN_INFO "Routing management LKM  module loaded successfully \n");

	nl_sk = netlink_kernel_create(&init_net, MY_NETLINK_PROTOCOL_NUM, &cfg);

	if(!nl_sk) {
		printk(KERN_INFO "failed to start module");
		return -ENOMEM
	}

	return 0;
}

void hello_world_cleanup_module(void) {

	printk(KERN_INFO "cleanup hello world LKM \n");
	netlink_kernel_release(nl_sk);
}

module_init(hello_world_init_module);
module_exit(hello_world_cleanup_module);

#define AUTHOR_NAME ata
#define MODULE_DESC HELLO_WORLD_LINUX_KERNEL_MODULE

MODULE_AUTHOR(AUTHOR_NAME);
MODULE_DESCRIPTION(MODULE_DESC);
MODULE_LICENSE(GPL);
