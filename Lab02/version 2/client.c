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

// typedef struct Params {
//   char** argv;
//   int qtd_lines;
// }Params;

// Inicialização do vetor
void initialize_vector(float vector[], int index_begin, int index_end) {
  for (int i = index_begin; i < index_end; i++) {
    vector[i] = pow(i - 10 / 2, 2);
    printf("Alooo %d\n", i);
  }
}

// Cálculo de cada posição do vetor
void calculate_positions_of_vector(float vector[], int index_begin, int index_end) {
  for (int i = index_begin; i < index_end; i++) {
    vector[i] = sqrt(vector[i]);
  }
}

void client_handler(int* argc) {
  int client_id, server_id;
  struct sockaddr_in client_addr;  /* dados do cliente */
  struct sockaddr_in server_addr; /* dados do servidor */
  FILE* fp;
  char* line = NULL;
  size_t len = 0;
  ssize_t read;


  if (*argc < 2) {
    printf("Informe: ./client <IP_ADDRESS>\n");
    exit(1);
  }

  /* criando um socket */
  client_id = socket(AF_INET, SOCK_STREAM, 0);
  if (client_id < 0) {
    printf("Nao foi possivel abrir o socket\n");
    exit(1);
  }

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


  // Inicializa o vetor e calcula cada posição
  float vector[10];
  // for (int i = 0; i < 10000; i++) {
  //   vector[i] = 0;
  // }
  initialize_vector(vector, 0, 10);
  
  calculate_positions_of_vector(vector, 0, 10);

  /* leitura dos ip's e portas dos workers */
  // fp = fopen("/workers.txt", "r");
  // if (fp == NULL) {
  //   printf("Nao foi possivel abrir o arquivo\n");
  //   exit(1);
  // }

  /* dados do servidor(es) */
  memset(&server_addr, '0', sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr("192.168.0.15");
  server_addr.sin_port = htons(atoi("5200"));

  printf("------ Estabelecendo conexao -------\n");
  /* faz a conexao com o servidor */
  server_id = connect(client_id, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (server_id < 0) {
    printf("Nao foi possivel conectar\n");
    exit(1);
  }

  /* Envia os dados */
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

  /* dados do servidor(es) */
  memset(&server_addr, '0', sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr("192.168.0.15");
  server_addr.sin_port = htons(atoi("5300"));

  printf("------ Estabelecendo conexao -------\n");
  /* faz a conexao com o servidor */
  server_id = connect(client_id, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (server_id < 0) {
    printf("Nao foi possivel conectar\n");
    exit(1);
  }

  /* Envia os dados */
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

void process_server(char** argv) {
  int server_id, client_id, server_len, n;
  struct sockaddr_in server_addr;  /* Informacoes do Cliente */
  struct sockaddr_in client_addr; /* Informacoes do Servidor */
  int qtd_workers = 2;
  float vector_result_smallest[10];
  float vector_result_biggest[10];
  int vector_result_s_index = 0;
  int vector_result_b_index = 0;
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
  client_addr.sin_port = htons(atoi("5100"));
  
  /* Fazendo bind na porta do servidor */
  if (bind(server_id, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0) {
    printf("Nao foi possivel fazer o bind\n");
    return;
  }

  listen(server_id, 5);
  server_len = sizeof(server_addr);
  
  printf("Servidor aguardando conexao ... \n");
  
  while (qtd_workers--) {
    /* aceita a conexao do cliente */
    client_id = accept(server_id, (struct sockaddr *)&server_addr, &server_len);
    if (client_id < 0) {
      printf("Nao foi possivel aceitar a conexao\n");
      return;
    }
    
    /* inicia a variavel que vai receber os dados */
    printf("----------- Aceitando conexao ----------\n");
    // memset(rcv_msg, 0x0, MAX_MSG); /* init buffer */

    float vector_result_aux[2];
    
    /* recebe os dados desse cliente */
    n = recv(client_id, vector_result_aux, sizeof(vector_result_aux), 0); /* espera por dados */
    if (n < 0) {
      printf("Nao pode receber os dados\n");
      return;
    }

    vector_result_smallest[vector_result_s_index++] = vector_result_aux[0];
    vector_result_biggest[vector_result_b_index++] = vector_result_aux[1];

    close(client_id);
  }

  float number_smallest = vector_result_smallest[0];
  float number_biggest = vector_result_biggest[0];

  for (int i = 0; i < qtd_workers; i++) {
    if (number_smallest > vector_result_smallest[i]) {
      number_smallest = vector_result_smallest[i];
    }
    if (number_biggest < vector_result_biggest[i]) {
      number_biggest = vector_result_biggest[i];
    }
  }

  printf("Menor: %.1f | Maior: %.1f\n\n", number_smallest, number_biggest);

  // printf("{TCP, IP_S: %s | Porta_S: %u}\n", inet_ntoa(client_addr.sin_addr),
  //         ntohs(client_addr.sin_port));
  // printf("{TCP, IP_C: %s | Porta_C: %u} => %s\n",
  //         inet_ntoa(server_addr.sin_addr),
  //         ntohs(server_addr.sin_port), rcv_msg);
  // printf("----------- Encerrando conexao ----------\n\n");
}

int main(int argc, char *argv[]) {
  pthread_t tid[2];
  // Params params;
  // int qtd_lines = 0;
  // FILE* fp;

  // fp = fopen("workers.txt", "r");
  // if (fp == NULL) {
  //   printf("Nao foi possivel abrir o arquivo\n");
  //   exit(1);
  // }

  // for (char c = getc(fp); c != EOF; c = getc(fp)) {
  //   if (c == '\n') {
  //     qtd_lines++;
  //   }
  // }


  // params.argv = argv;
  // params.qtd_lines = qtd_lines;
  // printf("Aqui - %d\n", params.qtd_lines);

  pthread_create(&tid[0], NULL, (void *)client_handler, (void *)&argc);
  pthread_create(&tid[1], NULL, (void *)process_server, (void *)argv);

  pthread_join(tid[0], NULL);
  pthread_join(tid[1], NULL);

  return 0;
}
