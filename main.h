
#ifndef MIAN_H
#define MIAN_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <netpacket/packet.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <netinet/in.h>
#include <pthread.h>
#include <unistd.h>
#include "ip_mac_link.h"
#include "ip_link.h"
#include "get_interface.h"
#include <netinet/ip.h>
#include <arpa/inet.h>


int sockfd;

#define RECV_SIZE 2048
typedef struct recv_data{
	ssize_t msg_len ;
	unsigned char msg[RECV_SIZE];
}RECV_DATA;
#endif