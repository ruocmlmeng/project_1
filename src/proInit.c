#include "proInit.h"

void * beep_thread_pass(void * arg)
{
    my_alarm();
    pthread_exit(NULL);
}

void start_beep_thread_pass()
{
    pthread_t tid;
    pthread_create(&tid,NULL,beep_thread_pass,NULL);
}

/*
    pro_Init : 初始化功能盒子的界面
    返回值:
        无
*/
void pro_Init(void)
{

    // 初始化硬件设备
    LCD_Init();
    // 锁屏界面
    BMP_Display("/home/wangjun/pics/suoping2.bmp", 0, 0);
    //记录输入错误的次数
    int num = 0;
    while (1)
    {


        if(!login_proInit())
        {
            break;
        }
        else
        {
            //密码输入三次错误就会蜂鸣
            num++;
        }

        if(num == 3)
        {
            //三次密码错误触发蜂鸣器
            start_beep_thread_pass();
        }
    }

    // 初始化界面
    // 背景图
    BMP_Display("/home/wangjun/initpic/topbgc.bmp", 0, 0);
    // 象棋对对碰游戏app
    BMP_Display("/home/wangjun/initpic/game1.bmp", MATRIX_X0, MATRIX_Y0);
    // 视频播放器app
    BMP_Display("/home/wangjun/initpic/mplay.bmp", MATRIX_X0 * 2, MATRIX_Y0);
    // 警报/灯光系统app
    BMP_Display("/home/wangjun/initpic/jb.bmp", MATRIX_X0, MATRIX_Y0 * 4);
    // 回忆录
    BMP_Display("/home/wangjun/initpic/photo.bmp", MATRIX_X0 * 2, MATRIX_Y0 * 4);

    while (1)
    {
        // 获取开发板坐标,进入不同的模块
        struct point p;
        p = Get_point();
        int x = p.x;
        int y = p.y;
        printf("x ==%d , y ==%d\n", x, y);
        // 安全警报系统
        if ((x >= MATRIX_X0 && x <= MATRIX_X0 + 100) && (y >= MATRIX_Y0 * 4 && y <= MATRIX_Y0 * 4 + 100))
        {
            // 警报系统
            my_alarm();
        }

        // 象棋对对碰
        if ((x >= MATRIX_X0 && x <= MATRIX_X0 + 100) && (y >= MATRIX_Y0 && y <= MATRIX_Y0 + 100))
        {
            BMP_Display("/home/wangjun/pics/star_new.bmp", 0, 0);
            BMP_Display("/home/wangjun/pics/begingame.bmp", MATRIX_X0 + 25, 200);
            BMP_Display("/home/wangjun/pics/exitgame.bmp", MATRIX_X0 + 25, 300);
            while (1)
            {
                struct point p1;
                p1 = Get_point();
                int x0 = p1.x;
                int y0 = p1.y;
                // 开始游戏
                if ((x0 >= MATRIX_X0 + 25 && x0 <= MATRIX_X0 + 300) && (y0 >= 200 && y0 <= 300))
                {

                    BMP_Display("/home/wangjun/pics/xq3.bmp", 0, 0);
                    BMP_Display("/home/wangjun/pics/zjm_btn.bmp", 0, 0);
                    BMP_Display("/home/wangjun/pics/sx_btn.bmp", 736, 0);
                    LCD_draw_grid();
                    // 初始化网格并生成两个随机数字
                    place_random_number();
                    place_random_number();
                    break;
                }
                // 退出游戏
                if ((x0 >= MATRIX_X0 + 25 && x0 <= MATRIX_X0 + 300) && (y0 >= 400 && y0 <= 480))
                {
                    // 回到初始化页面
                    pro_Init();
                }
            }
            // 游戏循环
            int game_over = 0;
            while (!game_over)
            {
                struct point p;
                p = Get_point();
                if ((p.x >= 0 && p.x <= 64) && (p.y >= 0 && p.y <= 64))
                {
                    goto_home_page();
                }
                if ((p.x >= 736 && p.x <= 800) && (p.y >= 0 && p.y <= 64))
                {
                    start_new_game();
                }
                // 获取用户输入的滑动方向
                enum move_dir dir = Get_Direction();

                // 调用处理滑动的逻辑函数
                handle_swipe(dir);

                // 判断游戏状态
                if (check_game_over())
                {
                    display_game_over();
                    // game_over = 1;//结束当前游戏循环
                }
            }
        }

        // 回忆录
        if ((x >= MATRIX_X0 * 2 && x <= MATRIX_X0 * 2 + 100) && (y >= MATRIX_Y0 * 4 && y <= MATRIX_Y0 * 4 + 100))
        {
            Create_album();
        }

        // 音频播放
        if ((x >= MATRIX_X0 * 2 && x <= MATRIX_X0 * 2 + 100) && (y >= MATRIX_Y0 && y <= MATRIX_Y0 + 100))
        {
            audio();
        }
    }

    // 释放硬件设备
    LCD_Uninit();
}
