#ifndef WEB_HHTP_H
#define WEB_HHTP_H

#include "main.h"
#include <event.h>
#include <event2/listener.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "pub.h"
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#define PORT 8080


// void writecb(struct bufferevent *bufev, void *arg);
// void eventcb(struct bufferevent *bufev, short events, void *arg);
// void readcb(struct bufferevent *bufev, void *arg);
// void listen_cb(struct evconnlistener *evl, evutil_socket_t fd, struct sockaddr *cliaddr, int socklen, void *ptr);
// void signal_cb(evutil_socket_t sig, short events, void *user_data);
// void send_header(struct bufferevent *bufev, int code,char *info,char *file_type,int length);
// void del_msg(char *msg , struct bufferevent *bufev);
// void send_info(struct bufferevent *bufev, char * filepath);
extern void* web_server(void *arg);

#endif