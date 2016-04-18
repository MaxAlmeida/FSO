#include <errno.h>              /* errno and error codes */
#include <sys/time.h>           /* for gettimeofday() */
#include <unistd.h>             /* for gettimeofday(), getpid() */
#include <stdio.h>              /* for printf() */
#include <unistd.h>             /* for fork() */
#include <sys/types.h>          /* for wait() */
#include <sys/wait.h>           /* for wait() */
#include <signal.h>             /* for kill(), sigsuspend(), others */
#include <sys/ipc.h>            /* for all IPC function calls */

int		*indice;
char texto_base[] = "abcdefghijklmnopqrstuvwxyz 1234567890 ABCDEFGHIJKLMNOPQRSTUVWXYZ";

void ImprimeTexto (void) {
	struct timeval tv;
	int number, tmp_index, i;
	int contador=1;
	while(contador<=500) {
		gettimeofday (&tv, NULL );
		number = ((tv.tv_usec / 47) % 3) + 1;
		tmp_index = *indice;
		for (i = 0; i < number; i++ ) {
			if( ! (tmp_index + i > sizeof(texto_base)) ) {
				fprintf(stderr, "%c", texto_base[tmp_index + i]);
			} /* fim-if */
		} /* fim-for */
		*indice = tmp_index + i;
		if (tmp_index + i > sizeof(texto_base)) {
			fprintf(stderr, "\n");
			*indice = 0;
		} /*fim-if */
		contador++;
	} /* fim-while */
	printf("\n");
} /* fim-ImprimeTexto */

int main (void) {
	int rtn;
	int count;
		
	indice = (int *) malloc (sizeof(int));
	/* fazendo essa variavel de indice apontar para o primeiro elemento do vetor */
	*indice= 0;

	/* chama a funcao de impressao do alfabeto */
	ImprimeTexto();
	
} /* fim-main */
