#ifndef _LINK_H_
#define _LINK_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct node
{
	//数据域
	//int data;
	unsigned char ip[4];
	unsigned char mac[6];
	//指针域
	struct node *next;
}NODE;
void ip_mac_create_link_header(NODE **head);
void ip_mac_link_insert_header( NODE *head,NODE *pnew);
void ip_mac_link_print(NODE *head);
void ip_mac_link_destroy(NODE *head);
NODE *ip_mac_link_search_data(NODE *head,unsigned char *ip);
void ip_mac_link_delete(NODE *head,unsigned char *ip);
#endif