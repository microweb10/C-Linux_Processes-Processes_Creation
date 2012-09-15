#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main(){
	pid_t pid;
	int i,estado,proceso,abuelo,bisabuelo;
	char etiqueta='A';
	pid=fork();
	switch(pid){ //proceso bisabuelo y proceso abuelo (A y B)
	case -1:
		printf("Error al crear el proceso hijo.\n");
		break;
	case 0:
		etiqueta++;
		printf("Soy el proceso %c: mi pid es %d. Mi padre es %d\n",etiqueta,getpid(),getppid());
		abuelo=getpid();
		bisabuelo=getppid();
		break;
	default:
		printf("Soy el proceso %c: mi pid es %d\n",etiqueta,getpid());
		sleep(7);
		printf("soy %c (%d) y muero\n",etiqueta+1,pid);
		printf("soy %c (%d) y muero\n",etiqueta,getpid());
		exit(0);
	}
	pid=fork();
	switch(pid){ //proceso padre (C)
	case -1:
		printf("Error al crear el proceso hijo.\n");
		break;
	case 0:
		etiqueta++;
		printf("Soy el proceso %c: mi pid es %d. Mi padre es %d. Mi abuelo es %d\n",etiqueta,getpid(), getppid(),bisabuelo);
		break;
	default:
		sleep(6);
		printf("soy %c (%d) y muero\n",etiqueta+1,pid);
		exit(0);
	}
	for(i=1;i<=2;i++){ //procesos hijos (D y E)
		pid=fork();
		switch(pid){
		case -1:
			printf("Error al crear el proceso hijo.\n");
			break;
		case 0:
			etiqueta=etiqueta+i;
			printf("Soy el proceso %c: mi pid es %d. Mi padre es %d. Mi abuelo es %d. Mi bisabuelo es %d\n",etiqueta,getpid(),getppid(),abuelo,bisabuelo);
			sleep(5-i); exit(0);
			break;
		}
	}
	for(i=2;i>=1;i--){
		proceso=wait(&estado); if(estado/256==0) printf("soy %c (%d) y muero\n",etiqueta+i,proceso);
	}
return 0;
}

