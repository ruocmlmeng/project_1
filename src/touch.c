#include "touch.h"
struct point Get_point(void)
{
    struct point p;
    p.x = -1;
    p.y = -1;
    // 1.打开触摸屏文件
    int fd_ts;
    fd_ts = open(TS_PATH, O_RDWR);
    if (fd_ts == -1)
    {
        perror("open error");
        return p;
    }
    // 2.获取点的信息
    struct input_event ts;
    while (1)
    {
        int ret = read(fd_ts, &ts, sizeof(ts));
        if (ret != sizeof(ts))
        {
            // 没有读到结构体大小的内容
            continue;
        }
        // 解析获取到的结构体信息
        if (ts.type == EV_ABS && ts.code == ABS_X)
        {
            p.x = ts.value * 800 / 1024;
        }
        if (ts.type == EV_ABS && ts.code == ABS_Y)
        {
            p.y = ts.value * 480 / 600;
        }

        // 当x,y坐标值都获取到了,结束循环
        if (p.x >= 0 && p.y >= 0)
        {
            break;
        }
    }
    // 3.关闭触摸屏文件,并且退出程序
    close(fd_ts);
    return p;
}

enum move_dir Get_Direction(void)
{
    int x0, y0; // 起始点的坐标
    int x1, y1; // 终止点的坐标
    int flag_x = 0;
    int flag_y = 0;
    // 1.打开触摸屏的文件
    int fd_ts;
    fd_ts = open(TS_PATH, O_RDWR);
    if (fd_ts == -1)
    {
        perror("open ts error");
        return -1;
    }
    // 2.获取输入信息(获取两个坐标点的数值)
    struct input_event ts;
    while (1)
    {
        int ret = read(fd_ts, &ts, sizeof(ts));
        if (ret != sizeof(ts))
        {
            // 没有读到结构体大小的内容
            continue;
        }
        // 解析获取到的结构体信息
        if (ts.type == EV_ABS && ts.code == ABS_X)
        {
            if (flag_x == 0)
            {
                x0 = ts.value;
                flag_x = 1;
            }
            x1 = ts.value;
        }
        if (ts.type == EV_ABS && ts.code == ABS_Y)
        {
            if (flag_y == 0)
            {
                y0 = ts.value;
                flag_y = 1;
            }
            y1 = ts.value;
        }

        // 判断手指离开屏幕
        if ((ts.type == EV_KEY && ts.code == BTN_TOUCH && ts.value == 0) || (ts.type == EV_ABS && ts.code == ABS_PRESSURE && ts.value == 0))
        {
            break;
        }
    }

    // 3.判定手指滑动的方向
    enum move_dir dir = -1;
    int det_x = abs(x0 - x1);
    int det_y = abs(y0 - y1);
    if (det_x > det_y)
    {
        // 左右滑动
        // 左右滑动
        if (det_x >= MOVE_THRESHOLD)
        {
            if (x0 < x1)
            {
                dir = MOV_RIGHT;
            }
            else
            {
                dir = MOV_LEFT;
            }
        }
    }
    else
    {
        // 上下滑动
        if (det_y >= MOVE_THRESHOLD)
        {
            if (y0 < y1)
            {
                dir = MOV_DOWN;
            }
            else
            {
                dir = MOV_UP;
            }
        }
    }
    // 4.关闭文件,退出函数
    close(fd_ts);
    return dir;
}
