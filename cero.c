#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main(int argc,char *argv[]){
	pid_t pid;
	int i,estado,proceso;
	if (argc!=3) printf("Parametros incorrectos, la sintaxis es: ./cero [1..n] [h/v].\n");
	else{
		switch(argv[2][0]){
		case 'h':
			for(i=1;i<=atoi(argv[1]);i++){
				pid=fork();
				switch(pid){
				case -1:
					printf("Error al crear el proceso hijo.\n");
					break;
				case 0:
					printf("Soy el hijo %d. Mi pid es %d y mi ppid es %d\n",i,getpid(),getppid());
					sleep(20);
					exit(0);
				}
			}
			for(i=1;i<=atoi(argv[1]);i++){
				proceso=wait(&estado);
				if(estado/256==0)
					printf("Soy el padre. Mi hijo %d con pid %d ha terminado\n",i,proceso);
				else
					printf("Soy el padre. Mi hijo %d con pid %d ha muerto con una senyal\n",i,proceso);
			}
			exit(0);
		case 'v':
			for(i=1;i<=atoi(argv[1]);i++){
				pid=fork();
				switch(pid){
				case -1:
					printf("Error al crear el proceso hijo.\n");
					break;
				case 0:
					printf("Soy el hijo %d. Mi pid es %d y mi ppid es %d\n",i,getpid(),getppid());
					break;
				default:
					sleep(20+atoi(argv[1])-i);
					printf("Soy el padre %d. Mi hijo con pid %d ha terminado\n",getpid(),pid);
					exit(0);
				}
			}
			break;
		default:
			printf("Parametros incorrectos, la sintaxis es: ./cero [1..n] [h/v].\n");
		}
	}
return 0;
}

