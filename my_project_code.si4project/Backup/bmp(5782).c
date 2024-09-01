#include "bmp.h"
#include "lcd.h"
/*
	Get_Bmp_info:获取图片的宽度, 高度, 色深
*/
struct pic Get_Bmp_info(const char * filename)
{
	struct pic p;
	int fd_dmp = open(filename,O_RDWR);
	if(fd_dmp == -1)
	{
		perror("open error");
		return p;
	}
	//求位图的宽度
	char buf[4];
	lseek(fd_dmp,0x12,SEEK_SET);
	int r = read(fd_dmp,buf,4);
	if(r == -1)
	{
		perror("read width error");
		return p;
	}
	p.width = (buf[3] << 24) | (buf[2] << 16) | (buf[1]<<8) | buf[0];

	//求位图的高度
	lseek(fd_dmp,0x16,SEEK_SET);
	r = read(fd_dmp,buf,4);
	if(r == -1)
	{
		perror("read height error");
		return p;
	}
	p.height = (buf[3] << 24) | (buf[2] << 16) | (buf[1]<<8) | buf[0];

	//求位图的色深
	lseek(fd_dmp,0x1c,SEEK_SET);
	r = read(fd_dmp,buf,2);
	if(r == -1)
	{
		perror("read depth error");
		return p;
	}
	p.depth = (buf[1]<<8) |buf[0];
	printf("图片宽度:%d , 高度:%d , 色深:%d\n",p.width,p.height,p.depth);

	//3.关闭图片文件
	close(fd_dmp);
	return p;
}
/*
	BMP_Display:显示一张BMP格式的图片
	@x0,y0 :  图片左上角点的坐标,为了确定图片显示位置
	@bmp_name:
	返回值:
		无
*/
void BMP_Display(const char * bmp_name, int x0 , int y0)
{
	//1.获取图片文件的信息(宽度、高度、色深)
	struct pic info = Get_Bmp_info(bmp_name);

	//2.计算像素数组的实际大小 
	//一行实际的字节数: 
	int line = abs(info.width) * info.depth/8 ;
	int laizi = 0; 
	if(line % 4)
    {
        laizi = 4 - line%4;
    }
	//一行的总字节数: 一行实际的字节数(line) + 赖子(laizi)						
	//实际大小
	int size = (line + laizi) * abs(info.height);

	//3. 开辟像素数组大小的内存空间，读取像素数组的内容
	unsigned char * p = malloc(size);
	
	//将光标偏移到像素数组的位置
	int fd_bmp = open(bmp_name, O_RDWR);
	if(fd_bmp == -1)
	{
		perror("open bmp error");
		return ;
	}
	lseek(fd_bmp, 54 , SEEK_SET );
	int ret = read(fd_bmp, p , size);
	if(ret == -1)
    {
		perror("read bmp error");
		return ;
    }

	//4.解析每一个像素点的颜色值，并且在对应的坐标位置画点
	unsigned char a, r, g ,b;
	int color;
	int i, j;
	int k = 0;
	for(j = 0;  j < abs(info.height) ; j++)
    {
        for(i = 0; i < abs(info.width); i++)
        {
            //b = *(p+k);k++;
            b = p[k++];
            g = p[k++];
            r = p[k++];
            if(info.depth == 32)
                  a = p[k++];
            else 
                  a = 0;
            color = a<<24 | r<<16 | g<<8 | b;
            LCD_Draw_Point( 
                (info.width > 0) ? (x0 + i):(x0 + abs(info.width)-1 -i), 
                (info.height > 0)? (y0 + abs(info.height)-1 - j) : (y0 + j), 
                color);
        }
        //过滤掉 赖子
        k += laizi;
    }

    //5. 释放空间，关闭文件
    free(p);
    close(fd_bmp);
}



/*
	Render_BMP_to_LCD:显示一张BMP格式的图片
	@x0,y0 :  图片左上角点的坐标,为了确定图片显示位置
*/
void Render_BMP_to_LCD(const char *bmp_file_path) 
{
    int fd_bmp;
    int width, height, depth;
    unsigned char bmp_header[54]; // BMP文件头
    unsigned char *p; // 指向像素数组的指针
    int line, laizi, size;
    int r, i, j, k;
    unsigned char a, r_c, g, b;
    int color;

    // 打开BMP文件
    fd_bmp = open(bmp_file_path, O_RDONLY);
    if (fd_bmp < 0) 
	{
        perror("Failed to open BMP file");
        return;
    }

    // 读取BMP文件头
    r = read(fd_bmp, bmp_header, 54);
    if (r != 54) 
	{
        perror("Failed to read BMP header");
        close(fd_bmp);
        return;
    }

    // 从BMP文件头中提取宽度、高度和颜色深度
    width = *(int *)&bmp_header[18];
    height = *(int *)&bmp_header[22];
    depth = *(short *)&bmp_header[28];

    // 计算每行的实际字节数和癞子
    line = abs(width) * depth / 8;
    laizi = (line % 4 == 0) ? 0 : (4 - line % 4);
    size = (line + laizi) * abs(height);

    // 为像素数组分配内存
    p = malloc(size);
    if (p == NULL) {
        perror("Failed to allocate memory for BMP pixel array");
        close(fd_bmp);
        return;
    }

    // 将光标偏移到像素数组的位置，并读取像素数据
    lseek(fd_bmp, 54, SEEK_SET);
    r = read(fd_bmp, p, size);
    if (r != size) {
        perror("Failed to read BMP pixel array");
        free(p);
        close(fd_bmp);
        return;
    }

    // 解析每一个像素点的颜色值，并在对应的坐标位置画点
    k = 0;
    for (j = 0; j < abs(height); j++) 
	{
        for (i = 0; i < abs(width); i++) 
		{
            b = p[k++];
            g = p[k++];
            r_c = p[k++];
            if (depth == 32) {
                a = p[k++];
            } else {
                a = 0;
            }
            color = (a << 24) | (r_c << 16) | (g << 8) | b;

            // 注意：在某些情况下，(width, height) 坐标系的原点可能在左下角，需要调整
            LCD_Draw_Point(i, abs(height) - 1 - j, color);
        }
        k += laizi; // 跳过癞子
    }

    // 释放内存并关闭文件
    free(p);
    close(fd_bmp);
}

