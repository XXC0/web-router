#include "key_pthread.h"
#include "web_http.h"

extern NODE *head ;
extern IPNODE *fi_head;
extern struct bufferevent *bufev;
char ipc_cmd[128];
char ipc_msg[128];

void help()
{
	printf("=------------------------=\n");
	printf("=---setip:192.168.1.4----=\n");
	printf("=---delip:192.168.2.4----=\n");
	printf("=---saveset--------------=\n");
	printf("=---showip---------------=\n"); 
	printf("=---showarp--------------=\n");
	printf("=------------------------=\n");
	if (bufev == NULL)
	{
		return ; 
	}
	char buf[2048]="";
	int len = sprintf(buf,
			"=------------------------=<br>"
			"=---setip:192.168.1.4----=<br>"
			"=---delip:192.168.2.4----=<br>"
			"=---saveset--------------=<br>"
			"=---showip---------------=<br>" 
			"=---showarp--------------=<br>"
			"=------------------------=<br>");
	bufferevent_write(bufev,buf,len);

}
void setip(char *msg)
{

	int ip;
	inet_pton(AF_INET, msg, &ip);
	unsigned char ip_msg[4]="";
	memcpy(ip_msg,&ip,4);
	IPNODE *new = (IPNODE *)malloc(sizeof(IPNODE));
	memcpy(new->fi_ip,ip_msg,4);
	ip_link_insert_header(fi_head,new);		
	ip_link_print(fi_head);
}


void delip(char *msg)
{

	int ip;
	inet_pton(AF_INET, msg, &ip);
	unsigned char ip_msg[4]="";
	memcpy(ip_msg,&ip,4);
	if((ip_msg[3]==0)||(ip_msg[3]==255)||(ip_msg[0]==0)){
		return ;
	}
	ip_link_delete(fi_head,ip_msg);
	ip_link_print(fi_head);
}

void save_ip_link()
{
	FILE *filter_ip = fopen("filter_ip.txt","wb+");
	if(filter_ip == NULL){
		perror("!!!configure file,in main.c");
		_exit(1);
	}
	char buff[20]="";
	IPNODE *pb=fi_head;
	while(pb!=NULL){
		sprintf(buff,"%d.%d.%d.%d\n",pb->fi_ip[0],pb->fi_ip[1],pb->fi_ip[2],pb->fi_ip[3]);
		fputs(buff,filter_ip);
		pb = pb->next;
	}
	fclose(filter_ip);
}

void *key_cmd(void *arg)
{
	printf("please input help to view helpdocument\n");
	while(1)
	{
		char buf[256]="";
		char tmp[256]="";
		char msg[128]="";
		fgets(tmp,sizeof(tmp),stdin);
		tmp[strlen(tmp)-1] = 0;
		sscanf(tmp,"%[^:]:%s",buf,msg);
		if (strcmp(buf,"exit") ==0 )
		{
			ip_link_destroy(fi_head);
			ip_mac_link_destroy(head);
			break;
		}
		else if (strcmp(buf,"showip") ==0)
		{
			ip_link_print(fi_head);
		}
		else if (strcmp(buf,"showarp") ==0)
		{
			ip_mac_link_print(head);
		}
		else if (strcmp(buf,"help") ==0)
		{
			help();
		}
		else if (strcmp(buf,"setip") ==0)
		{
			setip(msg);
		}
		else if (strcmp(buf,"delip") ==0)
		{
			delip(msg);
		}
		else if (strcmp(buf,"saveip") ==0)
		{
			save_ip_link();
		}
	}
	pthread_exit(NULL);
}


void filter_ip()
{
	ip_create_link_header(&fi_head);
	FILE *fp;
	fp = fopen("./filter_ip.txt","r");
	if (!fp)
	{
		perror("");
		return ;
	}
	while(1)
	{

		char buf[16]="";
		int ip;
		bzero(buf,sizeof(buf));
		if(fgets(buf,sizeof(buf),fp)==NULL)//读取一行
			break;
		buf[strlen(buf)-1]=0;
		inet_pton(AF_INET, buf, &ip);
		IPNODE *new = (IPNODE *)malloc(sizeof(IPNODE));
		memcpy(new->fi_ip,&ip,4);
		ip_link_insert_header(fi_head,new);
	}
	
	ip_link_print(fi_head);
	fclose(fp);
}

void ip_link_print__(IPNODE *head)
{
	if(head == NULL)
		return;
	IPNODE *pcur = head->next;
	int len =0;
	while(pcur != NULL)
	{

		char ip[32]="";
		len = sprintf(ip,"%d.%d.%d.%d<br>", pcur->fi_ip[0],pcur->fi_ip[1],pcur->fi_ip[2],pcur->fi_ip[3]);
		bufferevent_write(bufev,ip,len);
		pcur=pcur->next;
		
	}
}

