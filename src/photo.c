#include "photo.h"
//初始化上一次的点击时间
clickEvent last_click1 = {0,0,{0,0}};

/*
    beep_thread: 蜂鸣器线程函数
*/
void * beep_thread(void * arg)
{
    //调用蜂鸣器函数
    my_alarm();
    pthread_exit(NULL);
}

/*I
    start_beep_thread : 启动蜂鸣器的函数
*/
void start_beep_thread()
{
    pthread_t thread_id;
    pthread_create(&thread_id,NULL,beep_thread,NULL);
}

/*
    Create_Head : 创建头结点
    返回值 :
        新链表头结点的指针
*/
Head *Create_Head(void)
{
    Head *h = malloc(sizeof(Head));
    h->first = NULL;
    h->last = NULL;
    h->pic_num = 0;
    return h;
}

/*
    Add_Fnode: 添加一个数据结点
    @h:头结点的指针
    @p:要添加的数据节点指针
    返回值:
        成功返回 0
        失败返回 -1
*/
int Add_Fnode(Head *h, fnode *p)
{
    if (h == NULL || p == NULL)
    {
        return -1;
    }
    // 插入第一个结点,创建的是双向循环链表
    if (h->first == NULL)
    {
        h->first = p;
        h->last = p;
        p->next = p;
        p->prev = p;
        h->pic_num++;
    }
    else // 因为我这个不需要排序,插入图片直接往后面插就行,就是尾插
    {
        h->last->next = p;
        p->prev = h->last;
        h->last = p;
        h->last->next = h->first;
        h->first->prev = h->last;
    }
    h->pic_num++;
    return 0;
}

/*
    Search_Pics: 在一个指定的目录下面去搜索图片文件,
                每找到一个图片文件,则新建一个数据结点保存,
                并且将数据结点添加到链表中去.
    @h: 链表的头结点指针
    @paht: 指定待搜索的目录
    返回值:
        成功 0
        失败 -1
*/
int Search_Pics(Head *h, const char *path)
{
    // 1.打开文件目录文件
    DIR *fd = opendir(path);
    if (fd == NULL)
    {
        perror("open dir error");
        return -1;
    }

    // 2.读取目录项(就是读取里面内容)
    struct dirent *dirp = NULL;
    while (dirp = readdir(fd))
    {
        // 过滤掉 "." 当前目录  和 " .. "上一级目录
        if (strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0)
        {
            continue;
        }

        // 获取dirp->d_name的绝对路径
        char pathname[filename_len];
        sprintf(pathname, "%s/%s", path, dirp->d_name);
        //printf("%s\n", pathname);
        
        //判断是否为缩略图,如果是的话就过滤掉
        char *thumb_position = strstr(dirp->d_name,"_thumb");
        if(thumb_position != NULL)
        {
            continue;
        }

        // 获取文件属性,调用stat函数获取文件信息
        //  首先先定义一个struct stat * 类型的指针st
        struct stat st;
        int ret = stat(pathname, &st);
        if (ret == -1)
        {
            perror("stat error");
            return -1;
        }

        // 接下来就是判断文件的类型(bmp,jpg,png,jpge)
        // 图片都是普通文件,所有接下来判断是不是普通文件
        if (S_ISREG(st.st_mode))
        {
            // 判断是否为咱们所需要的图片文件
            fnode *p = malloc(sizeof(*p));
            p->next = p->prev = NULL;
            strcpy(p->pic_name, pathname);
            printf("%s\n", pathname);
            char *dot_position = strrchr(pathname, '.');
            if (dot_position != NULL)
            {
                // 如果找到了 '.'，将其替换为字符串终止符 '\0'
                *dot_position = '\0';
            }

            if (!strncmp(dot_position + 1, "bmp", 3))
            {
                p->pic_type = PIC_TYPE_BMP;
                sprintf(p->thumb_name, "%s_thumb.bmp", pathname); // 生成缩略图文件名

            }
            else if (!strncmp(dot_position + 1, "jpg", 3))
            {
                p->pic_type = PIC_TYPE_JPG;
            }
            else if (!strncmp(dot_position + 1, "jpeg", 4))
            {
                p->pic_type = PIC_TYPE_JPEG;
            }
            else
            {
                free(p);
                continue;
            }
            // 添加图片到结点中
            ret = Add_Fnode(h, p);
            if (ret == -1)
            {
                printf("add fnode -> %s error!\n", pathname);
                return -1;
            }
        }
        else if (S_ISDIR(st.st_mode))
        {
            // 如果打开的是目录文件就递归查找
            int r = Search_Pics(h, pathname);
            if (r == -1)
            {
                printf("Search_Pics error\n");
                return -1;
            }
        }
        else
        {
            // 如果是其他文件就跳过
            continue;
        }
    }

    // 关闭目录文件
    closedir(fd);
    return 0;
}
/*
    Destroy_Filelist : 销毁链表
    @h: 链表头结点指针
    返回值:
        无
*/
void Destroy_Filelist(Head *h)
{
    if (h == NULL)
    {
        return;
    }
    fnode *p = h->first;
    while (p)
    {
        if (h->first == h->last)
        {
            h->first = NULL;
            h->last = NULL;
            free(p);
            break;
        }
        h->first = p->next;
        p->next->prev = NULL;
        p->next = NULL;
        h->first->prev = h->last;
        h->last->next = h->first;
        free(p);
    }
    free(h);
}

