/*
Module: 求投影变换矩阵
要求：输入变换前后对应点对
输出：变换矩阵
Created by ZhangCong
Created date: 2014/07/30
 */
#include<math.h>
#include "matrix.h"

//src and dst are both pointed to an array of 4 items
void mGetPerspectiveTransform(const Point *src, const Point *dst, Matrix *map_matrix)
{
    int i, j;
    Matrix *matrixA = mCreateByValue(8, 8, 0.0); 
    Matrix *matrixB = mCreateByValue(8, 1, 0.0); 
    Matrix *inverseA;
    Matrix *lamda;


    //reset matrixB according to dst points
    for (i=0; i<4; ++i)
    {
	matrixB->data[2 * i][0] = dst[i].x;
	matrixB->data[2 * i + 1][0] = dst[i].y;
    }
    //reset matrixA according to src points
    for (i=0; i<4; ++i)
    {
	matrixA->data[2 * i][0] = src[i].x;
	matrixA->data[2 * i][1] = src[i].y;
	matrixA->data[2 * i][2] = 1.0;
	matrixA->data[2 * i][6] = -(dst[i].x * src[i].x);
	matrixA->data[2 * i][7] = -(dst[i].x * src[i].y);

	matrixA->data[2 * i + 1][3] = src[i].x;
	matrixA->data[2 * i + 1][4] = src[i].y;
	matrixA->data[2 * i + 1][5] = 1.0;
	matrixA->data[2 * i + 1][6] = -(dst[i].y * src[i].x);
	matrixA->data[2 * i + 1][7] = -(dst[i].y * src[i].y);
    }


    //solve A * (lamda) = B
    inverseA = mInverse(matrixA);
    //mShowMatrix(inverseA);
    lamda = mMultiply(inverseA, matrixB);

    //fill map_matrix according to Matrix lamda solved above
    for (i=0; i<3; ++i)
    {
	for (j=0; j<3; ++j)
	{
	    if (i == 2 && j == 2)
	    {
		map_matrix->data[i][j] = 1.0;
	    }
	    else
	    {
		map_matrix->data[i][j] = lamda->data[i * 3 + j][0];
	    }
	}
    }

    //memory free
    mDeleteMatrix(inverseA);
    mDeleteMatrix(lamda);
    mDeleteMatrix(matrixB);
    mDeleteMatrix(matrixA);
}


