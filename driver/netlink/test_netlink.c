#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


 
#define NETLINK_TEST 17
#define MAX_PAYLOAD 1024


 
int main(int argc, char **argv)
{
    int sock_fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_TEST);
	
	struct sockaddr_nl src_addr, dest_addr;
	struct msghdr msg;
	struct nlmsghdr *nlh = NULL;
	struct iovec iov;

    memset(&src_addr, 0, sizeof(src_addr));
    src_addr.nl_family = AF_NETLINK;
    src_addr.nl_pid = getpid();
    src_addr.nl_groups = 0;

    bind(sock_fd, (struct sockaddr*)&src_addr, sizeof(src_addr));
 
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.nl_family = AF_NETLINK;
    dest_addr.nl_pid = 0;       // send to kernel
    dest_addr.nl_groups = 0;
     
    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD));
    nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
    nlh->nlmsg_pid = getpid();
    nlh->nlmsg_flags = 0;
    strcpy(NLMSG_DATA(nlh), "Hello Kernel");
 
    iov.iov_base = (void *)nlh;
    iov.iov_len = nlh->nlmsg_len;
    msg.msg_name = (void *)&dest_addr;
    msg.msg_namelen = sizeof(dest_addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
 
    sendmsg(sock_fd, &msg, 0);  // send message to kernel
 
    // read message from kernel
    memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));
    int msgLen = recvmsg(sock_fd, &msg, 0);
    printf("Received mesage payload: %d|%s\n", iov.iov_len, (char *)NLMSG_DATA(nlh));

    // close socket
    close(sock_fd);

	return 0;
}



