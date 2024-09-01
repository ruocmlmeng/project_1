#include <stdio.h>
#include <stdlib.h>

#include "jconfig.h"
#include "jerror.h"
#include "jmorecfg.h"
#include "jpeglib.h"

#include "lcd.h"

/*
	JPG_Display: 显示一张jpg/jpeg格式图片
	@x0, y0 : 图片左上角点的坐标
	@jpg_name : 图片的文件名
	返回值: 
		无
*/
void JPG_Display(const char * jpg_name, int x0, int y0)
{
	//1. 分配并初始化一个jpeg解压对象
		//struct jpeg_decompress_struct 这个结构体在libjpeg这个库中，
		//是用来保存解压一个jpeg文件所需要的信息的。
		//struct jpeg_error_mgr 这个结构体在libjpeg这个库中
		//是用来保存解压或压缩过程的一些出错信息的。

		struct jpeg_decompress_struct dinfo; //声明一个解压的对象
		struct jpeg_error_mgr jerr; //声明一个出错信息的对象
		
		jpeg_create_decompress(&dinfo); //初始化dinfo这个解压对象
		dinfo.err = jpeg_std_error(&jerr); //初始化这个出错对象
	
	//2. 指定要解压缩的图像文件
		//用标准IO去打开这个文件
		FILE *infile;
		infile = fopen(jpg_name , "r");
		if (infile == NULL)
		{
			perror("fopen jpg error");
			return ;
		}
		
		jpeg_stdio_src(&dinfo, infile); //指定要解压的图像文件
		
					
	//3.调用jpeg_read_header()获取图像信息
		jpeg_read_header(&dinfo, TRUE);
			
	//4.用于设置jpeg解压对象dinfo的一些参数。可采用默认参数
		//(省略)
	
	//5. 调用jpeg_start_decompress()启动解压过程
		jpeg_start_decompress(&dinfo);
			
	//6. 读取一行或者多行扫描线上数据并处理，通常的代码是这样子的:
		unsigned char *buffer = malloc(dinfo.output_width * dinfo.output_components);
		
		//dinfo.output_scanline , 表示的意思是，已经扫描了多少行
		while (dinfo.output_scanline < dinfo.output_height)
		{
				
			jpeg_read_scanlines(&dinfo,  //解压对象
								&buffer,//保存解压后数据的二级指针, 
								1 //读取多少行数据来解压
								);	//dinfo.output_scanline + 1
								
			//每扫描一次 buffer里面就保存了一行的像素点的数据
			int i;
			unsigned char a, r , g ,b; //扫描后的数据以大端模式存储的
			unsigned char * p = buffer;
			int color;
			for(i = 0; i < dinfo.output_width; i++)
			{
				if(dinfo.output_components == 4)
				{
					a = *p++;
				}
				else 
				{
					a = 0;
				}
				r = *p++;
				g = *p++;
				b = *p++;
				color = a<<24|r<<16|g<<8|b;
				LCD_Draw_Point(x0 + i, y0 + dinfo.output_scanline , color );
			}
		}
			
	
	//7. 调用jpeg_finish_decompress(&dinfo)完成解压过程
		jpeg_finish_decompress(&dinfo);
	
	//8. 调用jpeg_destroy_decompress释放jpeg解压对象dinfo
		jpeg_destroy_decompress(&dinfo);
		fclose(infile);
}