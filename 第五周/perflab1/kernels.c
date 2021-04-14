/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/* 
 * Please fill in the following team struct 
 */
team_t team = {
    "Ony Team",            /* Team name */

    "yihua_Leo",         /* First member full name */
    "764503703@qq.com",     /* First member email address */

    "erhua_Leo",            /* Second member full name (leave blank if none) */
    "764503703@qq.com"      /* Second member email addr (leave blank if none) */
};

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/

/* 
 * naive_rotate - The naive baseline version of rotate 
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
}

/* 
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */
char rotate_descr[] = "rotate: Current working version 0";
void rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j, temp;
    for (j = 0; j < dim; j++)	//将j循环放外边，i循环放里边，减少指针的移动次数
    {
        temp = dim-1-j;		//建立临时变量temp储存重复计算的dim-1-j
        for (i = 0; i < dim; i++)
            dst[RIDX(temp,i,dim)] = src[RIDX(i,j,dim)];
    }
}

char rotate_descr1[] = "rotate: Current working version 1.1";
void rotate1(int dim, pixel *src, pixel *dst)
{
    int i, j, ii, jj, temp;
    for(ii = 0; ii < dim; ii=ii+32)
    {
        for(jj = 0; jj < dim; jj=jj+32)
        {
            for(j = jj; j < jj+32;j++)
	    {
		temp = dim-1-j;		  //建立临时变量储存需要重复计算的数据
		for(i = ii; i < ii+32;i++)//每次循环32个数据，以求cache命中
		    dst[RIDX(temp,i,dim)]=src[RIDX(i,j,dim)];
	    }
        }
    }
}

char rotate_descr2[] = "rotate: Current working version 2.0";
void rotate2(int dim, pixel *src, pixel *dst)
{
    int i, j;
    dst = dim*(dim-1)+dst;//设置转换地点的初始值
    for(i = 0; i < dim; i= i + 32)//每次变换32个数据，以求cache命中
    {//把32行作为一个划分界限，每次将这32行的第j列在一个循环内一起转换
        for(j = 0; j < dim; j++)//转换开始
        {
            *dst = *src; dst++; src+=dim;//一共这样操作32次
            *dst = *src; dst++; src+=dim;//每次转换完成后将转换源向下移动一格
            *dst = *src; dst++; src+=dim;//将接受转换的地点左移一位
            *dst = *src; dst++; src+=dim;
            *dst = *src; dst++; src+=dim;
            *dst = *src; dst++; src+=dim;
            *dst = *src; dst++; src+=dim;
            *dst = *src; dst++; src+=dim;
            *dst = *src; dst++; src+=dim;
            *dst = *src; dst++; src+=dim;
            *dst = *src; dst++; src+=dim;
            *dst = *src; dst++; src+=dim;
            *dst = *src; dst++; src+=dim;
            *dst = *src; dst++; src+=dim;
            *dst = *src; dst++; src+=dim;
            *dst = *src; dst++; src+=dim;
            *dst = *src; dst++; src+=dim;
            *dst = *src; dst++; src+=dim;
            *dst = *src; dst++; src+=dim;
            *dst = *src; dst++; src+=dim;
            *dst = *src; dst++; src+=dim;
            *dst = *src; dst++; src+=dim;
            *dst = *src; dst++; src+=dim;
            *dst = *src; dst++; src+=dim;
            *dst = *src; dst++; src+=dim;
            *dst = *src; dst++; src+=dim;
            *dst = *src; dst++; src+=dim;
            *dst = *src; dst++; src+=dim;
            *dst = *src; dst++; src+=dim;
            *dst = *src; dst++; src+=dim;
            *dst = *src; dst++; src+=dim;
            *dst = *src;
            src = src - dim*31 + 1;//转换源初始化，为下一次转换做准备
            dst = dst -dim -31;//转换目标点初始化，为下一次转换做准备
        }
        src = src + dim*31;//转换源向下移动32行
        dst = dst + dim*dim + 32;//转化目标点对于转换源进行对应
    }
}
/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_rotate_functions() 
{
    add_rotate_function(&naive_rotate, naive_rotate_descr);   
    add_rotate_function(&rotate, rotate_descr);   
    add_rotate_function(&rotate1, rotate_descr1);
    add_rotate_function(&rotate2, rotate_descr2);
    /* ... Register additional test functions here */
}


/***************
 * SMOOTH KERNEL
 **************/

