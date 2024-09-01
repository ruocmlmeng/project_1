#include"lcd.h"
#include "touch.h"
#include "bmp.h"
#include <string.h>
#include "game.h"

int main(int argc,char *argv[])
{
	//初始化硬件设备
	LCD_Init();

	BMP_Display("/home/wangjun/pic/star.bmp", 0, 0);
	BMP_Display("/home/wangjun/pic/btn_start.bmp", 550, 120);
	    // 游戏循环
    while (1) 
	{
        // 显示当前游戏状态，具体展示界面的代码取决于你的开发环境和库
        // 例如，可以在控制台输出或通过LCD显示


		struct point p;
		p = Get_point();
		printf("x = %d , y = %d \n",p.x,p.y);
		if((p.x >= 550 && p.x <=755) && (p.y>=120 && p.y <=180) )
		{
			/*BMP_Display("/home/wangjun/pic/board.bmp`", 0, 0);
			BMP_Display("/home/wangjun/pic/2.bmp", 35, 30);*/
			BMP_Display("/home/wangjun/pic/bgc.bmp", 0, 0);
			LCD_draw_grid();
			// 初始化网格并生成两个随机数字
		    place_random_number();
		    place_random_number();
			//place_random_two();
		}

		

		// 获取用户输入的滑动方向
		enum move_dir dir = Get_Direction();
        // 调用处理滑动的逻辑函数
        handle_swipe(dir);


        // 判断游戏状态
        if (check_game_over()) 
		{
            break;  // 游戏结束，退出循环
        }
    }

    printf("游戏结束\n");
	//释放硬件设备
	LCD_Uninit();
	return 0;
}