/*
    Create_album : 创建相册
    返回值:
        成功 0
        失败 -1
*/
int Create_album(void)
{
    Head *h = Create_Head();
    int r = Search_Pics(h, Search_Path);
    if (r == -1)
    {
        printf("search pics error!\n");
        return -1;
    }
    fnode *p = h->first;
    while (p)
    {

        // 先创建出第一张图片
        if (p->pic_type == 0)
        {

            BMP_Display(p->pic_name, 0, 0);
        }
        // 获取开始坐标
        struct point start_pos;
        start_pos = Get_point();
        int start_x = start_pos.x;
        int start_y = start_pos.y;

        // 获取结束坐标
        struct point end_pos = Get_point();
        int end_x = end_pos.x;
        int end_y = end_pos.y;

        // 判断是否为滑动动作
        if (abs(end_x - start_x) > SWIPE_THRESHOLD || abs(end_y - start_y) > SWIPE_THRESHOLD)
        {
            enum move_dir dir = Get_Direction();
            switch (dir)
            {
            case MOV_LEFT:
                printf("左滑\n");
                p = p->next;
                break;
            case MOV_RIGHT:
                printf("右滑\n");
                p = p->prev;
                break;
            case MOV_UP:
                printf("上滑\n");
                pro_Init();
                break;
            case MOV_DOWN:
                printf("%d\n", __LINE__);
                printf("下滑\n");
                BMP_Display("/home/wangjun/photo/photoBgc5.bmp", 0, 0);
                // 显示缩略图
                display_thumbnails(h);
                // 处理用户点击，点击后显示大图
                handle_thumbnail_click(h);
                break;
            default:
                break;
            }
        }
        else
        {
            // 处理图片点击的逻辑
            if (handle_picture_click(end_x, end_y, p, h))
            {
                // 如果双击检测到了就重新加载图片
                p = h->first;
                if (p == NULL)
                {
                    return 0;
                }
                else
                {
                    BMP_Display(p->pic_name, 0, 0);
                }
            }
        }
    }
    return 0;
}

/*
    is_double_click : 用户是否双击
    @x,y:x坐标,y坐标
    返回值:
        双击 1
        非双击 0
*/
int is_double_click(int x, int y)
{
    struct timespec current_time;
    clock_gettime(CLOCK_MONOTONIC, &current_time);

    // 计算时间差
    long time_difference_ms = (current_time.tv_sec - last_click1.click_time.tv_sec) * 1000 +
                              (current_time.tv_nsec - last_click1.click_time.tv_nsec) / 1000000;

    // 设定合理的双击时间阈值（例如 500ms）
    const int DOUBLE_CLICK_TIME_THRESHOLD = 1250;
    // 设定合理的双击位置阈值（例如 20 像素）
    const int DOUBLE_CLICK_POSITION_THRESHOLD = 15;

    // 检查是否在双击的时间阈值内, 且点击位置相近
    if (time_difference_ms < DOUBLE_CLICK_TIME_THRESHOLD &&
        abs(x - last_click1.x) < DOUBLE_CLICK_POSITION_THRESHOLD &&
        abs(y - last_click1.y) < DOUBLE_CLICK_POSITION_THRESHOLD)
    {
        // 是双击，重置上次点击时间
        last_click1.click_time = (struct timespec){0, 0};
        return 1;
    }

    // 更新最后点击事件
    last_click1.x = x;
    last_click1.y = y;
    last_click1.click_time = current_time;

    // 不是双击
    return 0;
}

