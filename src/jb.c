#include "jb.h"

/*
    alarm : 警报器,警报响5s 循环响3次
*/
void my_alarm()
{
    int fd = open("/sys/kernel/gec_ctrl/beep", O_RDWR); // 打开所有的LED文件
    if (fd == -1)
    {
        printf("sorry , open beep failed\n");
        return;
    }

    int k;
    printf("----------------------------\n");

    while (1)
    {
        // 蜂鸣器叫或者灯亮就往该文件里写入1
        k = 1;
        write(fd, &k, 4); // 写入1

        printf("please down enter\n");
        //getchar();
        sleep(1);
        k = 0;
        write(fd, &k, 4);
        printf("please down enter\n");

        //getchar();
        break;
    }

    close(fd);
}