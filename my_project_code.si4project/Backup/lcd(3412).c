#include "lcd.h"
#include "bmp.h"
//定义全局变量
int * plcd = NULL;
int fd_lcd = -1;

/*
	LCD_Init:初始化显示屏硬件设备
*/
void LCD_Init(void)
{
	//1.打开显示屏设备文件
	fd_lcd = open(LCD_PATH, O_RDWR);
	if(fd_lcd == -1)
	{
		perror("open lcd error");
		return ;
	}

	//2. 映射mmap
    plcd = mmap(NULL,800*480 *4, PROT_READ|PROT_WRITE, 
       					MAP_SHARED,fd_lcd, 0);
	if(plcd == MAP_FAILED)
	{
		perror("map lcd error");
		return ;
	}

}
/*
	LCD_Uinit: 释放屏幕相关资源
	
*/
void LCD_Uninit(void)
{
	//1.解映射
	 munmap(plcd, 800*480*4);

	//2.关闭屏幕文件
	close(fd_lcd);
}

/*
	LCD_Draw_Point: 在开发板屏幕上任意的位置
		画一个任意颜色的点.
	@x,y: 点的坐标
	@color: 颜色值
	返回值:无
*/
void LCD_Draw_Point(int x,int y,int color)
{
	if(x >= 0 && x < 800 && y>=0 && y <480)
	{
		*(plcd + 800*y +x) = color;
	}
}
/*
	LCD_Draw_Rect : 在屏幕上画一个任意大小,颜色的矩形
	@x,y :左上角点坐标
	@w, h:矩形宽和高
	@color:矩形颜色
	返回值:无
*/
void LCD_Draw_Rect(int x,int y,int w,int h,int color)
{
	int i,j;
	for(j = y; j < y + h; j++)
	{
		for(i = x; i < x + w;i++)
		{
			LCD_Draw_Point(i, j,color);
		}
	}
}