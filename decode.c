#include "decode.h"


//Il y a encore des fuites de mémoire ici lors de la décompression ...! Pas capable de trouvetr où pour l'instant.

int decode(char file[])
{
	long x = 0, y = 0, hP = 0, lP = 0, l = 0, h = 0, diff = 1, range = 0, domaine = 0, toto= 0;
	unsigned int ***final = {NULL};
	char output[32] = {'s', 'o', 'r', 't', 'i', 'e', '/', '0', '0', '0', '.', 'j', 'p', 'g'};
	FILE *input = NULL;
	CvMat *resultat = NULL, *resultat2 = NULL, *prec = 0;
	
	//printf("Combien d'iterations ? N = ");scanf("%ld", &nb_iter);
	input = fopen(file, "rb");

	if(input)
	{
		range = fgetc(input);
		domaine = fgetc(input);
		l = (256*fgetc(input))+fgetc(input);
		h = (256*fgetc(input)) + fgetc(input);
		lP = fgetc(input);lP=l/range;
		hP = fgetc(input);hP=h/range;
		
		//printf("Valeurs: %ld, %ld, %ld, %ld, %ld, %ld\n", range, domaine, l, h, lP, hP);

		//range = 4; domaine = 8;l = 1024; h = 1024; hP = 256; lP=256;
		
		//printf("Valeurs: %ld, %ld, %ld, %ld, %ld, %ld\n", range, domaine, l, h, lP, hP);
		
		printf("\nFichier ouvert, preparation de la situation de depart");
		resultat = cvCreateMat(h, l, CV_8UC1);
		for(y = 0; y < h; y++)
		{
			for (x = 0; x < l; x++)
			{
				toto = (((((x/domaine))%2)*127) + ((((y/domaine) % 2)))*127)+1;
				((unsigned char*)(resultat->data.ptr))[l * x + y] = toto;//((rand() % 255));
			}
		}
		//resultat=cvLoadImageM("lena.jpg", CV_LOAD_IMAGE_GRAYSCALE);

		cvSaveImage(output, resultat, NULL);
		
		final = (unsigned int***) calloc(l, sizeof(unsigned int**));
		for(x = 0; x < l; x++)
		{
			final[x] = (unsigned int **) calloc(h, sizeof(unsigned int *));
			for(y = 0; y < h; y++)
			{
				final[x][y] = (unsigned int*) calloc(5, sizeof(unsigned int));
			}
		}

		for(x = 0; x < lP; x++)
		{
			for(y = 0; y < hP; y++)
			{

				final[x][y][X] = fgetc(input);
				final[x][y][Y] = fgetc(input);
				final[x][y][ANGLE] =  getc(input)-124;
				final[x][y][A] = fgetc(input)-127;
				final[x][y][B] = fgetc(input)-127;
				//printf("(%ld,%ld) -> (%ld,%ld,%ld,%ld,%ld)\n", x, y, final[x][y][X], final[x][y][Y], final[x][y][ANGLE], fgetc(input), fgetc(input));
				//printf("(%ld,%ld) -> (%ld,%ld,%ld,%ld,%ld)\n", x, y, fgetc(input), fgetc(input), fgetc(input), fgetc(input), fgetc(input));
				//while ( getchar() != '\n')
				            ;
			}
		}
	}
	else
	{
		printf("\nFichier introuvable ! Abandon.");
	}
	fclose(input);
	printf("\n\nLancement de la boucle de decodage:\n");
	x = 0;
	while (diff)
	{
		prec = cvCreateMat(h, l, CV_8UC1);
		cvCopy(resultat, prec, NULL);
		resultat2 = cvCreateMat(h,l, CV_8UC1);
		iter(resultat, resultat2, final, lP, hP, l, h, range, domaine);
		diff = Perso_LSREval(resultat2, prec, l, h);
		cvReleaseMat(&resultat);
		resultat = cvCreateMat(h, l, CV_8UC1);
		cvCopy(resultat2, resultat, NULL);
		cvReleaseMat(&resultat2);
		cvReleaseMat(&prec);
		output[9] = 48 + ((x+1) % 10);
		output[8] = 48 + ((x+1) % 100 / 10);
		output[7] = 48 + ((x+1) / 100);
		cvSaveImage(output, resultat, NULL);
		printf("\n  Ecart restant:  %ld", diff);
		x++;
	}
	cvSaveImage("finale.jpg", resultat, NULL);
	cvReleaseMat(&resultat);
	for (x = 0; x < l; x++)
	{
		for(y = 0; y < h; y++)
		{
			free(final[x][y]);
		}
	}
	free(final);
	printf("\n\nFin du programme, image finale: 'finale.jpg', images intermediaires: 'sortie/XXX.jpg'\n\n");
	return 0;
}

