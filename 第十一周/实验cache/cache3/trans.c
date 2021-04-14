/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
	int i, j, x, y;
	//tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;
	if( (M == 32)&&(N == 32) )
	{
		for(i =  0; i < N; i = i + 8)
		{
			for(j = 0; j < M; j = j + 4)
			{
			//	if(i == j)
			//	{
			//		for(int x = i; x < i+8; x++)
			//		{
			//			tmp0 = A[x][j];     tmp1 = A[x][j+1];
        	//	        tmp2 = A[x][j+2];   tmp3 = A[x][j+3];
              //   		tmp4 = A[x][j+4];   tmp5 = A[x][j+5];
                // 		tmp6 = A[x][j+6];   tmp7 = A[x][j+7];
                 //		B[j][x] = tmp0;     B[j+1][x] = tmp1;
                 //		B[j+2][x] = tmp2;   B[j+3][x] = tmp3;
                 //		B[j+4][x] = tmp4;   B[j+5][x] = tmp5;
                 //		B[j+6][x] = tmp6;   B[j+7][x] = tmp7;
				//	}
			//	}
			//	else
					for(x = i; x < i+8; x++)
					{
						for(y = j; y < j+4; y++)
						{
							B[y][x] = A[x][y];
						}
					}
			}
		}
	}
			//	tmp0 = A[i][j];		tmp1 = A[i][j+1];
			//	tmp2 = A[i][j+2];	tmp3 = A[i][j+3];
			//	tmp4 = A[i][j+4];	tmp5 = A[i][j+5];
			//	tmp6 = A[i][j+6];	tmp7 = A[i][j+7];
			//	B[j][i] = tmp0;		B[j+1][i] = tmp1;
			//	B[j+2][i] = tmp2;	B[j+3][i] = tmp3;
			//	B[j+4][i] = tmp4;	B[j+5][i] = tmp5;
			//	B[j+6][i] = tmp6;	B[j+7][i] = tmp7;
		//	}
	//	}
//	}
//	else
//	for(i = 0; i < N; i++)
//	{
//		for(j = 0; j < M; j++)
//		{
//			tmp0 = A[i][j];
//			B[j][i] = tmp0;
//		}
//	}
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

