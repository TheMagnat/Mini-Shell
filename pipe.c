#include "head.h"

//Fonction gérant les pipes
int execCommand(char **commande, int in[2], int out[2], char** dirs, int *file){
	int pid;
	int save=0;
	pid = fork();
	if(pid<0)
		perror("fork");
	else if(pid>0){
		if(in)
			close(in[1]);
		wait(0);
	}
	else{
		if(in){
			close(in[1]);
			dup2(in[0], 0);
		}
		if(out){
			dup2(out[1], 1);
		}
		lancerfoncappart(commande,dirs,&save,file,1);
	}
	return 0;
}

# ifdef TEST
int main(int argc, char const *argv[]){
	int fd[2];
	//char *commande[] = {"echo", "salut", NULL};
	char *command_ls[] = {"ls","-a",NULL};
	char *command_sort[] = {"sort","-r",NULL};

	pipe(fd);

	execCommand(command_ls,NULL,fd);
	execCommand(command_sort,fd,NULL);
	return 0;
}
# endif