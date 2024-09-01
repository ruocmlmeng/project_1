#ifndef __bmp_H__
#define __bmp_H__
#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>


// BMP 文件头结构
typedef struct {
    unsigned short file_type;     // 文件类型，必须是 'BM'
    unsigned int file_size;       // 文件大小，以字节为单位
    unsigned short reserved1;     // 保留，必须设置为0
    unsigned short reserved2;     // 保留，必须设置为0
    unsigned int offset_data;     // 像素数据的偏移量（相对于文件头的起始位置）
} BMP_File_Header;

// BMP 信息头结构
typedef struct {
    unsigned int size;            // 信息头大小（40字节）
    int width;                    // 图像宽度
    int height;                   // 图像高度
    unsigned short planes;        // 必须设置为1
    unsigned short bit_count;     // 每个像素的位数（1, 4, 8, 16, 24, 或 32）
    unsigned int compression;     // 压缩方式（0 表示不压缩）
    unsigned int image_size;      // 图像大小（以字节为单位）
    int x_pixels_per_meter;       // 水平分辨率（像素/米）
    int y_pixels_per_meter;       // 垂直分辨率（像素/米）
    unsigned int colors_used;     // 颜色表中颜色数
    unsigned int colors_important;// 重要的颜色数
} BMP_Info_Header;

// BMP 图像结构
typedef struct {
    BMP_File_Header file_header;   // BMP文件头
    BMP_Info_Header info_header;   // BMP信息头
    unsigned char *data;           // 图片像素数据
    int width;                     // 图片的宽度
    int height;                    // 图片的高度
} BMP_Image;

// 函数声明
int BMP_Load(const char *file_name, BMP_Image *image);  // 加载 BMP 图片
void BMP_Free(BMP_Image *image);                        // 释放 BMP 图片数据
int BMP_GetPixel(BMP_Image *image, int x, int y);       // 获取指定坐标处的像素颜色
void LCD_Draw_BMP(int x, int y, BMP_Image *image);      // 在LCD上绘制 BMP 图片



#endif
