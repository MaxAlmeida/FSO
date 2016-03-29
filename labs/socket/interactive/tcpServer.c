/* *******************************************/
/*Lab.Redes 2  tcpServer.c (não concorrente) */
/*********************************************/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h> /* close */

#define SUCESSO 0
#define ERRO     1
#define MAX_MSG 	 100

int main (int argc, char *argv[]) {
  
  int sd, newSd, cliLen, n;
  struct sockaddr_in endCli;  /* Informacoes do Cliente */
  struct sockaddr_in endServ; /* Informacoes do Servidor */
  char rcv_msg[MAX_MSG];

  /* Criando o socket */
  sd = socket(AF_INET, SOCK_STREAM, 0);
  if(sd<0) {
    printf("nao foi possivel abrir o socket");
    return ERRO; }

  /* Preenchendo dados sobre este servidor */  
  endServ.sin_family 	   = AF_INET;
  endServ.sin_addr.s_addr  = inet_addr(argv[1]);
  endServ.sin_port 	   =  htons(atoi(argv[2]));

  /* Fazendo bind na porta do servidor */
  if(bind(sd, (struct sockaddr *) &endServ, sizeof(endServ))<0) {
    printf("nao foi possivel fazer o bind ");
    return ERRO; }
  listen(sd,5);
  cliLen = sizeof(endCli);
  printf("%s Servidor aguardando conexão ... ", argv[0]); 
  while (1) {  
    /* aceita a conexao do cliente */
    newSd = accept(sd, (struct sockaddr *) &endCli, &cliLen);
    if(newSd<0) {
        printf("nao foi possivel aceitar a conexao ");
        return ERRO; }
    /* inicia a variavel que vai receber os dados */
    printf("----------- Aceitando conexao ---------\n");
    memset(rcv_msg,0x0,MAX_MSG); /* init buffer */

    /* recebe os dados desse cliente */ 
    n = recv(newSd, rcv_msg, MAX_MSG, 0); /* espera por dados */

    if (n<0) {
       printf("nao pode receber os dados");
       return ERRO; }
    printf("{TCP, IP_S: %s, Porta_S: %u,", inet_ntoa(endServ.sin_addr), 
             ntohs(endServ.sin_port));
    printf(" IP_C: %s, Porta_C: %u} => %s\n", inet_ntoa(endCli.sin_addr), 
             ntohs(endCli.sin_port), rcv_msg);
    printf("----------- Encerrando conexao --------\n\n");
    close(newSd);
   } /* fim do while (1) */
   return SUCESSO;
} /* fim do programa */
