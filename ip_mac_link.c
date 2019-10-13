#include "ip_mac_link.h"
//创建头节点
void ip_mac_create_link_header(NODE **header)
{
	NODE *tmp = malloc(sizeof(NODE));
	// tmp->data = 0;
	strcpy((char *)tmp->ip,"");
	strcpy((char *)tmp->mac,"");
	tmp->next = NULL;
	*header = tmp;
}
//头插
void ip_mac_link_insert_header( NODE *head,NODE *pnew)
{
	if(head == NULL)
		return;
	//将首节点的地址给到新的节点 的next
	pnew->next = head->next;
	// 新的节点的地址给到头节点的next
	head->next = pnew;
}

//遍历
void ip_mac_link_print(NODE *head)
{
	if(head == NULL)
		return;
	NODE *pcur = head->next;
	while(pcur != NULL)
	{
		//printf("%d ",pcur->data);
		printf("ip %d.%d.%d.%d,",pcur->ip[0],pcur->ip[1],pcur->ip[2],pcur->ip[3]);
		printf("mac %02x:%02x:%02x:%02x:%02x:%02x\n",pcur->mac[0],pcur->mac[1],pcur->mac[2],pcur->mac[3],pcur->mac[4],pcur->mac[5]);
		//printf("ip %s ,mac %s\n", pcur->ip,pcur->mac);
		pcur=pcur->next;
		
	}
}
//销毁
void ip_mac_link_destroy(NODE *head)
{
	if(head == NULL)
		return;
	NODE  *tmp=NULL;
	while(head != NULL)
	{
		tmp = head->next;
		free(head);
		head = tmp;		
	}
}

//查找值为x的节点,返回节点的地址
NODE *ip_mac_link_search_data(NODE *head,unsigned char *ip)
{
	if(head == NULL)
		return NULL;
	NODE *tmp =head->next;
	while(tmp != NULL)
	{
		if(memcmp(tmp->ip,ip,4)==0)
		{
			return tmp; 
		}
		tmp=tmp->next;	
	}
	return NULL;
}

void ip_mac_link_delete(NODE *head,unsigned char *ip)
{
	if(head == NULL || head->next == NULL)
		return;
	NODE *pcur = head->next;
	NODE *prev = head;
	while( pcur != NULL)
	{
		//pcur的data等于x
		if(memcmp(pcur->ip,ip,4)==0)
		{
			prev->next=pcur->next;
			free(pcur);
			pcur = prev->next;
			
		}
		else
		{
			pcur= pcur->next;
			prev=prev->next;
		}
		
		
	}
	
}


