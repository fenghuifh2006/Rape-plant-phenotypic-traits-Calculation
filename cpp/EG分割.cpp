#define DLL_API extern "C" _declspec(dllexport)
#include<stdio.h>
#include <math.h>
#include "stdlib.h"

/*****ÕºœÒEG£¨ER„–÷µ∑÷∏Ó********/
DLL_API void TOPim2bw2(unsigned char *R,
					   unsigned char *G,
					   unsigned char *B,
					   int row,
					   int col,
					   float EGthreshold,
					   float ERthreshold,
					   unsigned char *bw);
DLL_API void TOPim2bw2(unsigned char *R,
					   unsigned char *G,
					   unsigned char *B,
					   int row,
					   int col,	
					   float EGthreshold,
					   float ERthreshold,
					   unsigned char *bw)
{
	int i, size;
	size = row * col;
	float *r = new float[size]; float *g = new float[size]; float *b = new float[size];
	float *EG = new float[size]; float *ER = new float[size];
	float RGB;
	for ( i = 0; i < size; i++ )
	{
		r[i] = .0; g[i] = .0; r[i] = .0; EG[i] = .0; ER[i] = .0;
		RGB = R[i] + G[i] + B[i];
		if ( RGB > 20 )
		{
			r[i] = R[i] / RGB; g[i] = G[i] / RGB; b[i] = B[i] / RGB;
			EG[i] = 2 * g[i] - r[i] - b[i];
			ER[i] = 1.4 * r[i] - g[i];
			EG[i] = EG[i] > EGthreshold ? 1 : 0;			
			ER[i] = ER[i] > ERthreshold ? 1 : 0;
		}
	}

	for(i = 0; i< size; i++)
		bw[i] = EG[i] - ER[i] > 0 ? 1 : 0;

	delete []r;
	delete []g;
	delete []b;
	delete []EG;
	delete []ER;
}