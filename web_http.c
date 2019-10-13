#include "web_http.h"
#include "key_pthread.h"


extern struct bufferevent *bufev;
char key_command[7][128]={
					"exit",
					"showip",
					"showarp",
					"help",
					"setip",
					"delip",
					"saveset"
};

void writecb(struct bufferevent *bufev, void *arg);
void eventcb(struct bufferevent *bufev, short events, void *arg);
void readcb(struct bufferevent *bufev, void *arg);
void listen_cb(struct evconnlistener *evl, evutil_socket_t fd, struct sockaddr *cliaddr, int socklen, void *ptr);
void signal_cb(evutil_socket_t sig, short events, void *user_data);
void send_header(struct bufferevent *bufev, int code,char *info,char *file_type,int length);
void del_msg(char *msg , struct bufferevent *bufev);
void send_info(struct bufferevent *bufev, char * filepath);

void* web_server(void *arg)
{
	// char *path = getenv("PWD");
 //    char mydir[256]="";
 //    strcpy(mydir,path);
 //    strcat(mydir,"/web-http");
 //    chdir(mydir);
	struct event_base *base = NULL;
	//struct event *sigev = NULL;
	struct evconnlistener *listener = NULL;
	struct sockaddr_in ser_addr;
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(PORT);
	ser_addr.sin_addr.s_addr = htons(0);
	base = event_base_new();
	listener = evconnlistener_new_bind(base,listen_cb, (void *)base, LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE,-1,
	(struct sockaddr*)&ser_addr,sizeof(ser_addr));
	//sigev = evsignal_new(base,SIGINT,signal_cb,(void *)base); 
	//event_add(sigev,NULL);
	event_base_dispatch(base);
	evconnlistener_free(listener);
	//event_free(sigev);
	event_base_free(base);
	return 0;
}

void eventcb(struct bufferevent *bufev, short events, void *arg)
{
	if (events & BEV_EVENT_EOF) {
		printf("Connection closed.\n");
	} else if (events & BEV_EVENT_ERROR) {
		printf("Got an error on the connection: %s\n",
		    strerror(errno));/*XXX win32*/
	}
	/* None of the other events can happen here, since we haven't enabled
	 * timeouts */
	bufferevent_free(bufev);
}

void writecb(struct bufferevent *bufev, void *arg)
{
	struct evbuffer *output = bufferevent_get_output(bufev);
	if (evbuffer_get_length(output) == 0) {
		printf("flushed answer\n");
		//bufferevent_free(bev);
	}
}

void readcb(struct bufferevent *bufev, void *arg)
{
	char msg[1024]="";
	//int ret = bufferevent_read(bufev,msg,sizeof(msg));
	bufferevent_read(bufev,msg,sizeof(msg));
	//printf("%s\n",msg);
	del_msg(msg,bufev);
	//bufferevent_write(bufev,msg,ret);
}

void listen_cb(struct evconnlistener *evl, evutil_socket_t fd, struct sockaddr *cliaddr, int socklen, void *ptr)
{
	struct event_base *base = (struct event_base *)ptr;
	// struct bufferevent *bufev = NULL;
	bufev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
	bufferevent_setcb(bufev,readcb, writecb,eventcb, NULL);
	bufferevent_enable(bufev, EV_WRITE);
	bufferevent_enable(bufev, EV_READ);
}
// void signal_cb(evutil_socket_t sig, short events, void *user_data)
// {
// 	struct event_base *base = user_data;
// 	struct timeval delay = { 2, 0 };

// 	printf("Caught an interrupt signal; exiting cleanly in two seconds.\n");

// 	event_base_loopexit(base, &delay);
// }
void del_msg(char *msg , struct bufferevent *bufev)
{
    char method[20]="";
    char data[128]="";
    sscanf(msg,"%[^ ] %[^ ]",method,data);
    if (strcasecmp(method,"get") == 0)
    {
        char *request = data+1;
        strdecode(request,data+1);
        char *file_type = get_mime_type(request);

        if (*request == 0)
        {
            request = "./";
        }
        struct stat s;
        if (stat(request,&s)== -1)
        {
        	char cmd[256]="";
			char msg[128]="";
			sscanf(request,"%[^:]:%s",cmd,msg);
			for (int i = 0; i < 7; ++i)
			{
				if (strcmp(key_command[i],cmd)== 0)
				{
					send_header(bufev,404,"Not Found",get_mime_type("*.html"),0);
					send_info(bufev,"dir_header.html");
					_key_cmd(cmd,msg);
					send_info(bufev,"dir_tail.html");
					break;
				}
				else if (strcmp(key_command[i],cmd) && i==6)
				{
					send_header(bufev,404,"Not Found",get_mime_type("*.html"),0);
            		send_info(bufev,"error.html");
				}
			}
        	// send_header(bufev,404,"Not Found",get_mime_type("*.html"),0);
         //    send_info(bufev,"error.html");
            
        }
        else
        {
            if(S_ISREG(s.st_mode))
            {

            	send_header(bufev,200,"OK",file_type,s.st_size);
                send_info(bufev,request);

            }
            else if(S_ISDIR(s.st_mode))//目录
            {   

                send_header(bufev,200, "OK",get_mime_type("*.html"),0);
                send_info(bufev,"dir_header.html");
                struct dirent **list=NULL;
                int ndir = scandir(request,&list,NULL,alphasort);
                int i=0;
                for(i=0;i<ndir;i++)
                {
                    char listbuf[256]="";
                    int n =0;
                    if(list[i]->d_type == DT_REG)
                    {
                        n = sprintf(listbuf,"<li><a href=%s>%s</a></li>",list[i]->d_name,list[i]->d_name);
                    }
                    else if(list[i]->d_type == DT_DIR)
                    {

                        n = sprintf(listbuf,"<li><a href=%s/>%s/</a></li>",list[i]->d_name,list[i]->d_name);
                    }
                    bufferevent_write(bufev,listbuf,n);
                    free(list[i]);
                }
                free(list);
                char buf[256]="";
                int len = sprintf(buf,"<li><a href=help>help</a></li>");
                bufferevent_write(bufev,buf,len);
                send_info(bufev,"dir_tail.html");
            }
        }
     }
 }

void send_header(struct bufferevent *bufev, int code,char *info,char *file_type,int length)
 {
	//状态行
	char buf[1024]="";
	int len =0;
	len = sprintf(buf,"HTTP/1.1 %d %s\r\n",code,info);
	bufferevent_write(bufev,buf,len);
	//消息头
	len = sprintf(buf,"Content-Type:%s\r\n",file_type);
	bufferevent_write(bufev,buf,len);
	if(length > 0)
	{
		len = sprintf(buf,"Content-Length:%d\r\n",length);
		bufferevent_write(bufev,buf,len);
	}
	//空行
	bufferevent_write(bufev,"\r\n",2);

 }

void send_info(struct bufferevent *bufev, char * filepath)
{
	int fd = open(filepath,O_RDONLY);
	if(fd < 0)
	{
		perror("");
		return ;
	}
	char buf[1024]="";
	while(1)
	{
		int count = read(fd,buf,sizeof(buf));
		if(count <=0)
		{
			break;
		}
		bufferevent_write(bufev,buf,count);
	}
	close(fd);
}