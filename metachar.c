#include "head.h"

//Cette fonction va traiter certains caractères (ici seulemnt *) et va les remplacer dans notre tableau de strings
void metacar(char **mot, char** dirs){

  int i, j, k, kk, k2, k3, test1, test2, pid, nbap, comptap, save;
  int fd[2];
  char pathname[MaxPathLength];
  char* buffer;

  char* repp[MaxMot];
  char* reppavant[MaxMot];
  char* reppapres[MaxMot];
  char* reppav_et_ap[MaxMot];

  char* mot3[MaxMot];
  char* command_ls[] = {"ls",NULL};

  char avant[32];
  char apres[32];

  buffer=malloc(2048*sizeof(char));
  test1=0;
  test2=0;
  for(i=0;mot[i]!=0;++i){ //Parcours de toute les strings
    for(j=0;mot[i][j]!=0;++j){ //Caractere par caractere
      if(mot[i][j]=='*'){ //Si * est trouvé
        pipe(fd);
        pid=fork();
        if(pid>0){
          wait(0);
          read(fd[0],buffer, 2048);
          decouper(buffer," \n",repp,MaxMot);
        }
        else{
          dup2(fd[1], 1);
          for(i = 0; dirs[i] != 0; i++){
            snprintf(pathname, sizeof pathname, "%s/%s", dirs[i], "ls");
            execv(pathname, command_ls);
          }
        }
        if(!j==0){ //Si * a des caractères devant elle
          test1=1;
          for(k=0;k<j;++k){
            avant[k]=mot[i][k];
          }
          avant[k]='\0';
          for(k=0,k2=0;repp[k]!=0;++k){
            for(kk=0;avant[kk]!=0;++kk){
              if(!(repp[k][kk]==avant[kk])){
                break;
              }
              if(avant[kk+1]==0){
                reppavant[k2]=repp[k];
                ++k2;
              }
            }
          }
          reppavant[k2]='\0';
        }
        if(mot[i][j+1]!=0){ //Si * a des caractères derriere elle
          test2=1;
          for(k=j+1,nbap=0;mot[i][k]!=0;++k,++nbap){
            apres[nbap]=mot[i][k];
          }
          apres[nbap]='\0';
          for(k=0,k2=0;repp[k]!=0;++k){
            for(k3=0;repp[k][k3]!=0;++k3){}
            for(kk=k3-1,comptap=0;comptap!=nbap;--kk,++comptap){
              if(!(repp[k][kk]==apres[nbap-comptap-1])){
                break;
              }
              if(comptap+1==nbap){
                reppapres[k2]=repp[k];
                ++k2;
              }
            }
          }
          reppapres[k2]='\0';
        }
        if(test1==1 && test2==1){ //Le cas ou * a des caractere devant et derriere
          for(k=0,k3=0;reppapres[k]!=0;++k){
            for(k2=0;reppavant[k2]!=0;++k2){
              if(!strcmp(reppapres[k],reppavant[k2])){
                reppav_et_ap[k3]=reppapres[k];
                ++k3;
                break;
              }
            }
          }
          reppav_et_ap[k3]='\0';
          for(k=0,k2=0;mot[k]!=0;++k,++k2){
            if(k==i){
              for(k3=0;reppav_et_ap[k3]!=0;++k3,++k2){
                mot3[k2]=reppav_et_ap[k3];
              }
              --k2;
              save=k2;
              
            }
            else{
              mot3[k2]=mot[k];
            }
          }
          mot3[k2]='\0';
          for(k=0;mot3[k]!=0;++k){
            mot[k]=mot3[k];
          }
          mot[k]='\0';
        }
        else if(test1==1){ //Le cas ou * a des caractere devant elle seulement
          for(k=0,k2=0;mot[k]!=0;++k,++k2){
            if(k==i){
              for(k3=0;reppavant[k3]!=0;++k3,++k2){
                mot3[k2]=reppavant[k3];
              }
              --k2;
              save=k2;
              
            }
            else{
              mot3[k2]=mot[k];
            }
          }
          mot3[k2]='\0';
          for(k=0;mot3[k]!=0;++k){
            mot[k]=mot3[k];
          }
          mot[k]='\0';
        }
        else if(test2==1){ //Le cas ou * a des caractères derriere elle seulement
          for(k=0,k2=0;mot[k]!=0;++k,++k2){
            if(k==i){
              for(k3=0;reppapres[k3]!=0;++k3,++k2){
                mot3[k2]=reppapres[k3];
              }
              --k2;
              save=k2;
              
            }
            else{
              mot3[k2]=mot[k];
            }
          }
          mot3[k2]='\0';
          for(k=0;mot3[k]!=0;++k){
            mot[k]=mot3[k];
          }
          mot[k]='\0';
        }
        else{ //Le cas ou etoile est seule
          for(k=0,k2=0;mot[k]!=0;++k,++k2){
            if(k==i){
              for(k3=0;repp[k3]!=0;++k3,++k2){
                mot3[k2]=repp[k3];
              }
              --k2;
              save=k2;
              
            }
            else{
              mot3[k2]=mot[k];
            }
          }
          mot3[k2]='\0';
          for(k=0;mot3[k]!=0;++k){
            mot[k]=mot3[k];
          }
          mot[k]='\0';
        }
        break;
      }
    }
  }
  free(buffer);
}