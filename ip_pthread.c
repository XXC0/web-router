#include "ip_pthread.h"

extern NODE *head ;
extern IPNODE *fi_head;


void ip_deal(unsigned char *getway,unsigned char *recv_msg,int ret,unsigned char *dst_ip,int sockfd,char *ifr_name_)
{


    NODE *tmp = NULL;
    memcpy(recv_msg+6,getway,6);
    tmp = ip_mac_link_search_data(head,dst_ip);
    if (tmp)
    {
        memcpy(recv_msg,tmp->mac,6);
    }
    struct sockaddr_ll sll;
    struct ifreq ethreq;
    strncpy(ethreq.ifr_name,ifr_name_,IFNAMSIZ);
    if(-1==ioctl(sockfd,SIOCGIFINDEX,(char*)&ethreq))
    {
        perror("");
        close(sockfd);
    }
    bzero(&sll,sizeof(sll));
    sll.sll_ifindex = ethreq.ifr_ifindex;
    sendto(sockfd,recv_msg,ret,0,(struct sockaddr*)&sll,sizeof(sll));

}


unsigned short checksum(unsigned short *buffer,int size) 
{ 
    unsigned long cksum=0; 
    while(size>1) 
    { 
        cksum+=*buffer++; 
        size-=sizeof(unsigned short); 
    } 
    if(size) 
    { 
        cksum+=*(unsigned char *)buffer; 
    } 
    //将32位数转换成16 
    while (cksum>>16) 
        cksum=(cksum>>16)+(cksum & 0xffff); 
    return (unsigned short) (~cksum); 
}

void *ip_packet(void *arg)
{
    int num = get_interface_num();
    RECV_DATA *recv_msg = (RECV_DATA *)arg;
    unsigned char src_ip[4] = "";
    unsigned char dst_ip[4] = "";
    memcpy(src_ip,recv_msg->msg+26,4);
    memcpy(dst_ip,recv_msg->msg+30,4);

    for (int i = 0; i < num; ++i)
    {
        if(recv_msg->msg[23] == 1 && memcmp(recv_msg->msg+30,net_interface[i].ip,4)== 0)
        {
            recv_msg->msg[22]--;
            *((unsigned short *)recv_msg->msg+24) = checksum((unsigned short *)(recv_msg->msg+14),20);
        }
    }

    if (ip_link_search_data(fi_head,src_ip) || ip_link_search_data(fi_head,dst_ip))//filter IP
    {
        return NULL;
    }
    else
    {
        for (int i = 0; i < num; i++)
        {
            
            if (memcmp(recv_msg->msg+30,net_interface[i].ip,3)== 0)
            {
                if(recv_msg->msg[33]==255){
                    return NULL;
                }
                if(strcmp(net_interface[i].name,"lo")==0){
                    return NULL;
                }

                ip_deal(net_interface[i].mac,recv_msg->msg,recv_msg->msg_len,dst_ip,sockfd,net_interface[i].name);
            }
            else if(memcmp(recv_msg->msg+30,net_interface[i].ip,3) && i==num-1)
            {
                if(recv_msg->msg[33]==255){
                    return NULL;
                }
                if(strcmp(net_interface[i].name,"lo")==0){
                    return NULL;
                }
                //NODE *tmp = ip_mac_link_search_data(head,);

                ip_deal(net_interface[1].mac,recv_msg->msg,recv_msg->msg_len,dst_ip,sockfd,net_interface[1].name);
            }
        }

    }
    pthread_exit(NULL);

}