/***************************************************************
 * Various typedefs and helper functions for the smooth function
 * You may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct {
    int red;
    int green;
    int blue;
    int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/* 
 * initialize_pixel_sum - Initializes all fields of sum to 0 
 */
static void initialize_pixel_sum(pixel_sum *sum) 
{
    sum->red = sum->green = sum->blue = 0;
    sum->num = 0;
    return;
}

/* 
 * accumulate_sum - Accumulates field values of p in corresponding 
 * fields of sum 
 */
static void accumulate_sum(pixel_sum *sum, pixel p) 
{
    sum->red += (int) p.red;
    sum->green += (int) p.green;
    sum->blue += (int) p.blue;
    sum->num++;
    return;
}

/* 
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel 
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum) 
{
    current_pixel->red = (unsigned short) (sum.red/sum.num);
    current_pixel->green = (unsigned short) (sum.green/sum.num);
    current_pixel->blue = (unsigned short) (sum.blue/sum.num);
    return;
}

/* 
 * avg - Returns averaged pixel value at (i,j) 
 */
static pixel avg(int dim, int i, int j, pixel *src) 
{
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++) 
	for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++) 
	    accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}

/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/

/*
 * naive_smooth - The naive baseline version of smooth 
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++)
            dst[RIDX(i, j, dim)] = avg(dim, i, j, src);

}

/*
 * smooth - Your current working version of smooth. 
 * IMPORTANT: This is the version you will be graded on
 */
char smooth_descr[] = "smooth: Current working version 0.1";
void smooth(int dim, pixel *src, pixel *dst) 
{
    int i, j, ii, jj, max_1, max_2, min_1, min_2;
    pixel_sum sum;
    pixel current_pixel;

    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++)
        {
	    sum.red = sum.green = sum.blue = 0, sum.num = 0;
            max_1 = max(i-1, 0); max_2 = max(j-1,0);
            min_1 = min(i+1,dim-1);min_2 = min(j+1, dim-1);
            for(ii = max_1; ii <= min_1; ii++)
                for(jj = max_2; jj <= min_2; jj++)
                    accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

            assign_sum_to_pixel(&current_pixel, sum);
            dst[RIDX(i, j, dim)] = current_pixel;
        }
}

char smooth_descr1[] = "smooth: Current working version 1.0";
void smooth1(int dim, pixel *src, pixel *dst)
{
    int i, j, ii, jj, max_1, max_2, min_1, min_2;
    pixel_sum sum;
    pixel current_pixel;

    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++)
        {
            sum.red = sum.green = sum.blue = 0, sum.num = 0;
            max_1 = max(i-1, 0); max_2 = max(j-1,0);
            min_1 = min(i+1,dim-1);min_2 = min(j+1, dim-1);
            for(ii = max_1; ii <= min_1; ii++)
                for(jj = max_2; jj <= min_2; jj++)
		{
		    sum.red += (int) src[RIDX(ii, jj, dim)].red;
		    sum.green += (int) src[RIDX(ii, jj, dim)].green;
	 	    sum.blue += (int) src[RIDX(ii, jj, dim)].blue;
		    sum.num++;
		}
	    current_pixel.red = (unsigned short) (sum.red/sum.num);
	    current_pixel.green = (unsigned short) (sum.green/sum.num);
	    current_pixel.blue = (unsigned short) (sum.blue/sum.num);
            dst[RIDX(i, j, dim)] = current_pixel;
        }
}

