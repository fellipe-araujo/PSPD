#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

typedef struct Result {
  float smallest;
  float biggest;
}Result;

typedef struct Config {
  char* ip_server;
  int port_server;
}Config;

// Função que recebe um vetor para fazer a análise e obter o menor e o maior valor
void analyse_vector(float vector[], Result* result) {

	result->smallest = vector[0];
	result->biggest = vector[0];

	for (int i = 0; i < 10000; i++) {
    if (vector[i] < result->smallest) {
      result->smallest = vector[i];
    }
    if (vector[i] > result->biggest) {
      result->biggest = vector[i];
    }
  }
}

void process_client(float vector[], Config config) {
  int client_id, server_id, i;
  struct sockaddr_in client_addr;  /* dados do cliente */
  struct sockaddr_in server_addr; /* dados do servidor */

  /* dados do servidor */
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(config.ip_server);
  server_addr.sin_port = htons(config.port_server);

  /* dados do cliente */
  client_addr.sin_family = AF_INET;
  client_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  client_addr.sin_port = htons(0);

  /* criando um socket */
  client_id = socket(AF_INET, SOCK_STREAM, 0);
  if (client_id < 0) {
    printf("Nao foi possivel abrir o socket\n");
    exit(1);
  }

  /* faz um bind para a porta escolhida */
  server_id = bind(client_id, (struct sockaddr *)&client_addr, sizeof(client_addr));
  if (server_id < 0) {
    printf("Nao foi possivel fazer o bind na porta TCP\n");
    exit(1);
  }

  printf("------- Estabelecendo conexao -------\n");
  /* faz a conexao com o servidor */
  server_id = connect(client_id, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (server_id < 0) {
    printf("Nao foi possivel conectar\n");
    exit(1);
  }

  Result result;
  float vector_result[2];

  analyse_vector(vector, &result);

  vector_result[0] = result.smallest;
  vector_result[1] = result.biggest;

  char* message;
  message = "Vetor analisado";

  printf("Enviando dado => %s \n", message);
  server_id = send(client_id, vector_result, sizeof(vector_result), 0);
  if (server_id < 0) {
    printf("Nao foi possivel enviar dados\n");
    close(client_id);
    exit(1);
  }

  printf("--------- Encerrando conexao ---------\n\n");
  close(client_id);
}

void server_handler(int argc, char* argv[]) {
  int sd, newSd, cliLen, n;
  struct sockaddr_in endCli;  /* Informacoes do Cliente */
  struct sockaddr_in endServ; /* Informacoes do Servidor */
  char* rcv_msg;
  rcv_msg = "Mensagem recebida";

  if (argc < 2) {
    printf("Informe: ./server <IP_ADDRESS_SERVER> <PORT_SERVER>\n");
    exit(1);
  }
  
  /* Criando o socket */
  sd = socket(AF_INET, SOCK_STREAM, 0);
  if (sd < 0) {
    printf("Nao foi possivel abrir o socket\n");
    return;
  }
  
  /* Preenchendo dados sobre este servidor */
  endServ.sin_family = AF_INET;
  endServ.sin_addr.s_addr = inet_addr(argv[1]);
  endServ.sin_port = htons(atoi(argv[2]));
  
  /* Fazendo bind na porta do servidor */
  if (bind(sd, (struct sockaddr *)&endServ, sizeof(endServ)) < 0) {
    printf("Nao foi possivel fazer o bind\n");
    return;
  }

  listen(sd, 5);
  cliLen = sizeof(endCli);
  
  printf("Servidor aguardando conexao ... \n");
  
  while (1) {
    /* aceita a conexao do cliente */
    newSd = accept(sd, (struct sockaddr *)&endCli, &cliLen);
    if (newSd < 0) {
      printf("Nao foi possivel aceitar a conexao\n");
      return;
    }
    
    /* inicia a variavel que vai receber os dados */
    printf("----------- Aceitando conexao ----------\n");

    float vector[10000];
    memset(&vector, '0', sizeof(vector));
    
    /* recebe os dados desse cliente */
    n = recv(newSd, vector, sizeof(vector), 0); /* espera por dados */
    if (n < 0) {
      printf("Nao pode receber os dados\n");
      return;
    }

    printf("{TCP, IP_S: %s | Porta_S: %u}\n", inet_ntoa(endServ.sin_addr),
           ntohs(endServ.sin_port));
    printf("{TCP, IP_C: %s | Porta_C: %u} => %s\n",
           inet_ntoa(endCli.sin_addr),
           ntohs(endCli.sin_port), rcv_msg);
    printf("----------- Encerrando conexao ----------\n\n");

    Config config;
    config.ip_server = inet_ntoa(endCli.sin_addr);
    config.port_server = 5100;

    process_client(vector, config);
    
    close(newSd);
  }
}

int main(int argc, char *argv[]) {
  server_handler(argc, argv);

  return 0;
}
