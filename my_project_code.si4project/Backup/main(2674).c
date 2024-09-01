#include"lcd.h"
#include "touch.h"
#include "bmp.h"
#include <string.h>


int main(int argc,char *argv[])
{
	//初始化硬件设备
	LCD_Init();

	BMP_Display("/home/wangjun/pic/star.bmp", 0, 0);
	BMP_Display("/home/wangjun/pic/btn_start.bmp", 550, 120);
	
	while(1)
	{
		struct point p;
		p = Get_point();
		printf("x = %d , y = %d ",p.x,p.y);
		
		enum move_dir dir = Get_Direction();
		switch(dir)
		{
			case MOV_LEFT:
					printf("左\n");
					break;
			case MOV_UP:
					printf("上\n");
					break;
			case MOV_DOWN:
					printf("下\n");
					break;
			case MOV_RIGHT:
					printf("右\n");
					break;
		}
		if((p.x >= 550 && p.x <=755) && (p.y>=120 && p.y <=180) )
		{
			/*BMP_Display("/home/wangjun/pic/board.bmp", 0, 0);
			BMP_Display("/home/wangjun/pic/2.bmp", 35, 30);*/
			BMP_Display("/home/wangjun/pic/bgc.bmp", 0, 0);
			LCD_draw_grid();
			place_random_two();
		}
	}
	
		
	


	//释放硬件设备
	LCD_Uninit();
	return 0;
}
