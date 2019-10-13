#ifndef _IP_LINK_H_
#define _IP_LINK_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct ip_node
{
	//数据域
	//int data;
	unsigned char fi_ip[4];
	//指针域
	struct ip_node *next;
}IPNODE;
void ip_create_link_header(IPNODE **head);
void ip_link_insert_header(IPNODE *head,IPNODE *pnew);
void ip_link_print(IPNODE *head);
void ip_link_destroy(IPNODE *head);
IPNODE *ip_link_search_data(IPNODE *head,unsigned char *ip);
void ip_link_delete(IPNODE *head,unsigned char *ip);
#endif