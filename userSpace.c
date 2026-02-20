#include <stdio.h>
#include <stdlib.h>
#include <linux/netlink.h>
#include <stdint.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <memory.h>
#include <pthread.h>
#include "netLinkKernelUtils.h"

int netlink_send_msg_to_kernel(int sock_fd, char *msg, uint32_t msg_size, int nlmsg_type, uint16_t flags) {
	struct sockaddr_nl dst_addr;
	memset(&dst_addr, 0, sizeof(dst_addr));
	dst_addr.nl_family = AF_NETLINK;
	dst_addr.nl_pid = 0;

	struct nlmsghdr *nlh = (struct nlmsghdr*) calloc(1, NLMSG_HDRLEN + NLMSG_SPACE(msg_size));

	nlh->nlmsg_len = NLMSG_HDRLEN + NLMSG_SPACE(msg_size);
	nlh->nlmsg_pid = getpid();
	nlh->nlmsg_flags = flags;
	nlh->nlmsg_type = nlmsg_type;
	nlh->nlmsg_seq = 0;

	strncpy(NLMSG_DATA(nlh), msg, msg_size);

	struct iovec iov;
	iov.iov_base = (void *) nlh;
	iov.iov_len = nlh->nlmsg_len;

	static struct msghdr outermsghdr;
	memset(&outermsghdr, 0, sizeof(struct msghdr));
	outermsghdr.msg_name = (void *) &dst_addr;
	outermsghdr.msg_namelen = sizeof(dst_addr);
	outermsghdr.msg_iov = &iov;
	outermsghdr.msg_iovlen = 1;

	int rc = sendmsg(sock_fd, &outermsghdr, 0);

	if(rc < 0) {
		printf("MSG sending failed, error no : %d\n", errno);
	}
	return rc;
}




static void greet_kernel(int sock_fd, char *msg, uint32_t msg_len) {
	netlink_send_msg_to_kernel(sock_fd, msg, msg_len, NLMSG_GREET, NLM_F_ACK);
}




int main (int argc, char **argv) {
	int choice;
	int sock_fd;

	sock_fd = create_netlink_socket(MY_NETLINK_PROTOCOL_NUM);
	if(sock_fd == -1) {
		printf("ERROR! netlink socker creation failed. error : %d\n", errno);
		exit(EXIT_FAILURE);
	}

	struct sockaddr_nl src_addr;
	struct nlmsghdr *nlh;

	src_addr.nl_family = AF_NETLINK;
	src_addr,nl_pid = getpid();

	if(bind(sock_fd, (struct sockaddr*)&src_addr, sizeof(src_addr)) == -1) {
		printf("failed to bind");
		exit(1);
	}

	while(1) {
		printf("Main Menu\n");
		printf("\t1.Greet Kernel\n");
		printf("\t2.Exit\n");
		scanf("%d\n", &choice);

		switch(choice) {
			case 1 : {
					char user_msg[MAX_PAYLOAD];
					memset(user_msg, 0, MAX_PAYLOAD);
					if(fgets(user_msg, MAX_PAYLOAD, stdin) === NULL) {
						printf("error reading from stdin");
						exit(EXIT_FAILURE);
					}
					greet_kernel(sock_fd, user_msg, strlen(user_msg));
				}
				break;
			case 2 : 
				exit_userspace(sock_fd);
				break;
			
		}
	}

	return 0;
}



