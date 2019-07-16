#include "head.h"

jmp_buf env;

int main(int argc, char * argv[]){

  char ligne[MaxLigne];
  char * mot[MaxMot];
  char * dirs[MaxDirs];
  int i, j, tmp, test, save, file;
  char chemin[MaxPathLength];

  save=0;
  /* Decouper PATH en repertoires */
  decouper(getenv("PATH"), ":", dirs, MaxDirs);

  /* Lire et traiter chaque ligne de commande */
  for(getcwd (chemin, MaxPathLength),printf("%s ? ",chemin); fgets(ligne, sizeof ligne, stdin) != 0;getcwd (chemin, MaxPathLength), printf("%s ? ",chemin)){
    decouper(ligne, " \t\n", mot, MaxMot);
    if (mot[0] == 0)            // ligne vide
      continue;
    metacar(mot,dirs);
    if (mot[0] == 0)            // Au cas ou l'utilisateur utilise des metacaractere qui rendent sa commande vide
      continue;
    setjmp(env);
    test=lancerfoncappart(mot,dirs,&save,&file,0);
    if(test==666){
      break;
    }
    if(test==5){
      if(save){
        longjmp(env,0);
      }
    }
    if(test==0){
      continue;
    }
  }

  printf("Bye\n");
  return 0;

}