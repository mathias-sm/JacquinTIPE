
/*FILE: code.c*/

#include "code.h"

//unsigned int* Perso_Min(unsigned int *final1, unsigned int *final2, unsigned int *final3, unsigned int *final4);
void Perso_Insere(CvMat ***petit, CvMat *grand, unsigned int ***final, unsigned int l, unsigned int z, unsigned int t, unsigned int angle, unsigned int alpha, unsigned int beta);

struct Args
{
  CvMat *image;
  CvMat ***petit;
  CvMat ***grand;
  unsigned int z0; 
  unsigned int t0; 
  unsigned int lLocal;
  unsigned int hLocal;
  unsigned int l;
  unsigned int h;
  unsigned int ***final;
  unsigned int id; 
};


int code (char file[])
{
	//Déclaration Variables
	unsigned int z = 0, t = 0, x = 0, y = 0;
	FILE *sortie = NULL;
	pthread_t thread1, thread2, thread3, thread4;
	struct Args args = {NULL, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 1}, args2, args3, args4;
	CvMat *travail = cvCreateMat(DOMAINE, DOMAINE, CV_8UC1);
	//__FIN  ZVirer les variables useless ci-dessus !Z


	sortie = fopen("out.jpgz", "wb");
	args.image = cvLoadImageM(file, CV_LOAD_IMAGE_GRAYSCALE);
	//cvSaveImage("jfiojze.jpg", args.image, NULL);
	/*tmp2 = cvLoadImageM("finale.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	tmp1 = cvCreateMat(512,512,CV_8UC1);
	cvAbsDiff(args.image, tmp2, tmp1);
	cvSaveImage("diff.jpg", tmp1, NULL);
	system("PAUSE");
	return 0;*/
	args.h = args.image->rows;
	args.l = args.image->cols;
	
	printf("\nPreparing images ...");

	if (args.image && sortie)
	{ 
		args.final = (unsigned int***) calloc(args.l, sizeof(unsigned int**));
		for(x = 0; x < args.l; x++)
		{
			args.final[x] = (unsigned int **) calloc(args.h, sizeof(unsigned int *));
			for(y = 0; y < args.h; y++)
			{
				args.final[x][y] = (unsigned int*) calloc(6, sizeof(unsigned int));
				args.final[x][y][ECART] = 999999999;
			}
		}

		args.petit = (CvMat***) calloc (args.l, sizeof(CvMat*));
		for(x = 0; x < args.l; x++)	
		{
			args.petit[x] = (CvMat**) calloc(args.h, sizeof(CvMat));
		}
		for(z = 0; z < args.l/RANGE; z++)
		{
			for(t = 0; t < args.h/RANGE; t++)
			{
				args.petit[t][z] = cvCreateMat(RANGE, RANGE, CV_8UC1);
				for(x=0; x < RANGE; x++)
				{
					for(y=0; y < RANGE; y++)
					{
						cvSetReal2D(args.petit[t][z], x, y, cvGetReal2D(args.image,  t*RANGE+y,z*RANGE+x));
					}
				}
			}
		}

		args.grand = (CvMat***) calloc (args.l, sizeof(CvMat*));
		for(x = 0; x < args.l; x++)	
		{
			args.grand[x] = (CvMat**) calloc(args.h, sizeof(CvMat));
		}
		for(z = 0; z < args.l/DOMAINE; z++)
		{
			for(t = 0; t < args.h/DOMAINE; t++)
			{
				args.grand[t][z] = cvCreateMat(RANGE, RANGE, CV_8UC1);
				for(x=0; x < DOMAINE; x++)
				{
					for(y=0; y < DOMAINE; y++)
					{
						cvSetReal2D(travail, x, y, cvGetReal2D(args.image,  t*DOMAINE+y,z*DOMAINE+x));
					}
				}
				cvResize(travail, args.grand[t][z], CV_INTER_AREA);
			}
		}

		//Début des calculs: on parcourt petit, et à chaque fois on cherche les paramètres !
		printf("\n\nPreparation OK ! Now calculating ...\n\n");

		args.lLocal = (args.l)/(NB_COEUR/2);
		args.hLocal = (args.h)/(NB_COEUR/2);

		pthread_create(&thread1, NULL, boucle, &args);
		args2 = args;
		args2.id = 0;
		args2.t0 = (args2.h)/(NB_COEUR/2);
		pthread_create(&thread2, NULL, boucle, &args2);
		args3 = args2;
		args3.z0 = (args3.l)/(NB_COEUR/2);
		pthread_create(&thread3, NULL, boucle, &args3);
		args4 = args3;
		args4.t0 = 0;
		pthread_create(&thread4, NULL, boucle, &args4);

		pthread_join(thread1, NULL);
		pthread_join(thread2, NULL);
		pthread_join(thread3, NULL);
		pthread_join(thread4, NULL);


		printf("\nDone.\n\nWriting into .jpgz file ...\n");
		fputc(RANGE, sortie);
		fputc(DOMAINE, sortie);
		fputc(((args.l)/256), sortie);
		fputc(((args.l)%256), sortie);
		fputc((args.h)/256, sortie);
		fputc((args.h)%256, sortie);
		fputc(args.l/RANGE, sortie);
		fputc(args.h/RANGE, sortie);
		for(x=0 ; x < args.l/RANGE ; x++)
		{
			for(y=0 ; y < args.h/RANGE ; y++)
			{
				fputc(args.final[x][y][X], sortie);
				fputc(args.final[x][y][Y], sortie);
				fputc(args.final[x][y][ANGLE], sortie);
				fputc(args.final[x][y][A], sortie);
				fputc(args.final[x][y][B], sortie);
				//printf("(%ld,%ld) -> (%ld,%ld,%ld,%ld,%ld)\n",x,y, args.final[x][y][X], args.final[x][y][Y], args.final[x][y][ANGLE], args.final[x][y][A]-127,args.final[x][y][B]-127);
			}
		}
		cvReleaseMat(&args.image);
		fclose(sortie);
		for (x = 0; x < args.l; x++)
		{
			for(y = 0; y < args.h; y++)
			{
				free(args.final[x][y]);
			}
		}
		free(args.petit);
		free(args.final);
		printf("\nFin du programme !\n\n");
		return (0);
	}
	else
	{
		printf("\n\nERROR: Maybe test.jpg does NOT exist ?\n\n"); //Cf la ligne où image est déclarée !
		return 0;
	}
}