void iter(CvMat *origine, CvMat *sortie, unsigned int*** final, unsigned int lP, unsigned int hP, unsigned int l, unsigned int h, unsigned int range, unsigned int domaine)
{
	CvMat *resultat = cvCreateMat(h, l, CV_8UC1), ***grand = NULL, ***petit = NULL, *travail = cvCreateMat(domaine, domaine, CV_8UC1), *backup = NULL, *backup2 = NULL;
	unsigned int x = 0, y = 0,v = 0, w = 0;

	petit = (CvMat***) calloc (l, sizeof(CvMat*));
	for(x = 0; x < l; x++)	
	{
		petit[x] = (CvMat**) calloc(h, sizeof(CvMat));
	}
	for(x = 0; x < l/range; x++)
	{
		for(y = 0; y < h/range; y++)
		{
			petit[x][y] = cvCreateMat(range, range, CV_8UC1);
		}
	}

	grand = (CvMat***) calloc (l, sizeof(CvMat*));
	for(x = 0; x < l; x++)	
	{
		grand[x] = (CvMat**) calloc(h, sizeof(CvMat));
	}
	for(x = 0; x < l/domaine; x++)
	{
		for(y = 0; y < h/domaine; y++)
		{
			grand[x][y] = cvCreateMat(range, range, CV_8UC1);
			for(v=0; v < domaine; v++)
			{
				for(w=0; w < domaine; w++)
				{
					((unsigned char*)(travail->data.ptr))[domaine * w + v] = ((unsigned char*)(origine->data.ptr))[l * (x*domaine+w) + y*domaine+v];
				}
			}
			cvResize(travail, grand[x][y], CV_INTER_LINEAR);
		}
	}
	cvReleaseMat(&travail);
	for(x = 0; x < lP; x++)
	{
		for(y = 0; y < hP;y ++)
		{
			backup = Perso_AXB(grand[final[x][y][X]][final[x][y][Y]], final[x][y][A], final[x][y][B]);
			if (final[x][y][ANGLE] == 0)
			{
				cvCopy(backup, petit[x][y], NULL);
			}
			else if(final[x][y][ANGLE] == 1)
			{
				backup2 = Perso_Rotate(backup, 90*final[x][y][ANGLE], range, range);
				petit[x][y] = Perso_Rotate(backup2, 180, range, range);
			}
			else if(final[x][y][ANGLE] == 2)
			{
				petit[x][y] = Perso_Rotate(backup, 90*final[x][y][ANGLE], range, range);
			}
			else if(final[x][y][ANGLE] == 3)
			{
				backup2 = Perso_Rotate(backup, 90*final[x][y][ANGLE], range, range);
				petit[x][y] = Perso_Rotate(backup2, 180, range, range);
			}
			else if (final[x][y][ANGLE] == 4)
			{
				backup2 = Perso_Rotate(backup, 0, range, range);
				petit[x][y] = Perso_Rotate(backup2, 180, range, range);
			}
			else if (final[x][y][ANGLE] == 5)
			{
				backup2 = Perso_Rotate(backup, 0, range, range);
				cvReleaseMat(&backup);
				petit[x][y] = Perso_Rotate(backup2, 180 - 90*(final[x][y][ANGLE]-4), range, range);
			}
			else if (final[x][y][ANGLE] == 6)
			{
				petit[x][y] = Perso_Rotate(backup, 0, range, range);
			}
			else
			{
				backup2 = Perso_Rotate(backup, 0, range, range);
				cvReleaseMat(&backup);
				petit[x][y] = Perso_Rotate(backup2, 270, range, range);
			}
			cvReleaseMat(&backup);
			cvReleaseMat(&backup2);
		}
	}
	cvReleaseMat(&backup);
	cvReleaseMat(&backup2);
	for(y = 0; y < h; y++)
	{
		for(x = 0; x < l; x++)
		{
			((unsigned char*)(resultat->data.ptr))[l * x + y] = ((unsigned char*)(petit[x/range][y/range]->data.ptr))[range * (x%range) + (y%range)];
		}
	}
	for(x = 0; x < lP; x++)
	{
		for(y = 0; y < hP;y ++)
		{
			cvReleaseMat(&petit[x][y]);
			free(petit[x][y]);
		}
		free(petit[x]);
	}

	for(x = 0; x < l / domaine; x ++)
	{
		for (y = 0; y < h/domaine; y++)
		{
			cvReleaseMat(&grand[x][y]);
			free(grand[x][y]);
		}
		free(grand[x]);
	}

	free(petit);
	free(grand);
	cvCopy(resultat, sortie, NULL);
	cvReleaseMat(&resultat);
}
