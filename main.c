#include "main.h"
#include "ip_pthread.h"
#include "arp_request.h"
#include "key_pthread.h"
#include "web_http.h"



NODE *head = NULL;
IPNODE *fi_head = NULL;
struct bufferevent *bufev = NULL;

int setip_hwip(char *ip,char *getway,char *net_name)
{
    struct ifreq temp;
    struct sockaddr_in *addr;
    int fd = 0;
    strcpy(temp.ifr_name,net_name);
    if((fd=socket(AF_INET, SOCK_STREAM, 0))<0)
    {
      return -1;
    }
    addr = (struct sockaddr_in *)&(temp.ifr_addr);
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = inet_addr(ip);
    if(ioctl(fd, SIOCSIFADDR, &temp))
    {
        return -1;
    }

    addr = (struct sockaddr_in *)&(temp.ifr_dstaddr);
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = inet_addr(getway);
    if(ioctl(fd, SIOCGIFDSTADDR, &temp))
    {
        return -1;
    }
    close(fd);
    return 0;
}



void setip_getway()
{
    FILE *fp;
    fp = fopen("./setip_getway.txt","r");
    if (!fp)
    {
        perror("");
        return ;
    }
    while(1)
    {

        char buf[128]="";
        char ip[16]="";
        char getway[16]="";
        char net_name[32]="";
        bzero(buf,sizeof(buf));
        if(fgets(buf,sizeof(buf),fp)==NULL)//读取一行
            break;
        //buf[strlen(buf)]=0;
        if (strstr(buf,"ens33"))
        {
            buf[strlen(buf)-1]=0;
            sscanf(buf,"%[^:]:%[^:]:%[^:]",net_name,ip,getway);
            setip_hwip(ip,getway,net_name);
        }
        else if ((strstr(buf,"ens36")))
        {
            sscanf(buf,"%[^:]:%[^:]:%[^:]",net_name,ip,getway);
            setip_hwip(ip,getway,net_name);
        }
    }
    
    ip_link_print(fi_head);
    fclose(fp);
}

int main(int argc, char const *argv[])
{
    //setip_getway();
    char *path = getenv("PWD");
    char mydir[1024]="";
    strcpy(mydir,path);
    strcat(mydir,"/web-http");
    chdir(mydir);
	getinterface();
	filter_ip();
	ip_mac_create_link_header(&head);
	sockfd = socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
    if (sockfd < 0)
    {
        perror("socket");
        exit(-1);
    }
    pthread_t pthid[5];
	pthread_create(&pthid[1],NULL,arp_request,NULL);
	pthread_detach(pthid[1]);
	pthread_create(&pthid[2],NULL,key_cmd,NULL);
	pthread_detach(pthid[2]);
	pthread_create(&pthid[3],NULL,web_server,NULL);
	pthread_detach(pthid[3]);
    pthread_create(&pthid[4],NULL,web_to_set_router,NULL);
    pthread_detach(pthid[4]);
	unsigned char recv_msg[1024] = "";
	while(1)
	{
		struct sockaddr_in client;
    	socklen_t len = sizeof(client);
        struct ether_header *ether_info = NULL;
    	unsigned  short ethernet_type;
        bzero(recv_msg,sizeof(recv_msg));
        int ret = recvfrom(sockfd,recv_msg,sizeof(recv_msg),0,(struct sockaddr*)&client,&len);
        ether_info = (struct ether_header*)recv_msg;
    	ethernet_type = ntohs(ether_info->ether_type);
    	if (ethernet_type == 0x0800)//IP
    	{
    		RECV_DATA *recv = (RECV_DATA *)malloc(sizeof(RECV_DATA));
			recv->msg_len = ret;
			memcpy(recv->msg, recv_msg, ret);
    		pthread_create(&pthid[0],NULL,ip_packet,(void *)recv);
    		pthread_detach(pthid[0]);
    	}
    	else if (ethernet_type == 0x0806)
    	{
    		NODE *new = (NODE *)malloc(sizeof(NODE));
    		memcpy(new->ip,recv_msg+28,4);
    		memcpy(new->mac,recv_msg+22,6);	        	
            if (ip_mac_link_search_data(head,new->ip) == NULL)
        	{
        		ip_mac_link_insert_header(head,new);
        	}
    	}
	}

	return 0;
}