void ip_mac_link_print__(NODE *head)
{
	if(head == NULL)
		return;
	NODE *pcur = head->next;
	int len =0;
	while(pcur != NULL)
	{
		char buf[256]="";
		len = sprintf(buf,"ip %d.%d.%d.%d,mac %02x:%02x:%02x:%02x:%02x:%02x<br>",pcur->ip[0],pcur->ip[1],pcur->ip[2],pcur->ip[3],
			pcur->mac[0],pcur->mac[1],pcur->mac[2],pcur->mac[3],pcur->mac[4],pcur->mac[5]);
		bufferevent_write(bufev,buf,len);
		pcur=pcur->next;
		
	}
}

void _key_cmd(char *cmd,char *msg)
{

	if (strcmp(cmd,"exit") ==0 )
	{
		ip_link_destroy(fi_head);
		ip_mac_link_destroy(head);
	}
	else if (strcmp(cmd,"showip") ==0)
	{
		ip_link_print__(fi_head);
	}
	else if (strcmp(cmd,"showarp") ==0)
	{
		ip_mac_link_print__(head);
	}
	else if (strcmp(cmd,"help") ==0)
	{
		help();
	}
	else if (strcmp(cmd,"setip") ==0)
	{
		setip(msg);
	}
	else if (strcmp(cmd,"delip") ==0)
	{
		delip(msg);
	}
	else if (strcmp(cmd,"saveset") ==0)
	{
		save_ip_link();
	}

}






void _ip_link_print__(IPNODE *head)
{

    if(head == NULL)
        return;
    IPNODE *pcur = head->next;
    char temp[4096]="";
    while(pcur != NULL)
    {

        char ip[32]="";
        sprintf(ip,"%d.%d.%d.%d<br>\n", pcur->fi_ip[0],pcur->fi_ip[1],pcur->fi_ip[2],pcur->fi_ip[3]);
        strcat(temp,ip);
        pcur=pcur->next;
        
    }
    msg_1.mtype = 2;
    memcpy(msg_1.mtext,temp,strlen(temp));
    msgsnd(msgid,&msg_1,sizeof(MSG)-sizeof(long),0);
}

void _ip_mac_link_print__(NODE *head)
{
    if(head == NULL)
        return;
    NODE *pcur = head->next;
    char temp[4096]="";
    while(pcur != NULL)
    {
        char buf[256]="";
        sprintf(buf,"ip %d.%d.%d.%d,mac %02x:%02x:%02x:%02x:%02x:%02x<br>\n",pcur->ip[0],pcur->ip[1],pcur->ip[2],pcur->ip[3],
            pcur->mac[0],pcur->mac[1],pcur->mac[2],pcur->mac[3],pcur->mac[4],pcur->mac[5]);
	    strcat(temp,buf);	
        pcur=pcur->next;
        
    }
    msg_1.mtype = 2;
    memcpy(msg_1.mtext,temp,strlen(temp));
    msgsnd(msgid,&msg_1,sizeof(MSG)-sizeof(long),0);
}

void help_()
{
    char buf[2048]="";
    int len = sprintf(buf,
            "=------------------------=<br>"
            "=---setip:192.168.1.4----=<br>"
            "=---delip:192.168.2.4----=<br>"
            "=---saveset--------------=<br>"
            "=---showip---------------=<br>" 
            "=---showarp--------------=<br>"
            "=------------------------=<br>");
    msg_1.mtype = 2;
    memcpy(msg_1.mtext,buf,len);
    msgsnd(msgid,&msg_1,sizeof(MSG)-sizeof(long),0);

}


int setip_hwip_(char *ip,char *getway,char *net_name)
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


void *web_to_set_router(void *arg)
{
    key_t key = ftok("/home/ubunqu/share/lot1901/04Net/web_router/cgi-bin",10);
    if(key < 0)
        perror("1");
    msgid = msgget(key,IPC_CREAT|0666);
    if(msgid < 0)
        perror("2");
    msg_1.mtype = 2;

    while(1)
    {
        msgrcv(msgid,&msg_2,sizeof(MSG)-sizeof(long),1,0);
        printf("[%s]\n",msg_2.mtext );
        sscanf(msg_2.mtext,"%[^:]:%s",ipc_cmd,ipc_msg);
        if (strcmp(ipc_cmd,"showip") ==0)
        {

            _ip_link_print__(fi_head);
        }
        else if (strcmp(ipc_cmd,"showarp") ==0)
        {
            _ip_mac_link_print__(head);

        }
        else if (strcmp(ipc_cmd,"help") ==0)
        {
            help_();
        }
        else if (strcmp(ipc_cmd,"setip") ==0)
        {
            setip(ipc_msg);
        }
        else if (strcmp(ipc_cmd,"delip") ==0)
        {
            delip(ipc_msg);
        }
        else if (strcmp(ipc_cmd,"saveip") ==0)
        {
            save_ip_link();
        }
        else if (strcmp(ipc_cmd,"setip_getway") ==0)
        {
        	char ip[16]="";
	        char getway[16]="";
	        char net_name[32]="";
            sscanf(ipc_msg,"%[^:]:%[^:]:%[^:]",net_name,ip,getway);
            setip_hwip_(ip,getway,net_name);
        }


    }
    pthread_exit(NULL);
    
}