/*
Module:新算法测试
Created by LiangPing.
Created date: 2014/08/19
 */
#ifndef STEP4_H
#define STEP4_H

#include<math.h>
#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"
#include "cvtypes.h"
#include "transform.h"
#define max(a,b) (a>b ? a:b)
#define min(a,b) (a<b ? a:b)

void processImage(ClImage* bmpImg,const char s[],int k);
void solve();
void synthesis(ClImage* srcImage[],char *s);

#endif
