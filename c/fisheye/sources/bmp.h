/*
Module: 图像处理基本功能
Created by LiangPing &  ChenCong.
Created date: 2014/07/06
 */
#ifndef __BMP_H
#define __BMP_H

#define bool int
#define false 0
#define true 1

typedef struct
{
    unsigned int    bfSize;
    unsigned short    bfReserved1;
    unsigned short    bfReserved2;
    unsigned int    bfOffBits;
} ClBitMapFileHeader;


typedef struct
{
    unsigned int  biSize; 
    int   biWidth; 
    int   biHeight; 
    unsigned short   biPlanes; 
    unsigned short   biBitCount;
    unsigned int  biCompression; 
    unsigned int  biSizeImage; 
    int   biXPelsPerMeter; 
    int   biYPelsPerMeter; 
    unsigned int   biClrUsed; 
    unsigned int   biClrImportant; 
} ClBitMapInfoHeader;


typedef struct 
{
    unsigned char rgbBlue; //该颜色的蓝色分量
    unsigned char rgbGreen; //该颜色的绿色分量
    unsigned char rgbRed; //该颜色的红色分量
    unsigned char rgbReserved; //保留值
} ClRgbQuad;


typedef struct   //图像处理基本单元
{
    int width;
    int height;
    int channels;
    unsigned char* imageData;
}ClImage;

/*
Module: 新建一幅空图
要求：输入图片大小等参数
输出：返回生成后的图片
Created by LiangPing.
Created date: 2014/07/06
 */
ClImage*  clNewImage(int height,int width,int v,unsigned char valu);

/*
Module: 删除一幅图
要求：调用该函数后  记得对image=NULL
Created by LiangPing.
Created date: 2014/07/06
 */
void clReleaseImage(ClImage* image);

/*
Module: 载入一幅图像
要求：输入路径
输出：返回图片指针
Created by ChenCong
Created date: 2014/07/06
 */
ClImage* clLoadImage(const char* path);

/*
Module: 把3色真彩图转化为2值图
要求：输入图片
输出：返回生成后的图片
备注:bmp  被修改
Created by LiangPing  &  ChenCong.
Created date: 2014/07/06
 */
void changeToTwoValue(ClImage* bmpImg);

/*
Module: 鱼眼矫正
要求：输入图片
输出：返回生成后的图片
备注:bmpImg  没被修改
Created by LiangPing  &  ChenCong.
Created date: 2014/07/06
 */
ClImage* correctImg(const ClImage* bmgImg);

/*
Module: 保存图像
要求：输入图片
输出：返回生成后的图片
备注:bmp  被修改
Created by ChenCong.
Created date: 2014/07/06
 */
bool clSaveImage(const char * path , const ClImage * bmpImg);

#endif