void *boucle(void *p)
{
	struct Args *args = p;
	unsigned int z = 0, t = 0, u = 0;
	unsigned int start = (unsigned int) time(NULL);
	unsigned int angle = 0;
	int alpha = 0, beta = 0;
	CvMat *backup = NULL, *backup2 = NULL;
	struct Temps temps = {0,0,0}, ecoule = {0,0,0};

	for(z=args->z0/DOMAINE ; z < (args->z0/(DOMAINE))+(args->lLocal)/DOMAINE ; z += 1)
	{
		for(t=args->t0/DOMAINE ; t < (args->t0/(DOMAINE))+(args->hLocal)/DOMAINE ; t += 1)
		{
			for(alpha = 10-(10*RANGE_ALPHA); alpha <= 10 ; alpha += PAS_ALPHA)
			{
				for(beta = (-1) * RANGE_BETA ; beta <= RANGE_BETA; beta += PAS_BETA)
				{
					backup = Perso_AXB(args->grand[z][t], alpha, beta);
					Perso_Insere(args->petit, backup, args->final, args->l, z, t, 0, alpha, beta);

					for(angle = 90; angle <= 270; angle += PAS_ANGLE)
					{
						backup2 = Perso_Rotate(backup, angle, RANGE, RANGE);
						Perso_Insere(args->petit, backup2, args->final, args->l, z, t, angle/90, alpha, beta);
						cvReleaseMat(&backup2);
					}

					backup2 = cvCreateMat(RANGE, RANGE, CV_8UC1);
					cvCopy(backup, backup2, NULL);
					cvReleaseMat(&backup);
					backup = Perso_Rotate(backup2, 0, RANGE, RANGE);
					cvReleaseMat(&backup2);

					Perso_Insere(args->petit, backup, args->final, args->l, z, t, 4, alpha, beta);

					for(angle = 90; angle <= 270; angle += PAS_ALPHA)
					{
						backup2 = Perso_Rotate(backup, angle, RANGE, RANGE);
						Perso_Insere(args->petit, backup2, args->final, args->l, z, t, 4+angle/90, alpha, beta);
						cvReleaseMat(&backup2);
					}
					cvReleaseMat(&backup);
				}
				cvReleaseMat(&backup);
			}
			if (args->id)
			{
				if(u % 1 == 0)
				{
					/*printf("[");
					for(x = 0; x < 50 * ((z*(args->lLocal/DOMAINE))+t)/((args->hLocal/DOMAINE)*(args->lLocal/DOMAINE)); x++)
					{
						printf("=");
					}
					printf(">");
					for (x = 50 * ((z*(args->lLocal/DOMAINE))+t)/((args->hLocal/DOMAINE)*(args->lLocal/DOMAINE)); x <= 50; x++)
					{
						printf("-");
					}
					printf("]");*/
					printf("  %.0f%%", 100. * ((z*(args->lLocal/DOMAINE))+t)/((args->hLocal/DOMAINE)*(args->lLocal/DOMAINE)));
					ecoule.secondes = (time(NULL) - start);
					conversion(&ecoule);
					//printf("\n\t->Ecoule : %ldh %ldm %lds", ecoule.heures, ecoule.minutes, ecoule.secondes);
					if ( 100. * ((z*(args->lLocal/DOMAINE))+t)/((args->hLocal/DOMAINE)*(args->lLocal/DOMAINE)) <= 1.)
					{
						printf("  ->  Restant: ~peu precis~\n");
					}
					else
					{
						temps.secondes = eval((time(NULL) - start), 100. * ((z*(args->lLocal/DOMAINE))+t)/((args->hLocal/DOMAINE)*(args->lLocal/DOMAINE)));
						conversion(&temps);
						printf("  ->  Restant: %uh %um %us\n", temps.heures, temps.minutes, temps.secondes);
					}
					u = 1;
				}
				else 
				{
					u++;
				}
			}
		}
	}
	return 0;
}

void Perso_Insere(CvMat ***petit, CvMat *grand, unsigned int ***final, unsigned int l, unsigned int z, unsigned int t, unsigned int angle, unsigned int alpha, unsigned int beta)
{
	unsigned int x = 0, y = 0, test = 0;

	for (x = 0; x < l/RANGE; x++)
	{
		for (y = 0; y < l/RANGE; y++)
		{
			test = Perso_LSREval(petit[x][y], grand, RANGE, RANGE);
			if (test < final[x][y][ECART])
			{
				final[x][y][X] = z;
				final[x][y][Y] = t;
				final[x][y][ANGLE] = angle+124;
				final[x][y][A] = alpha+127;
				final[x][y][B] = beta+127;
				final[x][y][ECART] = test;
			}
		}
	}
}

double eval(unsigned int time, double pourcentage)
{
	return ((double) time * (100. /pourcentage) - (double) time);
}

void conversion(struct Temps *travail)
{
	travail->heures = travail->secondes * 0.0002777777;
	travail->secondes = travail->secondes % 3600;
	travail->minutes = travail->secondes * 0.016666666;
	travail->secondes = travail->secondes % 60;
}
