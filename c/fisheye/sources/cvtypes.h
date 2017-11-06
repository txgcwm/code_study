/*
Module: 矩阵
Created by ZhangCong
Created date: 2014/07/18
 */
#ifndef __CVTYPES_H__
#define __CVTYPES_H__

typedef struct
{
    int row, column;
    double **data;
}Matrix;

typedef struct
{
    double x;
    double y;
}Point;

#endif
