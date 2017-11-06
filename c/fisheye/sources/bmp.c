/*
Module: 图像处理基本功能
Created by LiangPing &  ChenCong.
Created date: 2014/07/06
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "bmp.h"

/*
Module: 新建一幅空图
要求：输入图片大小等参数
输出：返回生成后的图片
Created by LiangPing.
Created date: 2014/07/06
 */
ClImage* clNewImage(int height,int width,int v,unsigned char valu)
{
    int i,j,k;

    ClImage* bmpImg = (ClImage*)malloc(sizeof(ClImage));
    bmpImg->width = width;
    bmpImg->height = height;
    bmpImg->channels = v;
    bmpImg->imageData = (unsigned char*)malloc(sizeof(unsigned char)*width*height*v);


    for ( i = 0;i < height; i++)
    {
	for ( j = 0;j < width; j++)
	{
	    for ( k = 0; k< 3;k ++)
	    {
		bmpImg->imageData[i*bmpImg->width*3+3*j + k]=valu;
	    }
	}
    }

    return bmpImg;
}

/*
Module: 删除一幅图
要求：调用该函数后  记得对image=NULL
Created by LiangPing.
Created date: 2014/07/06
 */
void clReleaseImage(ClImage* image)
{
    if(image != NULL){  
		free(image->imageData);
		free(image);
    }
}

/*
Module: 载入一幅图像
要求：输入路径
输出：返回图片指针
Created by ChenCong
Created date: 2014/07/06
 */
ClImage* clLoadImage(const char* path)
{
    ClImage* bmpImg;
    FILE* pFile;
    unsigned short fileType;
    ClBitMapFileHeader bmpFileHeader;
    ClBitMapInfoHeader bmpInfoHeader;
    int channels = 1;
    int width = 0;
    int height = 0;
    int step = 0;
    int offset = 0;
    unsigned char pixVal;
    ClRgbQuad* quad;
    int i, j, k;

    bmpImg = (ClImage*)malloc(sizeof(ClImage));
    pFile = fopen(path, "rb");
    if (!pFile) {
		free(bmpImg);
		return NULL;
    }

    fread(&fileType, sizeof(unsigned short), 1, pFile);
    if (fileType == 0x4D42) {
	//printf("bmp file! \n");

	fread(&bmpFileHeader, sizeof(ClBitMapFileHeader), 1, pFile);
	/*printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n");
	  printf("bmp文件头信息：\n");
	  printf("文件大小：%d \n", bmpFileHeader.bfSize);
	  printf("保留字：%d \n", bmpFileHeader.bfReserved1);
	  printf("保留字：%d \n", bmpFileHeader.bfReserved2);
	  printf("位图数据偏移字节数：%d \n", bmpFileHeader.bfOffBits);*/

	fread(&bmpInfoHeader, sizeof(ClBitMapInfoHeader), 1, pFile);
	/*printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n");
	  printf("bmp文件信息头\n");
	  printf("结构体长度：%d \n", bmpInfoHeader.biSize);
	  printf("位图宽度：%d \n", bmpInfoHeader.biWidth);
	  printf("位图高度：%d \n", bmpInfoHeader.biHeight);
	  printf("位图平面数：%d \n", bmpInfoHeader.biPlanes);
	  printf("颜色位数：%d \n", bmpInfoHeader.biBitCount);
	  printf("压缩方式：%d \n", bmpInfoHeader.biCompression);
	  printf("实际位图数据占用的字节数：%d \n", bmpInfoHeader.biSizeImage);
	  printf("X方向分辨率：%d \n", bmpInfoHeader.biXPelsPerMeter);
	  printf("Y方向分辨率：%d \n", bmpInfoHeader.biYPelsPerMeter);
	  printf("使用的颜色数：%d \n", bmpInfoHeader.biClrUsed);
	  printf("重要颜色数：%d \n", bmpInfoHeader.biClrImportant);
	  printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n");*/

	if (bmpInfoHeader.biBitCount == 8)
	{
	    //printf("该文件有调色板，即该位图为非真彩色\n\n");
	    channels = 1;
	    width = bmpInfoHeader.biWidth;
	    height = bmpInfoHeader.biHeight;
	    offset = (channels*width)%4;
	    if (offset != 0)
	    {
		offset = 4 - offset;
	    }
	    //bmpImg->mat = kzCreateMat(height, width, 1, 0);
	    bmpImg->width = width;
	    bmpImg->height = height;
	    bmpImg->channels = 1;
	    bmpImg->imageData = (unsigned char*)malloc(sizeof(unsigned char)*width*height);
	    step = channels*width;

	    quad = (ClRgbQuad*)malloc(sizeof(ClRgbQuad)*256);
	    fread(quad, sizeof(ClRgbQuad), 256, pFile);
	    free(quad);

	    for (i=0; i<height; i++)
	    {
		for (j=0; j<width; j++)
		{
		    fread(&pixVal, sizeof(unsigned char), 1, pFile);
		    bmpImg->imageData[(height-1-i)*step+j] = pixVal;
		}
		if (offset != 0)
		{
		    for (j=0; j<offset; j++)
		    {
			fread(&pixVal, sizeof(unsigned char), 1, pFile);
		    }
		}
	    }			
	}
	else if (bmpInfoHeader.biBitCount == 24)
	{
	    //printf("该位图为位真彩色\n\n");
	    channels = 3;
	    width = bmpInfoHeader.biWidth;
	    height = bmpInfoHeader.biHeight;

	    bmpImg->width = width;
	    bmpImg->height = height;
	    bmpImg->channels = 3;
	    bmpImg->imageData = (unsigned char*)malloc(sizeof(unsigned char)*width*3*height);
	    step = channels*width;

	    offset = (channels*width)%4;
	    if (offset != 0)
	    {
		offset = 4 - offset;
	    }

	    for (i=0; i<height; i++)
	    {
		for (j=0; j<width; j++)
		{
		    for (k=0; k<3; k++)
		    {
			fread(&pixVal, sizeof(unsigned char), 1, pFile);
			bmpImg->imageData[(height-1-i)*step+j*3+k] = pixVal;
		    }
		    //kzSetMat(bmpImg->mat, height-1-i, j, kzScalar(pixVal[0], pixVal[1], pixVal[2]));
		}
		if (offset != 0)
		{
		    for (j=0; j<offset; j++)
		    {
			fread(&pixVal, sizeof(unsigned char), 1, pFile);
		    }
		}
	    }
	}
    }

    return bmpImg;
}

