
#ifndef __touch_H__
#define __touch_H__

#define TS_PATH "/dev/input/event0"

#include <stdio.h>
#include <stdlib.h>
#include<linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define MOVE_THRESHOLD 30

enum move_dir
{
	MOV_UP = 1,
	MOV_DOWN,
	MOV_LEFT,
	MOV_RIGHT
};


struct point
{
	int x;
	int y;
};


struct point Get_point(void);
enum move_dir  Get_Direction(void);


#endif
