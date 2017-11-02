/*
Module: 矩阵基本运算
Created by ZhangCong
Created date: 2014/07/18
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "matrix.h"


//initializations
//创建矩阵
Matrix *mCreateByValue(int row, int column, double value)
{ 
    int i, j;
    Matrix *m = (Matrix *)malloc(sizeof(Matrix));

    m->row = row;
    m->column = column;
    m->data = (double **)malloc(row * sizeof(double *));

    for (i=0; i<row; ++i) {
		m->data[i] = (double *)malloc(column * sizeof(double));
		for (j=0; j<column; ++j) {
			m->data[i][j] = value;
		}
    }

    return m;
}

//根据数组创建矩阵
Matrix *mCreateByArray(int row, int column, const double *array)
{
    Matrix *m = (Matrix *)malloc(sizeof(Matrix));
    int i, j;
    m->row = row;
    m->column = column;
    m->data = (double **)malloc(row * sizeof(double *));

    for (i=0; i<row; ++i) {
		m->data[i] = (double *)malloc(column * sizeof(double));
		for (j=0; j<column; ++j) {
			m->data[i][j] = array[i * column + j];
		}
    }

    return m;
}

//根据数组创建矩阵
Matrix *mCreateByArray2(int row, int column, const double array[][3])
{
    Matrix *m = (Matrix *)malloc(sizeof(Matrix));
    int i, j;
    m->row = row;
    m->column = column;
    m->data = (double **)malloc(row * sizeof(double *));

    for (i=0; i<row; ++i) {
		m->data[i] = (double *)malloc(column * sizeof(double));
		for (j=0; j<column; ++j) {
			m->data[i][j] = array[i][j];
		}
    }

    return m;
}

//matrix functions
//翻转变换
Matrix *mTranspose(const Matrix *mat)
{
    int i, j;
	
    if (NULL == mat) {
		return NULL;
    }
	
	Matrix *ret = mCreateByValue(mat->column, mat->row, 0.0);

	for (i=0; i<mat->row; ++i) {
	    for (j=0; j<mat->column; ++j) {
			ret->data[j][i] = mat->data[i][j];
	    }
	}

	return ret;
}

//矩阵乘法
Matrix *mMultiply(const Matrix *mat1, const Matrix *mat2)
{
    int i, j, k;
    double sum;
    Matrix *ret;

    if (NULL == mat1 || NULL == mat2 || mat1->column != mat2->row) {
		return NULL;
    }

    ret = mCreateByValue(mat1->row, mat2->column, 0.0);

    for (i=0; i<mat1->row; ++i) {
		for (j=0; j<mat2->column; ++j) {
			sum = 0;
		
			for (k=0; k<mat1->column; ++k) {
				sum += mat1->data[i][k] * mat2->data[k][j];
			}
			ret->data[i][j] = sum;
		}
    }

    return ret;
}

//矩阵乘法
void mMultiply1(const Matrix *mat1, const Matrix *mat2,Matrix *ret)
{
    int i, j, k;
    double sum;
    //Matrix *ret;

    if (NULL == mat1 || NULL == mat2 || mat1->column != mat2->row) {
		return ;
    }

    //ret = mCreateByValue(mat1->row, mat2->column, 0.0);

    for (i=0; i<mat1->row; ++i)
    {
	for (j=0; j<mat2->column; ++j)
	{
	    sum = 0;
	    for (k=0; k<mat1->column; ++k)
	    {
		sum += mat1->data[i][k] * mat2->data[k][j];
	    }
	    ret->data[i][j] = sum;
	}
    }
}

//行变换
void exchangeRow(Matrix *mat, int row1, int row2)
{
    if (NULL == mat) {
		return;
    }
	
	double *temp = (double *)malloc(mat->column * sizeof(double));
	memcpy(temp, mat->data[row1], mat->column * sizeof(double));
	memcpy(mat->data[row1], mat->data[row2], mat->column * sizeof(double));
	memcpy(mat->data[row2], temp,  mat->column * sizeof(double));
	free(temp);
}

void subtractRow(Matrix *mat, int from, int pivot, double coef)
{
    if (NULL != mat)
    {
	int i;
	for (i=0; i<mat->column; ++i)
	{
	    mat->data[from][i] -= (coef * mat->data[pivot][i]);
	}
    }
}

//display functions
void mShowMatrix(const Matrix *mat)
{
    int i, j;
    if (NULL == mat)
    {
	printf("The matrix is [NULL]\n");
	return ;
    }
    else
    {
	printf("The matrix is [%d * %d]\n", mat->row, mat->column);

	for (i=0; i<mat->row; ++i)
	{
	    for (j=0; j<mat->column; ++j)
	    {
		printf("%15.8lf ", mat->data[i][j]);
	    }
	    printf("\n");
	}
    }
}

Matrix *mInverse(const Matrix *mat)
{
    int i, j, k;
    double temp_max;
    int max_r;
    double coef;
    Matrix *expand ;
    Matrix *ret;

    //preprocess
    if (NULL == mat)
    {
	printf("mInverse ERROR: input is NULL\n");
	return NULL;
    }
    if (mat->row != mat->column)
    {
	printf("mInverse ERROR: input matrix is not square\n");
	return NULL;
    }

    //initial the expanded matrix 
    expand = mCreateByValue(mat->row, 2 * mat->column, 0.0);

    for (i=0; i<expand->row; ++i)
    {
	for (j=0; j<mat->column; ++j)
	{
	    expand->data[i][j] = mat->data[i][j];
	}
	expand->data[i][mat->column + i] = 1.0;
    }	
    //Gaussian-Jordan Elimination
    //	  mShowMatrix(expand);
    //for each column
    for (i=0; i<mat->row; ++i)
    {
	//find the row with max abs value
	temp_max = -1.0;
	for (j=i; j<expand->row; ++j)
	{
	    if (fabs(expand->data[j][i]) > temp_max)
	    {
		temp_max = fabs(expand->data[j][i]);
		max_r = j;
	    }
	}

	//exchange the max row with row i
	if (max_r != i)
	{
	    exchangeRow(expand, max_r, i);
	}

	// mShowMatrix(expand);

	//set the pivot to 1.0
	coef = expand->data[i][i] / 1.0;

	for (k=0; k<expand->column; ++k)
	{
	    expand->data[i][k] /= coef;
	}
	//set every other value in column i to 0
	for (j=0; j<expand->row; ++j)
	{
	    if (j != i)
	    {
		subtractRow(expand, j, i, expand->data[j][i] / 1.0);
	    }
	}
    }

    //create the return matrix
    ret = mCreateByValue(expand->row, expand->row, 0.0);
    for (i=0; i<ret->row; ++i)
    {
	for (j=0; j<ret->column; ++j)
	{
	    ret->data[i][j] = expand->data[i][j + ret->row];
	}
    }

    //delete the expanded matrix
    mDeleteMatrix(expand);
    return ret;
}

//determinant functions
double mGetDeterminant(const Matrix *mat)
{
    //TODO
}

//delete functions
void mDeleteMatrix(Matrix *mat)
{
    if (NULL == mat) {
		return;
    }
	
	int i;

	for (i = 0; i < mat->row; ++i) {
		if(mat->data[i] != NULL)
			free(mat->data[i]);
	}
		
	free(mat->data);
	free(mat);
}
