#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int pidproceso;

void alarma(){
}

void alarma2(){
}

void ignorar(){
}

int main(int argc,char *argv[]){
	pid_t pid;
	int i,estado,proceso,abuelo,bisabuelo;
	char etiqueta='A';
	if(argc!=3) printf("Parametros incorrectos, la sintaxis es: ./dos [B,C,D] [1..n].\n");
	else{
		if((argv[1][0]!='B')&(argv[1][0]!='C')&(argv[1][0]!='D'))
			printf("Parametros incorrectos, la sintaxis es: ./dos [B,C,D] [1..n].\n");
		else{
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
				if(argv[1][0]=='B'){ // si este proceso es el que tiene que ejecutar el pstree lo preparo
					pidproceso=pid;
					signal(SIGUSR1,ignorar); // programo la llegada de la se?al e ignoro para no morir
					pause(); // espero a que me llegue la se?al
					printf("Soy el proceso %c con pid %d.\n",argv[1][0],getpid());
					execlp("pstree","pstree","-c",NULL); // ejecuto el comando pstree -c
					proceso=wait(&estado);
				}
				else{
					proceso=wait(&estado);
				}
				if(estado/256==0){
					printf("soy %c (%d) y muero\n",etiqueta+1,pid);
					printf("soy %c (%d) y muero\n",etiqueta,getpid());
				}
				exit(0);
			}
			pid=fork();
			switch(pid){ //proceso padre (C)
			case -1:
				printf("Error al crear el proceso hijo.\n");
				break;
			case 0:
				etiqueta++;
				printf("Soy el proceso %c: mi pid es %d. Mi padre es %d. Mi abuelo es %d\n",etiqueta, getpid(),getppid(),bisabuelo);
				break;
			default:
				if(argv[1][0]=='C'){ // si este proceso es el que tiene que ejecutar el pstree lo preparo
					pidproceso=pid;
					signal(SIGUSR1,ignorar); // programo la llegada de la se?al e ignoro para no morir
					pause(); // espero a que me llegue la se?al
					printf("Soy el proceso %c con pid %d.\n",argv[1][0],getpid());
					execlp("pstree","pstree","-c",NULL); // ejecuto el comando pstree -c
					proceso=wait(&estado);
				}
				else{
					proceso=wait(&estado);
				}
				if(estado/256==0)
					printf("soy %c (%d) y muero\n",etiqueta+1,pid);
				exit(0);
			}
			pid=fork();
			switch(pid){ //proceso hijo D
			case -1:
				printf("Error al crear el proceso hijo.\n");
				break;
			case 0:
				etiqueta=etiqueta+1;
				printf("Soy el proceso %c: mi pid es %d. Mi padre es %d. Mi abuelo es %d. Mi bisabuelo es %d\n",etiqueta,getpid(),getppid(),abuelo,bisabuelo);
				if(argv[1][0]=='D'){ // si este proceso es el que tiene que ejecutar el pstree lo preparo
					pidproceso=pid;
					signal(SIGUSR1,ignorar); // programo la llegada de la se?al e ignoro para no morir
					pause(); // espero a que me llegue la se?al
					printf("Soy el proceso %c con pid %d.\n",argv[1][0],getpid());
					execlp("pstree","pstree","-c",NULL); // ejecuto el comando pstree -c
				}
				signal(SIGALRM,alarma2);
				alarm(atoi(argv[2]));
				pause();
				exit(0);
				break;
			}
			pid=fork(); //proceso hijo E
			switch(pid){
			case -1:
				printf("Error al crear el proceso hijo.\n");
				break;
			case 0:
				etiqueta=etiqueta+2;
				printf("Soy el proceso %c: mi pid es %d. Mi padre es %d. Mi abuelo es %d. Mi bisabuelo es %d\n",etiqueta,getpid(),getppid(),abuelo,bisabuelo);
				signal(SIGALRM,alarma); //programo la captura de una se?al
				printf("programo la alarma para %d segundos.\n",atoi(argv[2]));
				alarm(atoi(argv[2]));  //programo la alarma para los segundos correspondientes
				pause(); //pausa hasta que termine el tiempo de la alarma
				kill(pidproceso,SIGUSR1);
				exit(0);
				break;
			default:
				for(i=2;i>=1;i--){
					proceso=wait(&estado);
					if(estado/256==0)
						printf("soy %c (%d) y muero\n",etiqueta+i,proceso);
				}
				exit(0);
			}
		}
	}
return 0;
}

