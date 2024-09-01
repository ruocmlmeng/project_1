

#ifndef __audio_H__
#define __audio_H__
#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <signal.h>
#include <string.h>
#include <dirent.h>
#include "bmp.h"
#include "touch.h"
#include "proInit.h"
// 暂停键的x坐标
#define stop_x ((800 - 100) / 2)

//播放视频的目录文件
#define video_dir "/home/wangjun/video/"
int create_fifo();
int slave_write_cmd(int fifo_fd, char *cmd);
void play_video(const char * filepath);
void audio();

#endif