/*
    handle_picture_click : 处理图片点击的逻辑
    @x,y:  点击的xy轴坐标
    @p : 被点击的图片的结点
    @h : 链表的头结点
    返回值:
        双击返回 1
        单击返回 0
*/
int handle_picture_click(int x, int y, fnode *p, Head *h)
{
    if (is_double_click(x, y))
    {
        printf("双击检测到图片:%s\n", p->pic_name);

        // 删除这张图片(删除这个结点)
        Delete_Photo_Node(h, p->pic_name);

        //启动蜂鸣器
        start_beep_thread();
        return 1;
    }
    else
    {
        printf("单击检测到图片:%s\n", p->pic_name);
        return 0;
    }
}

/*
    Delete_Photo_Node : 删除双击的照片
    @h:头文件
    @picname : 要删除的图片名
    返回值:
        无
*/
void Delete_Photo_Node(Head *h, const char *picname)
{
    if (h == NULL || h->first == NULL)
    {
        return;
    }
    fnode *px = h->first;
    int flag = 0; // 0 没找到 , 1 找到了
    do
    {
        if (strcmp(px->pic_name, picname) == 0)
        {
            flag = 1;
            break;
        }
        px = px->next;
    } while (px != h->first);

    // 遍历了一遍链表,还没有找到,直接退出
    if (flag == 0)
    {
        return;
    }
    // 头删
    if (px == h->first)
    {
        // 只有一个结点的情况
        if (h->first == h->last)
        {
            h->first = h->last = NULL;
        }
        else
        {
            h->first = px->next;
            h->first->prev = h->last;
            h->last->next = h->first;
        }
    }
    else if (px == h->last) // 尾删
    {
        h->last = px->prev;
        px->prev = NULL;
        h->last->next = h->first;
        h->first->prev = h->last;
    }
    else // 中间删除
    {
        px->next->prev = px->prev;
        px->prev->next = px->next;
    }
    free(px);
    h->pic_num--;
    return;
}

/*
    display_thumbnails: 显示缩略图
    @h:链表的头结点
*/
void display_thumbnails(Head *h)
{
    fnode *p = h->first;
    int x = MARGIN, y = MARGIN;
    while (p)
    {
        // 使用预先缩小的图片文件显示缩略图
        BMP_Display(p->thumb_name, x, y);

        // 更新缩略图的显示位置
        x += THUMB_WIDTH + MARGIN;
        if (x + THUMB_WIDTH > 800)
        {
            x = MARGIN;
            y += THUMB_HEIGHT + MARGIN;
        }

        if (p == h->last)
        {
            break;
        }
        p = p->next;
    }
}
/*
    handle_thumbnail_click :用于处理用户点击缩略图的事件，
                            并显示相应的原图。
    @h: 头结点指针
    返回值:
        无
*/
void handle_thumbnail_click(Head *h)
{
    struct point click_position;
    int x, y;

    // 等待用户点击
    while (1)
    {
        click_position = Get_point();
        x = click_position.x;
        y = click_position.y;

        fnode *p = h->first;
        int cur_x = MARGIN, cur_y = MARGIN;

        while (p)
        {
            // 检查点击的位置是否在当前缩略图内
            if (x >= cur_x && x <= cur_x + THUMB_WIDTH &&
                y >= cur_y && y <= cur_y + THUMB_HEIGHT)
            {
                // 显示对应的大图
                BMP_Display(p->pic_name, 0, 0);
                enum move_dir dir = Get_Direction();
                if(dir == MOV_UP)
                {
                    printf("上滑\n");
                    pro_Init();
                }
            }

            // 更新缩略图位置
            cur_x += THUMB_WIDTH + MARGIN;
            if (cur_x + THUMB_WIDTH > SCREEN_WIDTH)
            {
                cur_x = MARGIN;
                cur_y += THUMB_HEIGHT + MARGIN;
            }

            if (p == h->last)
            {
                break;
            }
            p = p->next;
        }
    }
}
