/*
Module: 矩阵基本运算
Created by ZhangCong
Created date: 2014/07/18
 */
#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "cvtypes.h"

//initializations
Matrix *mCreateByValue(int row, int column, double value);
Matrix *mCreateByArray(int row, int column, const double *array);
Matrix *mCreateByArray2(int row, int column, const double array[][3]);
//matrix functions
Matrix *mTranspose(const Matrix *mat);
Matrix *mMultiply(const Matrix *mat1, const Matrix *mat2);
void mMultiply1(const Matrix *mat1, const Matrix *mat2, Matrix *ret);


//determinant functions
double mGetDeterminant(const Matrix *mat);
//display function
void mShowMatrix(const Matrix *mat);
//delete function
void mDeleteMatrix(Matrix *mat);

Matrix *mInverse(const Matrix *mat);
#endif

//1.00000000      0.35795455      0.00568182      0.00000000      0.00000000      0.00000000   -220.00000000    -78.75000000      0.00000000      0.00000000      0.00568182
