#include "lcd.h"

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

// 这个函数来绘制矩形框，给定左上角坐标和宽高
void LCD_Draw_Rectangle(int x, int y, int width, int height, int color)
{
    int i, j;

    // 遍历矩形区域内的所有像素点
    for (i = 0; i < height; i++) 
	{         // 高度方向遍历
        for (j = 0; j < width; j++) 
		{      // 宽度方向遍历
            // 绘制当前像素点
            LCD_Draw_Point(x + j, y + i, color);
        }
    }
}

// 函数：绘制4x4的网格
void LCD_draw_grid() 
{
    int i, j;
    int x0, y0;
    
    for (i = 0; i < ITEM_NUM; i++) 
	{
        for (j = 0; j < ITEM_NUM; j++) 
		{
            // 计算每个方格的左上角坐标
            x0 = MATRIX_X0 + j * (ITEM_WIDTH + BLACK_LINE);
            y0 = MATRIX_Y0 + i * (ITEM_HEIGHT + BLACK_LINE);
            
            // 绘制方格 (用颜色0x000000绘制黑色边框)
            LCD_Draw_Rectangle(x0, y0, ITEM_WIDTH, ITEM_HEIGHT, 0x000000);
        }
    }
}
// 假设这是矩阵的定义，全为0表示空网格
int matrix_2048[ITEM_NUM][ITEM_NUM] = {0};
// 初始化随机数生成器并在随机空位置放置数字 "2"
void place_random_two() 
{
    int empty_positions[ITEM_NUM * ITEM_NUM][2];  // 用于存储所有空位置的坐标
    int empty_count = 0;

    // 遍历矩阵，找到所有空位置
    for (int i = 0; i < ITEM_NUM; i++)
	{
        for (int j = 0; j < ITEM_NUM; j++)
		{
            if (matrix_2048[i][j] == 0) 
			{
                empty_positions[empty_count][0] = i;  // 记录空位置的行号
                empty_positions[empty_count][1] = j;  // 记录空位置的列号
                empty_count++;
            }
        }
    }

    if (empty_count > 0) 
	{
        // 随机选择一个空位置
        srand(time(NULL));
        int random_index = rand() % empty_count;
        int x = empty_positions[random_index][0];
        int y = empty_positions[random_index][1];

        // 在随机选中的空位置放置数字 "2"
        int new_number = (rand() % 2 == 0) ? 2 : 4;
		matrix_2048[x][y] = new_number;

        // 根据坐标计算位置，并显示数字图片
        int x0 = MATRIX_X0 + (ITEM_WIDTH + BLACK_LINE) * y ;
        int y0 = MATRIX_Y0 + (ITEM_HEIGHT + BLACK_LINE) * x ;
		// 显示对应数字的图片
        char pathname[32];
        sprintf(pathname, "/home/wangjun/pic/%d.bmp", new_number);  // 生成图片文件名，如 "2.bmp" 或 "4.bmp"
        BMP_Display(pathname, x0, y0);  // 假设数字2的图片为 "2.bmp"
    }
}