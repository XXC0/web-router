#include "ip_link.h"
//创建头节点
void ip_create_link_header(IPNODE **header)
{
	IPNODE *tmp = malloc(sizeof(IPNODE));
	// tmp->data = 0;
	strcpy((char *)tmp->fi_ip,"");
	tmp->next = NULL;
	*header = tmp;
}
//头插
void ip_link_insert_header( IPNODE *head,IPNODE *pnew)
{
	if(head == NULL)
		return;
	//将首节点的地址给到新的节点 的next
	pnew->next = head->next;
	// 新的节点的地址给到头节点的next
	head->next = pnew;
}

//遍历
void ip_link_print(IPNODE *head)
{
	if(head == NULL)
		return;
	IPNODE *pcur = head->next;
	while(pcur != NULL)
	{
		//printf("%d ",pcur->data);
		printf("ip %d.%d.%d.%d \n", pcur->fi_ip[0],pcur->fi_ip[1],pcur->fi_ip[2],pcur->fi_ip[3]);
		pcur=pcur->next;
		
	}
}
//销毁
void ip_link_destroy(IPNODE *head)
{
	if(head == NULL)
		return;
	IPNODE  *tmp=NULL;
	while(head != NULL)
	{
		tmp = head->next;
		free(head);
		head = tmp;		
	}
}
//查找值为x的节点,返回节点的地址
IPNODE *ip_link_search_data(IPNODE *head,unsigned char *ip)
{
	if(head == NULL)
		return NULL;
	IPNODE *tmp =head->next;
	while(tmp != NULL)
	{
		if(memcmp(tmp->fi_ip,ip,4)==0)
		{
			return tmp; 
		}
		tmp=tmp->next;	
	}
	return NULL;
}

void ip_link_delete(IPNODE *head,unsigned char *ip)
{
	if(head == NULL || head->next == NULL)
		return;
	IPNODE *pcur = head->next;
	IPNODE *prev = head;
	while( pcur != NULL)
	{
		//pcur的data等于x
		if(memcmp(pcur->fi_ip,ip,4)==0)
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


