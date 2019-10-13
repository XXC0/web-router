#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

typedef struct _msg
{
    long mtype;        /*消息类型*/
    char mtext[4096];    /*消息正文*/

}MSG;

int main(void)
{
	printf("content-type:text/html\n\n");
	char *data = NULL;
	data = getenv("QUERY_STRING");
	//printf("<center><H1> <br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br> </H1></center>\n</html>");
	key_t key = ftok("/home/ubunqu/share/lot1901/04Net/web_router/cgi-bin",10);
    if(key < 0)
    	perror("");
    int msgid = msgget(key,IPC_CREAT|0666);
    if(msgid < 0)
        perror("");
    MSG msg_1,msg_2;

    msg_1.mtype = 1;
    strcpy(msg_1.mtext,data);
    msgsnd(msgid,&msg_1,sizeof(MSG)-sizeof(long),0);
    sleep(1);
    msgrcv(msgid,&msg_2,sizeof(MSG)-sizeof(long),2,0);
    printf("<center><H1>%s</H1></center>\n</html>",msg_2.mtext);

	return 0;
}