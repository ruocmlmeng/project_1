#include"lcd.h"
#include "bmp.h"

int main()
{
	//初始化硬件设备
	LCD_Init();


	int x,y,w,h,color;
	scanf("%d%d%d%d%x",&x,&y,&w,&h,&color);
	LCD_Draw_Rect(x, y, w,  h, color);

    // 加载BMP图片
    BMP_Image bg_image;
    if (BMP_Load("/home/wangjun/pic/Init_board.jpg", &bg_image) == 0) {
        // 显示图片
        LCD_Draw_BMP(0, 0, &bg_image);
    } else {
        printf("Failed to load background image\n");
    }

	//释放硬件设备
	LCD_Uninit();
	return 0;
}