/*
Module: 把3色真彩图转化为2值图
要求：输入图片
输出：返回生成后的图片
备注:bmp  被修改
Created by LiangPing  &  ChenCong.
Created date: 2014/07/06
 */

void changeToTwoValue(ClImage* bmpImg){
    int height = bmpImg->height, width = bmpImg->width;
    int i,j,k;
    for ( i = 0;i < height; i++)
	for ( j = 0;j < width; j++)
	{
	    //Y=0.3R+0.59G+0.11B
	    //BGR
	    int R = bmpImg->imageData[i*bmpImg->width*3+3*j+2];
	    int G = bmpImg->imageData[i*bmpImg->width*3+3*j+1];
	    int B = bmpImg->imageData[i*bmpImg->width*3+3*j+0];
	    int Y = 0.3*R+0.59*G+0.11*B;
	    for ( k = 0; k< 3;k ++)
		bmpImg->imageData[i*bmpImg->width*3+3*j+k] = Y;
	}
}

/*
Module: 鱼眼矫正
要求：输入图片
输出：返回生成后的图片
Created by LiangPing  &  ChenCong.
Created date: 2014/07/06
 */

#define Max_D  6
#define DIVS   200.0
#define UnitLen 23

double factors[Max_D] = {    //鱼眼矫正系数
    //1.8283,
    //  15.2819,
    // -55.4458,
    //  94.5918,
    // -73.6885,
    //  22.3126
    -0.3633,7.9387  ,-19.9699 ,39.2228,-33.1159 , 10.9308     
};

//求距离
double calcDis(double ax, double ay, double bx, double by){
    return sqrt((ax-bx)*(ax-bx)+(ay-by)*(ay-by));
}

//变换
double calcF(double r){
    /*int k;
      double R=0.0;
      double tmpr=1.;

      r=r/DIVS;
      for ( k = 0; k< Max_D ; k++){
      tmpr*=r;
      R = R  + factors[k]*tmpr;

      }*/
    int k;
    double R=0.0;
    double tmpr=1.;

    r=r/DIVS;
    for ( k = 0; k< Max_D ; k++){
	R = R  + factors[k]*tmpr;
	tmpr*=r;
    }
    return R*UnitLen;
}

/*
Module: 鱼眼矫正
要求：输入图片
输出：返回生成后的图片
备注:bmpImg  没被修改
Created by LiangPing  &  ChenCong.
Created date: 2014/07/06
 */
ClImage* correctImg(const ClImage* bmpImg){
    int i=0,j=0,k=0;
    int height = bmpImg->height, width = bmpImg->width;
    ClImage *newImg= (ClImage*)malloc(sizeof(ClImage));

    int midX, midY;
    int newMidX,newMidY;

    int MULTI = 3,tem=0;

    newImg->width = width + tem;
    //newImg->height = height; //changed by Zhang Cong
    newImg->height = width;
    newImg->channels = 3;
    newImg->imageData = (unsigned char*)malloc(sizeof(unsigned char)*newImg->width*3*newImg->height);
    memset(newImg->imageData,-1,sizeof(unsigned char)*newImg->width*3*newImg->height);

    midX = height/2, midY = width/2;
    newMidX = newImg->height/2, newMidY = newImg->width/2;

    for ( i = 0;i< height; i++)
	for ( j = 0;j < width; j++)
	{
	    double r = calcDis(i,j, midX, midY);
	    double R = calcF(r);

	    double nx = (i-midX)*R/r + 0.5;
	    double ny = (j-midY)*R/r + 0.5;

	    int nx1 = (int) nx + newMidX;
	    int ny1 = (int) ny + newMidY;

	    if ( nx1>=0 && nx1<newImg->height && ny1>=0 && ny1< newImg->width )
	    {
		for ( k = 0;k < 3; k++)
		    newImg->imageData[( nx1*newImg->width + ny1)*3+k] = bmpImg->imageData[(i*width+j)*3+k];

	    }
	}
    return newImg;

}

