/* Lab Redes II  */
/* C¢digo: tcpClient2.c			     */
/* ***************************************/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#define MAX_SIZE    	80

main(argc,argv)
	int 	argc; 
	char    *argv[];
{
	struct  sockaddr_in ladoServ; /* contem dados do servidor */
	int     sd;              /* socket descriptor                   */
	int     n;               /* numero de caracteres lidos do servidor */
	char    bufout[MAX_SIZE]; /* buffer de dados enviados  */
	
	memset((char *)&ladoServ,0,sizeof(ladoServ)); /* limpa estrutura */
	memset((char *)&bufout,0,sizeof(bufout)); /* limpa buffer */
	
	ladoServ.sin_family      = AF_INET; /* config. socket p. internet*/
	ladoServ.sin_addr.s_addr = inet_addr(argv[1]); /* ip servidor */
	ladoServ.sin_port        = htons(atoi(argv[2])); /* porta servidor */

	/* Cria socket */
	sd = socket(PF_INET, SOCK_STREAM, 0);
	if (sd < 0) {
		fprintf(stderr, "Criacao do socket falhou!\n");
		exit(1); }

	/* Conecta socket ao servidor definido */
	if (connect(sd, (struct sockaddr *)&ladoServ, sizeof(ladoServ)) < 0) {
		fprintf(stderr,"Tentativa de conexao falhou!\n");
		exit(1); }
	while (1) {
		printf("Digite algo (FIM - para terminar): ");
		fgets(bufout, MAX_SIZE, stdin);
		send(sd,&bufout,strlen(bufout),0);
		if (strncmp(bufout, "FIM",3) == 0) 
			break;
	} /* fim while */
	printf("------- encerrando conexao -----\n");
	close (sd); /* fecha a conexao */
	return (0);
} /* fim do programa */
