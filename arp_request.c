#include "arp_request.h"
extern NODE *head ;
extern IPNODE *fi_head;

void* arp_request(void *arg)
{
	while(1)
    {
    	unsigned char msg_[1024]={
        0xff,0xff,0xff,0xff,0xff,0xff,
        0x00,0x0c,0x29,0xf3,0xc5,0xb1, 
        0x08,0x06,
        0x00,0x01,
        0x08,0x00,
        0x06,
        0x04,
        0x00,0x01,
        0x00,0x0c,0x29,0xf3,0xc5,0xb1,
        192,168,1,254,
        0x00,0x00,0x00,0x00,0x00,0x00,
        192,168,1
	    } ;
	    struct sockaddr_ll sll;
	    struct ifreq ethreq;
	    strncpy(ethreq.ifr_name,"ens33",IFNAMSIZ);
	    ioctl(sockfd,SIOCGIFINDEX,(char*)&ethreq);
	    bzero(&sll,sizeof(sll));
	    sll.sll_ifindex = ethreq.ifr_ifindex;
	    for (int i = 0; i < 255; i++)
	    {
	        msg_[41] = i;
	        sendto(sockfd,msg_,42,0,(struct sockaddr*)&sll,sizeof(sll));
	        
	    }
	    bzero(&ethreq,sizeof(ethreq));
	    strncpy(ethreq.ifr_name,"ens36",IFNAMSIZ);
	    ioctl(sockfd,SIOCGIFINDEX,(char*)&ethreq);
	    bzero(&sll,sizeof(sll));
	    sll.sll_ifindex = ethreq.ifr_ifindex;
	    for (int i = 0; i < 255; i++)
	    {
	    	msg_[8] = 0xbb;
	    	msg_[25] = 0xbb;
	    	msg_[28] = 2;
	    	msg_[40] = 2;	
	        msg_[41] = i;
	        sendto(sockfd,msg_,42,0,(struct sockaddr*)&sll,sizeof(sll));
	        
	    }
	    sleep(60);
	}
}