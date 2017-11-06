#include "process.h"

#define M  6
#define DIV  200.0
#define Unit 23 
#define eps 1e-8

int sig(double x){return (x>eps)-(x<-eps);}

double f2[]={ 
    -0.013095549,
    0.33565336,
    -0.03071721,  
    0.0011722012,  
    -0.00000073336637,
    -0.00000066272356                    
};

//求距离
double Dis(double ax, double ay, double bx, double by){
    return sqrt((ax-bx)*(ax-bx)+(ay-by)*(ay-by));
}

double calcF1(double r){
    int k;
    double R=0.0;
    double tmpr=1.;

    r=r/(Unit);
    for ( k = 0; k<M ; k++){

	R = R  + f2[k]*tmpr;
	tmpr*=r;
    }
    return R*DIV;
}

double f1[]={-0.3633042321,7.938669648,-19.96994962,39.22284236,-33.11588152,10.93079446 };

double calcF2(double r){
    int k;
    double R=0.0;
    double tmpr=1.;

    r=r/(DIV);
    for ( k = 0; k<M ; k++){

	R = R  + f1[k]*tmpr;
	tmpr*=r;
    }
    return R*Unit;
}


void mWarp(const ClImage* srcImage, ClImage* dstImage,const Matrix * warpMat,int v)
{
    int i,j,k;
    int height = dstImage->height, width = dstImage->width;
    int newX,newY;
    int dstmidX, dstmidY;
    int srcmidX,srcmidY;
    //Matrix * srcP = NULL ,* dstP = NULL;

    Matrix *srcP = mCreateByValue(3,1,1.0);  //像素点初始位置
    Matrix *dstP = mCreateByValue(3,1,1.0);  //变换后的位置

    dstmidX = height/2, dstmidY = width/2;
    srcmidX = srcImage->height/2, srcmidY = srcImage->width/2;

    for ( i = 0;i < height; i++)
	for ( j = 0;j < width; j++)
	{
	    double w,a,b,c,d;
	    double r,R,nx=i,ny=j;

	    r = Dis(nx,ny, dstmidX, dstmidY);
	    R = calcF2(r);
	    nx = (nx-dstmidX)*R/r + dstmidX; //鱼眼矫正点
	    ny = (ny-dstmidY)*R/r + dstmidY;


	    //printf("%lf %lf %lf %lf\n", r, R, nx, ny);


	    if(v==0 && nx>300) continue;
	    if(v==1 && ny<380) continue;
	    if(v==2 && nx<420) continue;
	    if(v==3 && ny>340) continue;


	    srcP->data[0][0]=ny;   
	    srcP->data[1][0]=nx;
	    mMultiply1(warpMat, srcP,dstP);  //通过变换矩阵的乘法，求得变换后位置
	    /*
	    mShowMatrix(srcP);
	    mShowMatrix(warpMat);
	    mShowMatrix(dstP);
	    */
	    if(sig(dstP->data[2][0])==0 ) continue;
	    ny=dstP->data[0][0]/dstP->data[2][0] ;   //视角转换点
	    nx=dstP->data[1][0]/dstP->data[2][0] ;

	    if(sig(nx)<0 || sig(ny)<0 || sig(nx- dstImage->height +1)>=0 || sig(ny- dstImage->width+1)>=0) continue;

	    r = Dis(nx,ny, dstmidX, dstmidY);
	    R = calcF1(r);
	    nx = (nx-dstmidX)*R/r + srcmidX; //鱼眼矫正点
	    ny = (ny-dstmidY)*R/r + srcmidY;

	    //printf("%lf %lf %lf %lf\n", r, R, nx, ny);

	    if(sig(nx)<0 || sig(ny-80)<0 || sig(nx- srcImage->height +1)>=0 || sig(ny- srcImage->width+1+80)>=0) continue;
	    newX=(int) nx;   
	    newY=(int) ny;

	    //printf("%d %d\n", newX, newY);

	    a=(nx-newX)*(ny-newY);  b=(newX+1-nx)*(ny-newY);
	    c=(nx-newX)*(newY+1-ny);d=(newX+1-nx)*(newY+1-ny);

	    //printf("%lf %lf %lf %lf\n", a, b, c, d);
	    //return ;

	    for ( k = 0; k< 3;k ++)//像素迁移
	    {           
		w= d*srcImage->imageData[newX*srcImage->width*3+3*newY + k] + c*srcImage->imageData[(newX+1)*srcImage->width*3+3*newY + k]
		    +b*srcImage->imageData[newX*srcImage->width*3+3*(newY+1) + k] + a*srcImage->imageData[(newX+1)*srcImage->width*3+3*(newY+1) + k];

		dstImage->imageData[i*dstImage->width*3+3*j+k] = (int)(w+0.5);
	    }
	} 
    mDeleteMatrix(srcP);//释放内存
    mDeleteMatrix(dstP);
}

