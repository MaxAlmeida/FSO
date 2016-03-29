/* Codigo: tcpServer.c			     */
/* ***************************************/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#define QLEN            1  /* tamanho da fila de clientes  */
#define MAX_SIZE	80	   /* tamanho do buffer */
int main(int argc, char *argv[])
{
	struct  sockaddr_in endServ; /* estrutura do servidor   */
	struct  sockaddr_in endCli; /*  estrutura do cliente    */
	int     sd, novo_sd;     /* socket descriptors          */
	int     alen,n;          /* tamanho do endereco         */
	char    bufin[MAX_SIZE]; /* buffer                      */
	
	memset((char *)&endServ,0,sizeof(endServ)); /* limpa endServ */
	endServ.sin_family = AF_INET;         /* conf. familia internet  */
	endServ.sin_addr.s_addr = inet_addr (argv[1]); 
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
  while(1){
	printf("Ouvindo o IP %s na porta %s ...\n\n", argv[1], argv[2]);
	/* Aceita conexoes */
	alen = sizeof(endCli);
	if ( (novo_sd=accept(sd, (struct sockaddr *)&endCli, &alen)) < 0) {
		fprintf(stderr, "Falha na conexao\n");
		exit(1); }
	//printf("Cliente %s conectado.\n",inet_ntoa(endCli.sin_addr));
	  printf("Cliente conectado...\n");
	while (1) {
		memset(&bufin, 0x0, sizeof(bufin));
		n = recv(novo_sd,&bufin,sizeof(bufin),0);
		if (strncmp(bufin, "FIM", 3) == 0)
			break;
		printf("<- %s\n", bufin);
	} /* fim while */
	printf("---- encerrando a conexao -------\n");
	close (novo_sd);
  } //fim do while
	close (sd);
} /* fim do programa */
