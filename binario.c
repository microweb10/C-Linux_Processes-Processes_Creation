#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>



int main(int argc,char *argv[]){
	pid_t pid1,pid2;
	int i,estado,proceso;
	if (argc!=2) printf("Parametros incorrectos, la sintaxis es: ./binario [1..n].\n");
	else{
		for(i=1;i<=atoi(argv[1]);i++){
			pid1=fork();
			switch(pid1){
			case -1:
				printf("Error al crear el proceso hijo.\n");
				break;
			case 0:
				printf("Soy el hijo %d. Mi pid es %d y mi ppid es %d\n",i,getpid(),getppid());
				signal(SIGUSR1,SIG_IGN); //SE IGNORA LA SEÑAL PARA QUE EL PROCESO NO MUERA
				pause();
				printf("Soy el primer hijo y ahora despierto.\n");
				break;
			}
			pid2=fork();
			switch(pid2){
			case -1:
				printf("Error al crear el proceso hijo.\n");
				break;
			case 0:
				printf("Soy el hijo %d. Mi pid es %d y mi ppid es %d\n",i,getpid(),getppid());
				break;
			default:
				printf("Valor de pid1 %d\n",pid1);
				kill(pid1,SIGUSR1);
				sleep(7);
				exit(0);
			}
		}
		exit(0);
	}
return 0;
}