char smooth_descr2[] = "smooth: Current working version 2.0";
void smooth2(int dim, pixel *src, pixel *dst)
{
    int i, j, ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++)
        {
            sum.red = sum.green = sum.blue = 0, sum.num = 0;
	    if(i==0)
	    {
		if(j==0)
		{
		    sum.red = sum.red + (int)src[RIDX(0,0,dim)].red +
			      (int)src[RIDX(0,1,dim)].red +
			      (int)src[RIDX(1,0,dim)].red +
			      (int)src[RIDX(1,1,dim)].red;
		    sum.green = sum.green + (int)src[RIDX(0,0,dim)].green +
                              (int)src[RIDX(0,1,dim)].green + 
                              (int)src[RIDX(1,0,dim)].green +
                              (int)src[RIDX(1,1,dim)].green;
                    sum.blue = sum.blue + (int)src[RIDX(0,0,dim)].blue +
                              (int)src[RIDX(0,1,dim)].blue +
                              (int)src[RIDX(1,0,dim)].blue +
                              (int)src[RIDX(1,1,dim)].blue;
		    sum.num = sum.num + 4;
		}
		else if(j==dim-1)
		{
		    sum.red = sum.red + (int)src[RIDX(0,dim-1,dim)].red +
                              (int)src[RIDX(0,dim-2,dim)].red +
                              (int)src[RIDX(1,dim-1,dim)].red +
                              (int)src[RIDX(1,dim-2,dim)].red;
                    sum.green = sum.green + (int)src[RIDX(0,dim-1,dim)].green +
                              (int)src[RIDX(0,dim-2,dim)].green +
                              (int)src[RIDX(1,dim-1,dim)].green +
                              (int)src[RIDX(1,dim-2,dim)].green;
                    sum.blue = sum.blue + (int)src[RIDX(0,dim-1,dim)].blue +
                              (int)src[RIDX(0,dim-2,dim)].blue +
                              (int)src[RIDX(1,dim-1,dim)].blue +
                              (int)src[RIDX(1,dim-2,dim)].blue;
                    sum.num = sum.num + 4;
		}
		else
		{
		    for(ii = 0; ii <= 1;ii++)
			for(jj = j - 1; jj <= j + 1;jj++)
			{
			    accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);
			}
		}
	    }
	    else if(i == dim - 1)
	    {
		if(j == 0)
		{
                    sum.red = sum.red + (int)src[RIDX(i,j,dim)].red +
                              (int)src[RIDX(i,j+1,dim)].red +
                              (int)src[RIDX(i-1,j,dim)].red +
                              (int)src[RIDX(i-1,j+1,dim)].red;
                    sum.green = sum.green + (int)src[RIDX(i,j,dim)].green +
                              (int)src[RIDX(i,j+1,dim)].green +
                              (int)src[RIDX(i-1,j,dim)].green +
                              (int)src[RIDX(i-1,j+1,dim)].green;
                    sum.blue = sum.blue + (int)src[RIDX(i,j,dim)].blue +
                              (int)src[RIDX(i,j+1,dim)].blue +
                              (int)src[RIDX(i-1,j,dim)].blue +
                              (int)src[RIDX(i-1,j+1,dim)].blue;
                    sum.num = sum.num + 4;
                }
		else if(j == dim-1)
		{
                    sum.red = sum.red + (int)src[RIDX(i,j,dim)].red +
                              (int)src[RIDX(i,j-1,dim)].red +
                              (int)src[RIDX(i-1,j,dim)].red +
                              (int)src[RIDX(i-1,j-1,dim)].red;
                    sum.green = sum.green + (int)src[RIDX(i,j,dim)].green +
                              (int)src[RIDX(i,j-1,dim)].green +
                              (int)src[RIDX(i-1,j,dim)].green +
                              (int)src[RIDX(i-1,j-1,dim)].green;
                    sum.blue = sum.blue + (int)src[RIDX(i,j,dim)].blue +
                              (int)src[RIDX(i,j-1,dim)].blue +
                              (int)src[RIDX(i-1,j,dim)].blue +
                              (int)src[RIDX(i-1,j-1,dim)].blue;
                    sum.num = sum.num + 4;
                }
		else
		{
                    for(ii = dim - 2; ii <= dim - 1;ii++)
                        for(jj = j - 1; jj <= j + 1;jj++)
                        {
                            accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);
                        }
                }
	    }
	    else
	    {
		if(j == 0)
		    for(ii = i - 1; ii <= i + 1;ii++)
			for(jj = 0; jj <= 1; jj++)
			{
			    accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);
			}
		else if(j == dim - 1)
		    for(ii = i - 1; ii <= i + 1;ii++)
			for(jj = dim - 2; jj <= dim - 1; jj++)
			{
			    accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);
			}
		else
                    for(ii = i - 1; ii <= i + 1;ii++)
                        for(jj = j - 1; jj <= j + 1;jj++)
                        {
                            accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);
                        }
	    }		
        current_pixel.red = (unsigned short) (sum.red/sum.num);
        current_pixel.green = (unsigned short) (sum.green/sum.num);
	current_pixel.blue = (unsigned short) (sum.blue/sum.num);
        dst[RIDX(i, j, dim)] = current_pixel;
    }
}


/********************************************************************* 
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_smooth_functions() {
    add_smooth_function(&naive_smooth, naive_smooth_descr);
    add_smooth_function(&smooth, smooth_descr);
    add_smooth_function(&smooth1, smooth_descr1);
    add_smooth_function(&smooth2, smooth_descr2);
    /* ... Register additional test functions here */
}

