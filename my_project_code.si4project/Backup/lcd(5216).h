

#ifndef __lcd_H__
#define __lcd_H__

#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>


#define LCD_PATH "/dev/fb0"

void LCD_Init(void);
void LCD_Uninit(void);
void LCD_Draw_Point(int x,int y,int color);
void LCD_Draw_Rect(int x,int y,int w,int h,int color);



#endif
