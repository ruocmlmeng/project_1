

#ifndef __lcd_H__
#define __lcd_H__

#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include "bmp.h"
#include <time.h>

#define LCD_PATH "/dev/fb0"

#define SCREEN_WIDTH  800  // 根据实际屏幕分辨率设置
#define SCREEN_HEIGHT 480

#define ITEM_WIDTH    85   // 每个方格的宽度
#define ITEM_HEIGHT   85   // 每个方格的高度
#define ITEM_NUM      4    // 网格的行列数
#define BLACK_LINE    2    // 方格之间的黑线宽度

// 起始坐标，网格从屏幕中央开始绘制
#define MATRIX_X0     ((SCREEN_WIDTH - (ITEM_NUM * ITEM_WIDTH + (ITEM_NUM - 1) * BLACK_LINE)) / 2)
#define MATRIX_Y0     ((SCREEN_HEIGHT - (ITEM_NUM * ITEM_HEIGHT + (ITEM_NUM - 1) * BLACK_LINE)) / 2)

void LCD_Init(void);
void LCD_Uninit(void);
void LCD_Draw_Point(int x,int y,int color);
void LCD_Draw_Rect(int x,int y,int w,int h,int color);

void LCD_Draw_Rectangle(int x, int y, int width, int height, int color);
void LCD_draw_grid();
void place_random_two() ;


#endif
