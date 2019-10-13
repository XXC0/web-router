#ifndef KEY_PTHREAD_H
#define KEY_PTHREAD_H

#include "main.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
int msgid;

typedef struct _msg
{
    long mtype;        /*消息类型*/
    char mtext[4096];    /*消息正文*/
}MSG;

MSG msg_1,msg_2;


extern void *key_cmd(void *arg);
extern void _key_cmd(char *cmd,char *msg);
extern void *web_to_set_router(void *arg);
extern void filter_ip();


#endif