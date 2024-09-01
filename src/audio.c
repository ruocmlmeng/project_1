

#include "audio.h"

// 创建管道文件
int create_fifo()
{
    // 检测管道文件是否存在，不存在就创建
    int ret = access("./fifo", F_OK);
    {
        if (ret == 0)
        {
            printf("fifo has exist\n");
        }
        else if (ret == -1)
        {
            mkfifo("./fifo", 0777); // 创建管道文件
        }
    }

    int fifo_fd = open("./fifo", O_RDWR); // 打开管道文件
    if (fifo_fd == -1)
    {
        perror("open fifo failed");
        return -1;
    }

    return fifo_fd;
}

// 把slave命令写入管道文件
int slave_write_cmd(int fifo_fd, char *cmd)
{
    write(fifo_fd, cmd, strlen(cmd));
    return 0;
}


/*
    play_video : 播放视频
    @filepanth : 视频播放的路径
    返回值 : 
            无
*/
void play_video(const char * filepath)
{

            // 播放视频
            int ret = execl("/usr/bin/mplayer",
                            "/usr/bin/mplayer",
                            "-slave",
                            "-quiet",
                            "-ao", "oss",                                    // 使用 OSS 音频驱动
                            "-vfm", "ffmpeg",                                // 使用 FFmpeg 视频解码器
                            "-lavdopts", "lowres=1:fast:skiploopfilter=all", // 解码优化选项
                            "-cache", "16384",                               // 设置缓存大小
                            "-autosync", "30",                               // 自动同步选项
                            "-msglevel",
                            "all=4",                                          // 输出所有日志信息
                            "-input", "file=./fifo",
                            "-zoom", "-x", "700", "-y", "380",
                            "-geometry", "0:0",
                            filepath,                                       //要播放的文件的路径
                            NULL);

            if (ret == -1)
            {
                perror("errno execlp");
                // exit(1);
                exit(EXIT_FAILURE);
            }
}

