#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include <string.h>
#include <setjmp.h>
#include <fcntl.h>

#define PROMPT "? "

//lanceur.c
int lancerfoncappart(char** mot, char** dirs, int *save, int *file, int child);
int lancerfonc(char** mot, char** dirs, int *save, int *file, int child);

//metachar.c
void metacar(char **mot, char** dirs);

//pipe.c
int execCommand(char **commande, int in[2], int out[2], char** dirs, int *file);

//decouper.c
void decouper(char *, char *, char **, int);

//mon_if.c
int mon_if(char** f1, char** f2, char** dirs, int child, int* file);

//moncd.c
int moncd(char **);

enum {
  MaxLigne = 1024,              // longueur max d'une ligne de commandes
  MaxMot = MaxLigne / 2,        // nbre max de mot dans la ligne
  MaxDirs = 100,		// nbre max de repertoire dans PATH
  MaxPathLength = 512,		// longueur max d'un nom de fichier
  MaxFonc = 100,
};