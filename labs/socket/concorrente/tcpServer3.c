/* Lab Redes II */
/* Codigo: tcpServer3.c			     */
/* ***************************************/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

#define PORTA_SERV      5205           /* porta padrao */
#define QLEN            5               /* tamanho da fila de clientes  */
#define MAX_SIZE	80		/* tamanho do buffer */

int main(int argc, char *argv[])
{
   struct sockaddr_in endServ; /* estrutura do servidor   */
   struct sockaddr_in endCli; /*  estrutura do cliente    */
   int    sd, novo_sd;  /* socket descriptors */
   int    pid, alen,n; 
	
   memset((char *)&endServ,0,sizeof(endServ)); /* limpa endServ */
   endServ.sin_family = AF_INET;         /* conf. familia internet  */
   endServ.sin_addr.s_addr = inet_addr(argv[1]); /* conf. com ip disponivel */
   endServ.sin_port = htons(atoi(argv[2])); /* porta disponivel */

   /* Cria socket */
   sd = socket(PF_INET, SOCK_STREAM, 0);
   if (sd < 0) {
     fprintf(stderr, "Falha ao criar socket!\n");
     exit(1); }

   /* liga socket a porta e ip */
   if (bind(sd, (struct sockaddr *)&endServ, sizeof(endServ)) < 0) {
     fprintf(stderr,"Ligacao Falhou!\n");
     exit(1); }

   /* Ouve porta */
   if (listen(sd, QLEN) < 0) {
     fprintf(stderr,"Falhou ouvindo porta!\n");
     exit(1); }

   printf("Servidor ouvindo a porta %s ...\n\n", argv[2]);
   /* Aceita conexoes */
   alen = sizeof(endCli);
   for ( ; ; ) {
	 /* espera nova conexao de um processo cliente ... */	
	if ( (novo_sd=accept(sd, (struct sockaddr *)&endCli, &alen)) < 0) {
		fprintf(stderr, "Falha na conexao\n");
		exit(1); }
	pid = fork();
	if (pid < 0)
	   fprintf(stderr, "Falha na criacao do processo filho!\n");
	else if (pid == 0) { 
		close (sd); /* encerra o socket sd */
		fprintf(stdout, "Cliente %s conectado.\n", inet_ntoa(endCli.sin_addr));
		atende_cliente(novo_sd);
		exit(0);
	} /* fim else-if */
	
	close (novo_sd);
  } /* fim for */
} /* fim do programa */

void atende_cliente(int sock_filho) {
   char bufin[MAX_SIZE];
   int  n;
   while (1) {
 	memset(&bufin, 0x0, sizeof(bufin));
	n = recv(sock_filho, &bufin, sizeof(bufin),0);
	if (strncmp(bufin, "FIM", 3) == 0)
            break;
	printf("<- %s\n", bufin);
    } /* fim while */
    printf("---- encerrando dialogo desse filho -------\n");
    close (sock_filho);
 } /* fim trata_msg */
