#include"lcd.h"
#include "touch.h"
#include "bmp.h"
#include <string.h>
#include "game.h"
#include <pthread.h>
#include <signal.h>
#include "proInit.h"

#define file_path "/home/wangjun/pic/fhcq.mp3"
pthread_t tid;
//标志位
int flag = 0;
//播放音乐的线程
void *play_mp3_thread(void * arg)
{
	if(flag == 1)
	{
		pthread_kill(tid,SIGKILL);
	}
	char command[256];
	if(flag == 0)
	{
		flag = 1;
		sprintf(command,"madplay -Q %s",file_path);

	}


	
	system(command);
	pthread_exit(NULL);
	return NULL;
}

int main()
{
	//初始化界面
	pro_Init();
	
	return 0;
}
