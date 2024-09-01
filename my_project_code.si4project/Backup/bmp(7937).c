#include "bmp.h"
#include "lcd.h"
int BMP_Load(const char *file_name, BMP_Image *image) {
    FILE *file = fopen(file_name, "rb");
    if (!file) {
        return -1;
    }

    fread(&image->file_header, sizeof(BMP_File_Header), 1, file);
    fread(&image->info_header, sizeof(BMP_Info_Header), 1, file);

    // 验证 image_size，如果为0则计算图像大小
    if (image->info_header.image_size == 0) {
        int bytes_per_pixel = 3; // 假设24位图像
        image->info_header.image_size = image->info_header.width * image->info_header.height * bytes_per_pixel;
    }

    image->data = malloc(image->info_header.image_size);
    if (!image->data) {
        fclose(file);
        return -1;
    }

    fseek(file, image->file_header.offset_data, SEEK_SET);
    fread(image->data, image->info_header.image_size, 1, file);

    fclose(file);

    image->width = image->info_header.width;
    image->height = image->info_header.height;

    return 0;
}


void BMP_Free(BMP_Image *image) {
    if (image->data) {
        free(image->data);
        image->data = NULL;
    }
}


int BMP_GetPixel(BMP_Image *image, int x, int y) {
    if (x < 0 || x >= image->width || y < 0 || y >= image->height || image->data == NULL) {
        return -1;  // 返回错误，坐标超出范围或数据指针为空
    }

    int bytes_per_pixel = 3;
    int row_size = image->width * bytes_per_pixel;
    int pixel_offset = (image->height - 1 - y) * row_size + x * bytes_per_pixel;

    if (pixel_offset >= image->info_header.image_size) {
        return -1; // 确保偏移量不会超出图像数据的范围
    }

    unsigned char blue = image->data[pixel_offset];
    unsigned char green = image->data[pixel_offset + 1];
    unsigned char red = image->data[pixel_offset + 2];

    int color = (red << 16) | (green << 8) | blue;
    return color;
}



void LCD_Draw_BMP(int x, int y, BMP_Image *image) {
    for (int j = 0; j < image->height; j++) {
        for (int i = 0; i < image->width; i++) {
            int color = BMP_GetPixel(image, i, j);  // 获取每个像素的颜色
            if (color != -1) {  // 确保获取到的像素颜色有效
                LCD_Draw_Point(x + i, y + j, color);  // 在LCD上绘制该像素
            } else {
                // 错误处理，必要时可以输出错误日志或执行其他操作
            }
        }
    }
}


