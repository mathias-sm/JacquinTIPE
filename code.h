#include "outils.h"

struct Temps
{
        unsigned int heures;
        unsigned int minutes;
        unsigned int secondes;
};

int code (char file[]);
double eval(unsigned int time, double pourcentage);
void *boucle(void *p);
void conversion(struct Temps *travail);
