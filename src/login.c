#include "login.h"

/*
    login_proInit : 登录初始化界面
    返回值:
        解锁成功 0
        解锁失败 -1
*/
int login_proInit()
{
    int input_password[3] = {0}; // 用于存储用户点击的三个数字
    int index = 0;               // 用于记录当前输入的位置
    int stored_password[3];      // 用于存储从文件中读取的密码
    // 用于文件操作
    FILE *password_file = fopen("/home/wangjun/password.txt", "r");
    if (password_file == NULL)
    {
        perror("open password file error");
        return -1;
    }
    // 读取存储的密码
    fscanf(password_file, "%d %d %d", &stored_password[0], &stored_password[1], &stored_password[2]);
    fclose(password_file);
    // 点击屏幕输入数字
    while (index < 3)
    {
        // 获取屏幕迪点击的坐标
        struct point p = Get_point();
        int x = p.x;
        int y = p.y;
        if ((x >= 230 && x <= 320) && (y >= 130 && y <= 200))
        {
            // 7
            input_password[index] = 7;
            printf("7\n");
        }
        if ((x >= 365 && x <= 455) && (y >= 130 && y <= 200))
        {
            // 8
            input_password[index] = 8;
            printf("8\n");
        }
        if ((x >= 500 && x <= 590) && (y >= 130 && y <= 200))
        {
            // 9
            input_password[index] = 9;
            printf("9\n");
        }

        if ((x >= 230 && x <= 320) && (y >= 245 && y <= 315))
        {
            // 4
            input_password[index] = 4;
            printf("4\n");
        }
        if ((x >= 365 && x <= 455) && (y >= 245 && y <= 315))
        {
            // 5
            input_password[index] = 5;
            printf("5\n");
        }
        if ((x >= 500 && x <= 590) && (y >= 245 && y <= 315))
        {
            // 6
            input_password[index] = 6;
            printf("6\n");
        }

        if ((x >= 230 && x <= 320) && (y >= 360 && y <= 430))
        {
            // 1
            input_password[index] = 1;
            printf("1\n");
        }
        if ((x >= 365 && x <= 455) && (y >= 360 && y <= 430))
        {
            // 2
            input_password[index] = 2;
            printf("2\n");
        }
        if ((x >= 500 && x <= 590) && (y >= 360 && y <= 430))
        {
            // 3
            input_password[index] = 3;
            printf("3\n");
        }

        if (index == 0)
        {
            BMP_number(index, input_password[0], 230, 32);
        }
        if (index == 1)
        {
            BMP_number(index, input_password[1], 362, 32);
        }
        if (index == 2)
        {
            BMP_number(index, input_password[2], 496, 32);
        }
        index++;
    }

    if (input_password[0] == stored_password[0] &&
        input_password[1] == stored_password[1] &&
        input_password[2] == stored_password[2])
    {
        return 0; // 密码正确
    }
    else
    {
        int i = 0;
        for (i = 0; i < sizeof(input_password[0]) / sizeof(input_password); i++)
        {
            input_password[i] = 0;
        }
        char empty_name[32];
        sprintf(empty_name,"%s/qingkong.bmp",path_pass);
        BMP_Display(empty_name,230, 32);
        BMP_Display(empty_name,362, 32);
        BMP_Display(empty_name, 496, 32);
        sleep(1);
        return -1; // 密码错误
    }
}
/*
    BMP_number: 将点击的数字图片画到相应的位置
    @index : 下标
    @num: 数组中对应下标的值
    @x,y: 图片要放入的位置
    返回值:
        无
*/
void BMP_number(int index, int num, int x, int y)
{
    char filename[32];
    sprintf(filename, "%s/%d_num.bmp", path_pass, num);
    BMP_Display(filename, x, y);
}