/*
Module: 保存图像
要求：输入图片
输出：返回生成后的图片
备注:bmp  被修改
Created by ChenCong.
Created date: 2014/07/06
 */
bool clSaveImage(const char* path, const ClImage* bmpImg)
{
    FILE *pFile;
    unsigned short fileType;
    ClBitMapFileHeader bmpFileHeader;
    ClBitMapInfoHeader bmpInfoHeader;
    int step;
    int offset;
    unsigned char pixVal = '\0';
    int i, j;
    ClRgbQuad* quad;

    pFile = fopen(path, "wb");
    if (!pFile)
    {
	return false;
    }

    fileType = 0x4D42;
    fwrite(&fileType, sizeof(unsigned short), 1, pFile);
    if (bmpImg ==NULL){
	printf("There's no BPM to write\n");
	return 0;
    }
    if (bmpImg->channels == 3)//24位，通道，彩图
    {
	step = bmpImg->channels*bmpImg->width;
	offset = step%4;
	if (offset != 4)
	{
	    step += 4-offset;
	}

	bmpFileHeader.bfSize = bmpImg->height*step + 54;
	bmpFileHeader.bfReserved1 = 0;
	bmpFileHeader.bfReserved2 = 0;
	bmpFileHeader.bfOffBits = 54;
	fwrite(&bmpFileHeader, sizeof(ClBitMapFileHeader), 1, pFile);

	bmpInfoHeader.biSize = 40;
	bmpInfoHeader.biWidth = bmpImg->width;
	bmpInfoHeader.biHeight = bmpImg->height;
	bmpInfoHeader.biPlanes = 1;
	bmpInfoHeader.biBitCount = 24;
	bmpInfoHeader.biCompression = 0;
	bmpInfoHeader.biSizeImage = bmpImg->height*step;
	bmpInfoHeader.biXPelsPerMeter = 0;
	bmpInfoHeader.biYPelsPerMeter = 0;
	bmpInfoHeader.biClrUsed = 0;
	bmpInfoHeader.biClrImportant = 0;
	fwrite(&bmpInfoHeader, sizeof(ClBitMapInfoHeader), 1, pFile);

	for (i=bmpImg->height-1; i>-1; i--)
	{
	    for (j=0; j<bmpImg->width; j++)
	    {
		pixVal = bmpImg->imageData[i*bmpImg->width*3+j*3];
		fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
		pixVal = bmpImg->imageData[i*bmpImg->width*3+j*3+1];
		fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
		pixVal = bmpImg->imageData[i*bmpImg->width*3+j*3+2];
		fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
	    }
	    if (offset!=0)
	    {
		for (j=0; j<offset; j++)
		{
		    pixVal = 0;
		    fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
		}
	    }
	}
    }
    else if (bmpImg->channels == 1)//8位，单通道，灰度图
    {
	step = bmpImg->width;
	offset = step%4;
	if (offset != 4)
	{
	    step += 4-offset;
	}

	bmpFileHeader.bfSize = 54 + 256*4 + bmpImg->width;
	bmpFileHeader.bfReserved1 = 0;
	bmpFileHeader.bfReserved2 = 0;
	bmpFileHeader.bfOffBits = 54 + 256*4;
	fwrite(&bmpFileHeader, sizeof(ClBitMapFileHeader), 1, pFile);

	bmpInfoHeader.biSize = 40;
	bmpInfoHeader.biWidth = bmpImg->width;
	bmpInfoHeader.biHeight = bmpImg->height;
	bmpInfoHeader.biPlanes = 1;
	bmpInfoHeader.biBitCount = 8;
	bmpInfoHeader.biCompression = 0;
	bmpInfoHeader.biSizeImage = bmpImg->height*step;
	bmpInfoHeader.biXPelsPerMeter = 0;
	bmpInfoHeader.biYPelsPerMeter = 0;
	bmpInfoHeader.biClrUsed = 256;
	bmpInfoHeader.biClrImportant = 256;
	fwrite(&bmpInfoHeader, sizeof(ClBitMapInfoHeader), 1, pFile);

	quad = (ClRgbQuad*)malloc(sizeof(ClRgbQuad)*256);
	for (i=0; i<256; i++)
	{
	    quad[i].rgbBlue = i;
	    quad[i].rgbGreen = i;
	    quad[i].rgbRed = i;
	    quad[i].rgbReserved = 0;
	}
	fwrite(quad, sizeof(ClRgbQuad), 256, pFile);
	free(quad);

	for (i=bmpImg->height-1; i>-1; i--)
	{
	    for (j=0; j<bmpImg->width; j++)
	    {
		pixVal = bmpImg->imageData[i*bmpImg->width+j];
		fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
	    }
	    if (offset!=0)
	    {
		for (j=0; j<offset; j++)
		{
		    pixVal = 0;
		    fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
		}
	    }
	}
    }
    fclose(pFile);

    return true;
}
