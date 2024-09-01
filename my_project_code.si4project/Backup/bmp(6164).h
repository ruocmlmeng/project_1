#ifndef __bmp_H__
#define __bmp_H__

#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#include <sys/types.h>
#include <unistd.h>

struct pic
{
	int width; // 位图宽度
	int height; // 位图高度
	short depth;	//位图深度
};
struct pic Get_Bmp_info(const char * filename);
void BMP_Display(const char * bmp_name ,int x ,int y );


void Render_BMP_to_LCD(const char *bmp_file_path);


#endif
