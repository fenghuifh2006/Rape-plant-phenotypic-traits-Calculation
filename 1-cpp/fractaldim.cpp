#define DLL_API extern "C" _declspec(dllexport)
#include<stdio.h>
#include <math.h>
#include "stdlib.h"

#define STACK_INIT_SIZE 1000//存储空间初始分配量
#define STACKINCREMENT 100//存储空间分配增量

typedef struct
{
	int *base;//栈底指针
	int *top;//栈顶指针
	int stacksize;//当前已分配的存储空间，以元素为单位
}Stack;

//构造一个空栈S
bool InitStack(Stack &S)
{
	S.base = (int *)malloc(STACK_INIT_SIZE * sizeof(int));
	if (!S.base)
		return false;
	S.top = S.base;
	S.stacksize = STACK_INIT_SIZE;
	return true;
}

//入栈，插入元素e为新的栈顶元素
bool Push(Stack &S, int e)
{
	if (S.top - S.base >= S.stacksize)//栈满，追加存储空间
	{
		S.base = (int*)realloc(S.base, (S.stacksize + STACKINCREMENT) * sizeof(int));
		if (!S.base) return false;//存储分配失败
		S.top = S.base + S.stacksize;
		S.stacksize += STACKINCREMENT;
	}
	*S.top++ = e;
	return true;
}

//出栈，若栈不空，删除S的栈顶元素，用e返回其值
int Pop(Stack &S)
{
	if (S.top == S.base) return false;
	//*e = * --S.top;
	return *--S.top;
}
//若栈S为空栈，返回1，否则返回0
bool StackEmpty(Stack S)
{
	return S.top == S.base;
}

//销毁栈S
void DestroyStack(Stack &S)
{
	delete[]S.base;
	S.base = NULL;
	S.top = NULL;
}








/*************/
//计盒维数
DLL_API void box_counting(unsigned char *bw,
	int row,
	int col,
	int area,
	float *FD);
DLL_API void box_counting(unsigned char *bw,
	int row,
	int col,
	int area,
	float *FD)
{
	if (area <= 0)
		return;
	int loop;
	int i, j, m, n, times;
	int blockSize = 2;  //分块大小
	int arow, acol, index, prow, pcol, x, y;
	bool flag;

	arow = ceil(log((float)row) / log(2.0));
	acol = ceil(log((float)col) / log(2.0));
	loop = (arow > acol) ? arow : acol; //循环次数，即分块次数
	int *N = new int[loop + 1];  //盒子个数
	N[0] = area;
	for (i = 1; i <= loop; i++)
		N[i] = 0;



	//计盒
	for (times = 1; times <= loop; times++)   //不同分块大小循环
	{
		arow = (row + blockSize - 1) / blockSize;  //行方向块数
		acol = (col + blockSize - 1) / blockSize;  //列方向块数

		for (m = 0; m < arow; m++)
		for (n = 0; n < acol; n++)          //不同盒子循环
		{
			prow = m * blockSize;
			pcol = n * blockSize;

			flag = false;

			//每个盒子内前景点判断
			for (i = 0; i < blockSize; i++)
			{
				for (j = 0; j < blockSize; j++)
				{
					x = prow + i;
					y = pcol + j;
					if (x < row && y < col)
					{
						index = x * col + y;
						if (bw[index] != 0)
						{
							N[times]++;
							flag = true;
							break;
						}
					}
				}
				if (flag == true)
					break;
			}
		}

		blockSize = blockSize * 2;
	}

	//最小二乘直线拟合y=ax+b，求斜率 
	//A=∑xi^2,B=∑xi,C=∑yixi,D=∑yi
	//a=(Cn-BD)/(An-BB)
	//b=(AD-CB)/(An-BB) 

	double A = 0.0;
	double B = 0.0;
	double C = 0.0;
	double D = 0.0;
	double xi, yi;
	double xStep = log(2.0);
	for (i = 0; i <= loop; i++)
	{
		xi = i * xStep;
		A += xi * xi;
		B += xi;
		yi = log(double(N[i]));
		C += xi * yi;
		D += yi;
	}
	*FD = -(C * (loop + 1) - B * D) / (A * (loop + 1) - B * B);

}