#include "head.h"

//Gestion de cd a noter que si la fonction est lancé par un enfant, elle ne fera strictement rien du moins dans l'environement principale et donc celui qui nous interesse.
int moncd(char * av[]){
	char * dir;
	int t, ac;
	for(ac=1;av[ac]!=0;++ac){}
	// traiter les arguments
	if (ac < 2){
		dir = getenv("HOME");
		if (dir == 0)
			dir = "/tmp";
	}
	else if (ac > 2){
		fprintf(stderr, "usage: %s [dir]\n", av[0]);
		return 1;
	}
	else
		dir = av[1];
	// faire le boulot
	t = chdir(dir);
	if (t < 0){
		perror(dir);
		return 1;
	}
	return 0;
}