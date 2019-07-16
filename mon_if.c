#include "head.h"

//Fonction gérant les &&
int mon_if(char** f1, char** f2, char** dirs, int child, int* file){
	char pathname[MaxPathLength];
	int i, newpid, statut;
	int save=0;
	newpid=fork();
	if(newpid==0){
		lancerfoncappart(f1,dirs,&save,file,1);
		perror("Erreur sur commande 1");
		exit(1);
	}
	waitpid(newpid,&statut,0);
	if(statut==0){
		if(child==0){ //Si le processus courant est deja un enfant on le fait finir son parcours ici en ne lancant par de fork.
			newpid=fork();
		}
		if(newpid==0 || child==1){
			lancerfoncappart(f2,dirs,&save,file,1);
			perror("Erreur sur commande 2");
			exit(1);
		}
		waitpid(newpid,&statut,0);
	}
	return 0;
}