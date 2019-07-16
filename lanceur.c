#include "head.h"

//Fonction principale pour lancer une commande, elle va s'occuper le tableau de chaine de caractère découpé
int lancerfoncappart(char** mot, char** dirs, int *save, int *file, int child){
  int test, i, tmp;
  char pathname[MaxPathLength];

  test=lancerfonc(mot,dirs,save,file,child);
  //Fin du programme
  if(test==666){
    return 666;
  }
  //Retour sans attente
  if(test==3){
    if(save){
      return 5;
    }
    return 0;
  }
  //Attente de l'enfant
  if(test==4){
    wait(0);
      if(save){
        return 5;
      }
      return 0;
  }
  if(test==0){
      tmp = fork();               // lancer le processus enfant
    if (tmp < 0){
        perror("fork");
        return 0;
    }
    if (tmp != 0){             // parent : attendre la fin de l'enfant
        while(wait(0) != tmp);
        if(save){
          return 5;
        }
        return 0;
    }
  }

  // enfant : exec du programme
  for(i = 0; dirs[i] != 0; i++){
    snprintf(pathname, sizeof pathname, "%s/%s", dirs[i], mot[0]);
    execv(pathname, mot);
  }
                              // aucun exec n'a fonctionne
  fprintf(stderr, "%s: not found\n", mot[0]);
  exit(1);
}

//Cette fonction va s'occuper de traiter les caractere spéciaux, a noter quel l'odre de chaque test n'est pas choisie au hasard
int lancerfonc(char** mot, char** dirs, int *save, int *file, int child){
  int i, pid, pid2;
  int fd[2];

  pid=0;
  pid2=0;

  //Recuperer la suite si la premiere partie d'un ; est fini
  if(*save){
    for(i = 0;mot[*save+i]!=0;++i){
      mot[i]=mot[*save+i];
    }
    mot[i]='\0';
    *save=0;
  }
  //On s'occupe de ;
  for(i=0;mot[i]!=0;++i){
    if(!strcmp (mot[i],";")){
      *save=i+1;
      mot[i]='\0';
      break;
     }
  }
  //On s'occupe de &
  for(i=0;mot[i]!=0;++i){}
  if(!strcmp (mot[i-1],"&")){
    if(!child==0){
      fprintf(stderr, "Impossible de lancer en arrière plan une partie de commande.\n",mot[i-1]);
      exit(1);
    }
    mot[i-1]='\0';
    pid=fork();
    if(pid){
      return 3;
    }
    child=1;
  }
  //On s'occupe de &&
  for(i=1;mot[i]!=0;++i){
    if(!strcmp (mot[i],"&&")){
      mot[i]='\0';
      mon_if(mot,&mot[i+1],dirs,child,file);
      return 3;
    }
  }
  //On s'occupe des pipes
  for(i=1;mot[i]!=0;++i){
    if(!strcmp (mot[i],"|")){
      pipe(fd);
      mot[i]='\0';
      execCommand(mot,NULL,fd,dirs,file);
      execCommand(&mot[i+1],fd,NULL,dirs,file);
      if(child==1){
        exit(0);
      }
      return 3;
    }
  }
  //On s'occupe des redirection, a noter qu'il est très simple d'en rajouter
  for(i=1;mot[i]!=0;++i){
    if(!strcmp (mot[i],">") || !strcmp (mot[i],">>") || !strcmp (mot[i],"&>") || !strcmp (mot[i],"2>") || !strcmp (mot[i],"2>>") || !strcmp (mot[i],"2>&1") || !strcmp (mot[i],"1>&2") || !strcmp (mot[i],"1<&2") || !strcmp (mot[i],"2<&1")){
      if(!child==1){
        pid2 = fork();
        child=1;
      }
    	if(pid2>0){
    		return 4;
    	}
    	if(!strcmp (mot[i],">")){
    		*file = open(mot[i+1],  O_WRONLY|O_CREAT, 0666);
    		dup2(*file, 1);
    	}
      else if(!strcmp (mot[i],"<")){
        *file = open(mot[i+1],  O_RDONLY);
        dup2(*file, 0);
      }
    	else if(!strcmp (mot[i],">>")){
    		*file = open(mot[i+1],  O_WRONLY|O_APPEND|O_CREAT, 0666);
    		dup2(*file, 1);
    	}
    	else if(!strcmp (mot[i],"&>")){
    		*file = open(mot[i+1],  O_WRONLY|O_CREAT, 0666);
    		dup2(*file, 1);
    		dup2(*file, 2);
    	}
    	else if(!strcmp (mot[i],"2>")){
    		*file = open(mot[i+1],  O_WRONLY|O_CREAT, 0666);
    		dup2(*file, 2);
    	}
      else if(!strcmp (mot[i],"2>&1")){
        dup2(1, 2);
      }
      else if(!strcmp (mot[i],"1>&2")){
        dup2(2, 1);
      }
      else if(!strcmp (mot[i],"1<&2")){
        dup2(1, 2);
      }
      else if(!strcmp (mot[i],"2<&1")){
        dup2(2, 1);
      }
    	else{
    		*file = open(mot[i+1],  O_WRONLY|O_APPEND|O_CREAT, 0666);
    		dup2(*file, 2);
    	}
    	mot[i]='\0';
    }
  }
  //On s'occupe de cd qui est une commande particuliere car doit etre traité par le processus principale
  if(!strcmp (mot[0],"cd")){
    moncd(mot);
    return 3;
  }
  //On envoie le signal de fin
  if(!strcmp (mot[0],"exit")){
    // ou exit() si on ne veut pas avoir a faire de test a la sortie de lancerfonc.
    return 666;
  }
  //Si le processus courant est un processus enfant, on renvoie 1 qui fera savoir a la fonction qu'elle ne doit pas lancer de fork
  if(child==1){
    return 1;
  }
  return 0;
}