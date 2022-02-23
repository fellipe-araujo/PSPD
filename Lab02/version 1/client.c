#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>

typedef struct Result {
  float smallest;
  float biggest;
}Result;

typedef struct Args {
  char** argv;
  int argc;
}Args;

// Inicialização do vetor
void initialize_vector(float vector[], int index_begin, int index_end) {
  for (int i = index_begin; i < index_end; i++) {
    vector[i] = pow(i - 10 / 2, 2);
  }
}

// Cálculo de cada posição do vetor
void calculate_positions_of_vector(float vector[], int index_begin, int index_end) {
  for (int i = index_begin; i < index_end; i++) {
    vector[i] = sqrt(vector[i]);
  }
}

void client_handler(char* argv[]) {
  int client_id, server_id;
  struct sockaddr_in client_addr;  /* dados do cliente */
  struct sockaddr_in server_addr; /* dados do servidor */

  /* criando um socket */
  client_id = socket(AF_INET, SOCK_STREAM, 0);
  if (client_id < 0) {
    printf("Nao foi possivel abrir o socket\n");
    exit(1);
  }

  /* dados do servidor(es) */
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr("192.168.0.16");
  server_addr.sin_port = htons(atoi("5200"));

  /* dados do cliente */
  client_addr.sin_family = AF_INET;
  client_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  client_addr.sin_port = htons(0);

  /* faz um bind para a porta escolhida */
  server_id = bind(client_id, (struct sockaddr *)&client_addr, sizeof(client_addr));
  if (server_id < 0) {
    printf("Nao foi possivel fazer o bind na porta TCP\n");
    exit(1);
  }

  printf("------ Estabelecendo conexao -------\n");
  /* faz a conexao com o servidor */
  server_id = connect(client_id, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (server_id < 0) {
    printf("Nao foi possivel conectar\n");
    exit(1);
  }

  // Inicializa o vetor e calcula cada posição
  float vector[10000];
  memset(&vector, '0', sizeof(vector));
  initialize_vector(vector, 0, 10000);
  calculate_positions_of_vector(vector, 0, 10000);

  char* message;
  message = "vetor sendo enviado";

  printf("Enviando dado => %s \n", message);
  server_id = send(client_id, vector, sizeof(vector), 0);
  if (server_id < 0) {
    printf("Nao foi possivel enviar dados\n");
    close(client_id);
    exit(1);
  }

  printf("--------- Encerrando conexao ---------\n\n");
  close(client_id);
}

void process_server(char* argv[]) {
  int server_id, client_id, server_len, n;
  struct sockaddr_in server_addr;  /* Informacoes do Cliente */
  struct sockaddr_in client_addr; /* Informacoes do Servidor */
  char* rcv_msg;
  rcv_msg = "Vetor analisado recebido";
  
  /* Criando o socket */
  server_id = socket(AF_INET, SOCK_STREAM, 0);
  if (server_id < 0) {
    printf("Nao foi possivel abrir o socket\n");
    return;
  }
  
  /* Preenchendo dados sobre este servidor */
  client_addr.sin_family = AF_INET;
  client_addr.sin_addr.s_addr = inet_addr(argv[1]);
  client_addr.sin_port = htons(atoi(argv[2]));
  
  /* Fazendo bind na porta do servidor */
  if (bind(server_id, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0) {
    printf("Nao foi possivel fazer o bind\n");
    return;
  }

  listen(server_id, 5);
  server_len = sizeof(server_addr);
  
  printf("Servidor aguardando conexao ... \n");
  
  while (1) {
    /* aceita a conexao do cliente */
    client_id = accept(server_id, (struct sockaddr *)&server_addr, &server_len);
    if (client_id < 0) {
      printf("Nao foi possivel aceitar a conexao\n");
      return;
    }
    
    /* inicia a variavel que vai receber os dados */
    printf("----------- Aceitando conexao ----------\n");

    float vector_result[2];
    
    /* recebe os dados desse cliente */
    n = recv(client_id, vector_result, sizeof(vector_result), 0); /* espera por dados */
    if (n < 0) {
      printf("Nao pode receber os dados\n");
      return;
    }

    printf("Menor: %.1f | Maior: %.1f\n\n", vector_result[0], vector_result[1]);

    printf("{TCP, IP_S: %s | Porta_S: %u}\n", inet_ntoa(client_addr.sin_addr),
           ntohs(client_addr.sin_port));
    printf("{TCP, IP_C: %s | Porta_C: %u} => %s\n",
           inet_ntoa(server_addr.sin_addr),
           ntohs(server_addr.sin_port), rcv_msg);
    printf("----------- Encerrando conexao ----------\n\n");
    
    close(client_id);
    break;
  }
}

int main(int argc, char *argv[]) {
  pthread_t tid[2];

  pthread_create(&tid[0], NULL, (void *)client_handler, (void *)argv);
  pthread_create(&tid[1], NULL, (void *)process_server, (void *)argv);

  pthread_join(tid[0], NULL);
  pthread_join(tid[1], NULL);

  return 0;
}