// 视频播放功能
void audio()
{
    // 显示主界面

    BMP_Display("/home/wangjun/pics/bgc.bmp", 0, 0);
    // 调节音量
    BMP_Display("/home/wangjun/pics/V.bmp", 0, 380);
    // 回退几秒
    BMP_Display("/home/wangjun/pics/back.bmp", stop_x - 10 - 10 - 100 - 100, 380);
    // 上一个视频
    BMP_Display("/home/wangjun/pics/pre.bmp", stop_x - 10 - 100, 380);
    // 暂停播放
    BMP_Display("/home/wangjun/pics/stop.bmp", stop_x, 380);
    // 下一个视频
    BMP_Display("/home/wangjun/pics/next.bmp", stop_x + 10 + 100, 380);
    // 快进几秒
    BMP_Display("/home/wangjun/pics/quck.bmp", stop_x + 10 + 10 + 100 + 100, 380);
    //退回到主界面
    BMP_Display("/home/wangjun/pics/backInit.bmp", 700, 0);

    //获取目录文件里面的视频文件
    struct dirent ** namelist;
    int n = scandir(video_dir, &namelist, NULL, alphasort);
    if(n < 0)
    {
        perror("scandir error");
        return ;
    }
    //存放mp4文件的字符串数组的下标
    int current_index = 0;

    //找第一个 .mp4 文件
    while(current_index < n && !(strstr(namelist[current_index]->d_name, ".mp4") || strstr(namelist[current_index]->d_name, ".avi")))
    {
        current_index++;
    }

    // 创建管道文件并获取文件描述符
    int fifo_fd = create_fifo();
    int video_stop_cont_flags = 1;

    int play_flag = 0;
    pid_t pid;
    pid = fork(); // 创建进程

    if (pid == 0) // 子进程
    {
        while (1)
        {
            // 设置初始音量和取消静音
            //slave_write_cmd(fifo_fd, "volume 50 1\n");
            //slave_write_cmd(fifo_fd, "mute 0\n");

            // 播放视频
            char filename[128];
            sprintf(filename,"%s%s",video_dir,namelist[current_index]->d_name);
            play_video(filename);
            // 视频播放完毕，重新开始
            sleep(1); // 添加短暂延时，避免 CPU 占用过高
        }
    }

    while (1) // 父进程
    {
        // 获取触摸屏坐标
        struct point p = Get_point();
        int x = p.x;
        int y = p.y;

        // 判断触摸点的坐标范围，执行相应的功能

        // 暂停/继续播放
        if (x >= stop_x && x <= stop_x + 100 && y >= 380 && y <= 480)
        {
            if (play_flag == 0)
            {
                BMP_Display("/home/wangjun/pics/start.bmp", stop_x, 380);
                kill(pid, SIGSTOP); // 子进程暂停
                play_flag = 1;
            }
            else
            {
                BMP_Display("/home/wangjun/pics/stop.bmp", stop_x, 380);
                kill(pid, SIGCONT); // 子进程继续
                play_flag = 0;
            }
        }

        // 视频退出
        if (x >= 700 && x <= 800 && y >= 0 && y <= 100)
        {
            kill(pid, SIGKILL); // 杀死当前子进程
            close(fifo_fd);     // 关闭管道文件
            pro_Init();          // 回到初始化界面
            break;              // 退出循环，结束程序
        }

        // 快进视频
        if (x >= stop_x + 10 + 10 + 100 + 100 && x <= stop_x + 10 + 10 + 200 + 100 && y >= 380 && y <= 480)
        {
            slave_write_cmd( fifo_fd, "seek +5\n");
        }

        // 回退视频
        if (x >= stop_x - 10 - 10 - 100 - 100 && x <= stop_x - 20 && y >= 380 && y <= 480)
        {
            slave_write_cmd(fifo_fd, "seek -5\n");
        }

        // 上一个视频（预留的功能）
        if (x >= stop_x - 10 - 100 && x <= stop_x - 10 && y >= 380 && y <= 480)
        {
            do
            {
                current_index = (current_index - 1 + n) % n;
            } while (!(strstr(namelist[current_index]->d_name, ".mp4") || strstr(namelist[current_index]->d_name, ".avi")));
            //杀死当前播放视频的子进程
            kill(pid,SIGKILL); 
            //创建播放上一个视频的子进程
            pid = fork();
            if( pid == 0)
            {
                char prev_filename[512];
                sprintf(prev_filename,"%s%s",video_dir,namelist[current_index]->d_name);
                play_video(prev_filename);
            }
        }

        // 下一个视频（预留的功能）
        if (x >= stop_x + 10 + 100 && x <= stop_x + 110 + 100 && y >= 380 && y <= 480)
        {
            // 执行下一个视频的功能代码
            do
            {
                current_index = (current_index + 1) % n;
            }while(!(strstr(namelist[current_index]->d_name, ".mp4") || strstr(namelist[current_index]->d_name, ".avi")));
            //杀死当前播放视频的子进程
            kill(pid,SIGKILL); 
            //创建播放上一个视频的子进程
            pid = fork();
            if(pid == 0)
            {
                char next_filename[512];
                sprintf(next_filename,"%s%s",video_dir,namelist[current_index]->d_name);
                play_video(next_filename);
            }
        }

        // 增加音量
        if (x >= 0 && x <= 100 && y >= 380 && y <= 480)
        {
            slave_write_cmd(fifo_fd, "volume +200\n");
        }

        // 减少音量
        if (x >= 0 && x <= 100 && y >= 380 && y <= 480)
        {
            slave_write_cmd(fifo_fd, "volume -200\n");
        }

        // 静音/取消静音
        if (x >= stop_x + 100 && x <= stop_x + 200 && y >= 380 && y <= 480)
        {
            slave_write_cmd(fifo_fd, "mute\n");
        }
    }
}
