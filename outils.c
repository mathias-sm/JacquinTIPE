#include "outils.h"

CvMat *Perso_AXB(CvMat *travail, int a, int b)
{
	unsigned int x, y;
	float val;
	CvMat *result = cvCreateMat(RANGE, RANGE, CV_8UC1);
	unsigned char *inputR = (unsigned char*)(result->data.ptr), *inputT = (unsigned char*)(travail->data.ptr);

	for (y = 0; y < RANGE; y++)
	{
		for (x = 0; x < RANGE; x++)
		{
			val = ((float) a/10)* (float) inputT[RANGE*x+y] + (float) b;
			if (val >= 255)
			{
				val = 255;
			}
			else if (val <= 0)
			{
				val = 0;
			}
			inputR[RANGE*x+y] = (unsigned int) val;
		}
	}
	return result;
}

unsigned int Perso_LSREval(CvMat *src1, CvMat *src2, unsigned int l, unsigned int h)
{
	unsigned int x, y;
	int tmp = 0;
	unsigned int result = 0;
	unsigned char *input1 = (unsigned char*)(src1->data.ptr), *input2 = (unsigned char*)(src2->data.ptr);
	
	
	for(x = 0; x < l; x++)
	{
		for(y=0; y < h; y++)
		{
			tmp = input1[h * y + x] - input2[h * y + x];
			result += tmp * tmp;
		}
	}
	return result;
}

CvMat* Perso_Rotate(CvMat *source, double angle, unsigned int l, unsigned int h)
{
	CvMat *dst = cvCreateMat(h, l, CV_8UC1);
	unsigned int x, y;
	unsigned char *inputS = (unsigned char*)(source->data.ptr), *inputD = (unsigned char*)(dst->data.ptr);

	if(angle == 90)
	{
		for(y = 0; y < h; y++)
		{
			for(x = 0; x < l; x++)
			{
				inputD[l*x + y] = inputS[l*y + (l - 1 - x)];
			}
		}
	}
	else if(angle == 180)
	{
		for(y = 0; y < h; y++)
		{
			for(x = 0; x < l; x++)
			{
				inputD[l*x + y] = inputS[l*(l - 1 - x) + (h - 1 - y)];
			}
		}
	}
	else if(angle == 270)
	{
		for(y = 0; y < h; y++)
		{
			for(x = 0; x < l; x++)
			{
				inputD[l*x + y] = inputS[l*(l - 1 - y) + x];
			}
		}
	}
	else if(angle == 0) //Convention !! Ici, je fais un flip en fait !
	{
		for(y = 0; y < h; y++)
		{
			for(x = 0; x < l; x++)
			{
				inputD[l*x + y] = inputS[l*(l - 1 - x) + y];
			}
		}
	}
	return dst;
}
