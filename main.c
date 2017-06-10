#include <stdio.h>
#include "code.h"
#include "decode.h"


int main (int argc, char *argv[])
{
	if (argc != 3) 
	{
		printf("Usage: algo [-z|-u] FILE\n\nFin du programme.\n");
	}
	else
	{
		if((argv[1][0] == '-') && (argv[1][1] == 'z'))
		{
			return code(argv[2]);
		}
		else if((argv[1][0] == '-') && (argv[1][1] == 'u'))
		{
			return decode(argv[2]);
		}
		else
		{
			printf("Usage: algo [-z|-u] FILE\n\nFin du programme.\n");
		}
	}
	return 0;
}
