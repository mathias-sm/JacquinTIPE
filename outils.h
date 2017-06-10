#include <stdio.h>
#include <stdlib.h>
#include "defines.h"
#include "time.h"
#include <opencv/highgui.h>
#include "pthread.h"
#include <opencv2/imgproc/imgproc_c.h>

CvMat* Perso_AXB(CvMat *travail, int a, int b);
unsigned int Perso_LSREval(CvMat *src1, CvMat *src2, unsigned int l, unsigned int h);
CvMat* Perso_Rotate(CvMat *source, double angle, unsigned int l, unsigned int h);
