

#ifndef __game_H__
#define __game_H__
#include"lcd.h"
#include"bmp.h"
#include"touch.h"

//处理滑动方向的逻辑
void handle_swipe() ;

//生成新的随机数字(2或4)
void place_random_number();

//判断是否还能移动
int can_move();

//判断游戏是否结束
int check_game_over();  

//显示指定位置的数字图片
void display_number(int row, int col);

//判断是否可以合并
int can_merge(int old_x, int old_y, int new_x, int new_y, int horizontal);

//清除指定位置的数字图片
void Clear_BMP_Display(int x, int y);

//上下左右移动
int move_left();
int move_right();
int move_up();
int move_down();


#endif
