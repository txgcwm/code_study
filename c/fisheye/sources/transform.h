/*
Module: 求投影变换矩阵
要求：输入变换前后对应点对
输出：变换矩阵
Created by ZhangCong
Created date: 2014/07/30
 */
#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "cvtypes.h"

void mGetPerspectiveTransform(const Point *src, 
	const Point *dst,
	Matrix *map_matrix);

#endif
