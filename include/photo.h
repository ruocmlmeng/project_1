

#ifndef __photo_H__
#define __photo_H__
#include <dirent.h>
#include <string.h>
#include <pthread.h>
#include "lcd.h"
#include "touch.h"
#include "bmp.h"
#include "game.h"
#include "jb.h"
#include "jpg.h"
#include "jb.h"
// 待搜索的目录(存放的是照片文件)
#define Search_Path "/home/wangjun/photo"

// 定义一个文件名的长度
#define filename_len 64

// 定义双击时间阈值 500ms
#define DOUBLE_CLICK_THRESHOLD 1000

#define SWIPE_THRESHOLD 20

#define THUMB_WIDTH 100  // 缩略图的宽度
#define THUMB_HEIGHT 100 // 缩略图的高度
#define MARGIN 10        // 缩略图之间的间距

// 定义照片类型的枚举类型,来保存不同类型的图片
enum PIC_TYPE
{
    PIC_TYPE_BMP,
    PIC_TYPE_JPG,
    PIC_TYPE_PNG,
    PIC_TYPE_GIF,
    PIC_TYPE_JPEG,
    PIC_TYPE_UNKNOW = 100 // 这个是未知的类型
};

// 图片文件结点的类型(一个图片文件包含一些什么信息)
typedef struct fnode
{
    char pic_name[filename_len];   // 文件名
    char thumb_name[filename_len]; // 缩略图名
    enum PIC_TYPE pic_type;        // 图片类型

    // 定义一个双向循环链表
    struct fnode *next;
    struct fnode *prev;
} fnode;

// 头结点类型(定义了一个带头结点的双向链表)
typedef struct head
{
    fnode *first; // 指向第一个数据结点
    fnode *last;  // 指向最后一个数据结点
    int pic_num;  // 记录图片文件个数
} Head;

// 定义一个结构体用于存储一次点击事件的信息
typedef struct clickEvent
{
    int x;                      // 点击的x坐标
    int y;                      // 点击的y坐标
    struct timespec click_time; // POSIX标准定义的结构体,秒/纳秒
} clickEvent;

Head *Create_Head(void);
int Add_Fnode(Head *h, fnode *p);
int Search_Pics(Head *h, const char *path);
void Destroy_Filelist(Head *h);
int Create_album(void);
int is_double_click(int x, int y);
int handle_picture_click(int x, int y, fnode *p, Head *h);
void Delete_Photo_Node(Head *h, const char *picname);
void display_thumbnails(Head *h);
void handle_thumbnail_click(Head *h);
void *beep_thread(void *arg);
void start_beep_thread();
#endif