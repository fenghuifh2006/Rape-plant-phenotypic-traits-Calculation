#define DLL_API extern "C" _declspec(dllexport)
#include<stdio.h>
#include <math.h>
#include "stdlib.h"

#define STACK_INIT_SIZE 1000//�洢�ռ��ʼ������
#define STACKINCREMENT 100//�洢�ռ��������

typedef struct
{
	int *base;//ջ��ָ��
	int *top;//ջ��ָ��
	int stacksize;//��ǰ�ѷ���Ĵ洢�ռ䣬��Ԫ��Ϊ��λ
}Stack;

//����һ����ջS
bool InitStack(Stack &S)
{
	S.base = (int *)malloc(STACK_INIT_SIZE * sizeof(int));
	if (!S.base)
		return false;
	S.top = S.base;
	S.stacksize = STACK_INIT_SIZE;
	return true;
}

//��ջ������Ԫ��eΪ�µ�ջ��Ԫ��
bool Push(Stack &S, int e)
{
	if (S.top - S.base >= S.stacksize)//ջ����׷�Ӵ洢�ռ�
	{
		S.base = (int*)realloc(S.base, (S.stacksize + STACKINCREMENT) * sizeof(int));
		if (!S.base) return false;//�洢����ʧ��
		S.top = S.base + S.stacksize;
		S.stacksize += STACKINCREMENT;
	}
	*S.top++ = e;
	return true;
}

//��ջ����ջ���գ�ɾ��S��ջ��Ԫ�أ���e������ֵ
int Pop(Stack &S)
{
	if (S.top == S.base) return false;
	//*e = * --S.top;
	return *--S.top;
}
//��ջSΪ��ջ������1�����򷵻�0
bool StackEmpty(Stack S)
{
	return S.top == S.base;
}

//����ջS
void DestroyStack(Stack &S)
{
	delete[]S.base;
	S.base = NULL;
	S.top = NULL;
}








/*************/
//�ƺ�ά��
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
	int blockSize = 2;  //�ֿ��С
	int arow, acol, index, prow, pcol, x, y;
	bool flag;

	arow = ceil(log((float)row) / log(2.0));
	acol = ceil(log((float)col) / log(2.0));
	loop = (arow > acol) ? arow : acol; //ѭ�����������ֿ����
	int *N = new int[loop + 1];  //���Ӹ���
	N[0] = area;
	for (i = 1; i <= loop; i++)
		N[i] = 0;



	//�ƺ�
	for (times = 1; times <= loop; times++)   //��ͬ�ֿ��Сѭ��
	{
		arow = (row + blockSize - 1) / blockSize;  //�з������
		acol = (col + blockSize - 1) / blockSize;  //�з������

		for (m = 0; m < arow; m++)
		for (n = 0; n < acol; n++)          //��ͬ����ѭ��
		{
			prow = m * blockSize;
			pcol = n * blockSize;

			flag = false;

			//ÿ��������ǰ�����ж�
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

	//��С����ֱ�����y=ax+b����б�� 
	//A=��xi^2,B=��xi,C=��yixi,D=��yi
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