double warp[4][9]={
    /*0.62800148   ,  -1.11784300  ,  149.22586278,
      0.03459439   ,  -0.74789976  ,  255.38543477,
      0.00017682   ,  -0.00322240  ,    1.00000000,

      -1.08086602   ,  -0.44936258  ,  536.66093356,
      -0.74778383   ,   0.04917850  ,  218.94735176,
      -0.00300687   ,   0.00010449  ,    1.00000000,

      0.38674159   ,  -0.95463736  ,  237.64202687,
      0.03628431   ,  -0.69930462  ,  248.52885738,
      0.00005389   ,  -0.00259164  ,    1.00000000,

      -0.86606871   ,  -0.52247699  ,  519.63227269,
      -0.53989058   ,  -0.05216010  ,  245.78398192,
      -0.00245646   ,  -0.00018446  ,    1.00000000*/

    0.52455097    , -0.99152982   , 149.70247343,
    -0.05529165   ,  -0.64788086  ,  261.44229591,
    -0.00009344   ,  -0.00285444  ,    1.00000000,
    -1.14020011   ,  -0.48131786  ,  563.79125968,
    -0.77249894   ,   0.09224414  ,  202.46491453,
    -0.00318730   ,   0.00022576  ,    1.00000000,
    0.41949038   ,  -0.95986062  ,  222.39861209,
    0.03599006   ,  -0.70305201  ,  244.84533525,
    0.00004834   ,  -0.00262030  ,    1.00000000,
    -0.83491946   ,  -0.57638650  ,  539.13636569,
    -0.48716860   ,  -0.05007877  ,  238.96014542,
    -0.00237727   ,  -0.00016949  ,    1.00000000
};

void processImage(ClImage* bmpImg,const char s[], int k)
{
    int height = bmpImg->height, width = bmpImg->width;
    unsigned char*  temp;
    ClImage* img=clNewImage(width,width,bmpImg->channels,0);  //新建一幅空图

    Matrix * warpMat=mCreateByArray(3,3,warp[k]);    //获得变换矩阵

    mWarp(bmpImg, img,warpMat,k);      //投影变换

    mDeleteMatrix(warpMat);        //释放内存

    clSaveImage(s, img);

    temp=bmpImg->imageData;
    bmpImg->imageData= img->imageData;  //交换数据
    img->imageData=temp;

    bmpImg->height=img->height;
    bmpImg->width=img->width;

    clReleaseImage(img);    //释放内存
    img=NULL;

    printf("Step4 is ok ...\n");

    return ;
}


double  srcQuad[4][8] = {
    /*{295,185,295,265,425,265,425,185},
      {515,265,425,265,425,455,515,455},
      {425,535,425,455,295,455,295,535},
      {205,455,295,455,295,265,205,265}*/
    {295,265,335,295,385,295,425,265},
    {425,265,385,295,385,425,425,455},
    {425,455,385,425,335,425,295,455},
    {295,455,335,425,335,295,295,265}
};
double dstQuad[4][8] = {
    //{280,279,193,340,542,325,437,275},
    //{267,294,167,343,552,332,448,287},
    //{299,303,207,347,506,361,429,310},
    //{253,270,138,328,555,321,455,271}
    {193,340,260,409,485,402,539,325},
    {164,344,107,422,608,425,555,333},
    {210,348,253,422,462,430,509,362},
    {138,327,110,414,582,397,552,323}
};

void solve()
{
    int i,j;
    Point srcPoints[4], dstPoints[4];
    Matrix *warpMat = mCreateByValue(3, 3, 0.0);

    for(i=0;i<4;i++) {
		for(j=0;j<4;j++) {
			srcPoints[j].x= srcQuad[i][j*2];
			srcPoints[j].y= srcQuad[i][j*2+1];
			dstPoints[j].x= dstQuad[i][j*2];
			dstPoints[j].y= dstQuad[i][j*2+1];
		}
		
		mGetPerspectiveTransform(srcPoints, dstPoints, warpMat);
		mShowMatrix(warpMat);
    }
	
    getchar(); 
    mDeleteMatrix( warpMat);
	
    return ;
}

void synthesis(ClImage* srcImage[],char *s)
{
    int height = srcImage[0]->height, width = srcImage[0]->width;
    ClImage* img=clNewImage(height,width,srcImage[0]->channels,0);  //新建一幅空图
    ClImage* temp=clNewImage(height,width,srcImage[0]->channels,0);
    //ClImage* img=clNewImage(height,width,srcImage[0]->channels,255);  //新建一幅空图
    //ClImage* temp=clNewImage(height,width,srcImage[0]->channels,255);
    int i,j,k,t;

    for(i=0;i<height;i++)
	for(j=0;j<width;j++)
	    for(k=0;k<3;k++)
	    {
		int cnt=0,w=0;
		for(t=0;t<4;t++)
		{   
		    if(srcImage[t]->imageData[i*width*3+3*j + k]>0) cnt++,w+=srcImage[t]->imageData[i*width*3+3*j + k];

		    /* if(srcImage[t]->imageData[i*width*3+3*j + k] < img->imageData[i*width*3+3*j+k])
		       img->imageData[i*width*3+3*j+k] = srcImage[t]->imageData[i*width*3+3*j + k]; */
		}	

		if(cnt==0) w=0,cnt=1;
		img->imageData[i*width*3+3*j+k] = (int)(w*1.0/cnt);
	    }

    for(i=0;i<height;i++)
	for(j=0;j<width;j++)
	    for(k=0;k<3;k++) 
		if(img->imageData[i*width*3+3*j+k]>0)
		{
		    int i1,j1;
		    int cnt=0,w=0;
		    for(i1=max(0,i-1);i1<min(i+2,height);++i1)
			for(j1=max(0,j-1);j1<min(j+2,width);++j1)
			    if(img->imageData[i1*width*3+3*j1+k]>0) ++cnt,w+=img->imageData[i1*width*3+3*j1+k];
		    temp->imageData[i*width*3+3*j+k]=(int)(w/cnt);
		    //printf("%d %d  %d\n",temp->imageData[i*width*3+3*j+k],img->imageData[i*width*3+3*j+k],cnt);
		}

    // Replenish(img,25);            //图片修复
    clSaveImage(s, temp);
    clReleaseImage(temp);  
    clReleaseImage(img);

    return ;
}
