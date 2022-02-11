/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "app.h"
#include "math.h"

void initialize_vector(vetor v, int index_begin, int index_end) {
  for (int i = index_begin; i < index_end; i++) {
    v.vetor_val[i] = pow(i - 10 / 2, 2);
  }
}

void calculate_positions_of_vector(vetor v, int index_begin, int index_end) {
  for (int i = index_begin; i < index_end; i++) {
    v.vetor_val[i] = sqrt(v.vetor_val[i]);
  }
}

Result analyse_vector(CLIENT *clnt, vetor *v) {
	Result *result;

	result = analyse_vector_1000(v, clnt);

	if (result == NULL) {
		fprintf(stderr, "Problema na chamada RPC\n");
		exit(0);
	}

	return *result;
}

int main (int argc, char *argv[]) {
	CLIENT *clnt1, *clnt2;
	Result result1, result2;
	vetor v0, v1, v2;
	int index_begin, index_end;
	int vector_lenght;

	vector_lenght = atoi(argv[3]);
	index_begin = 0;
	index_end = vector_lenght;

	v0.vetor_len = vector_lenght;
	v0.vetor_val = (float*)malloc(sizeof(float) * (v0.vetor_len + 2));
	v1.vetor_len = vector_lenght / 2;
	v1.vetor_val = (float*)malloc(sizeof(float) * (v1.vetor_len + 2));
	v2.vetor_len = vector_lenght / 2;
	v2.vetor_val = (float*)malloc(sizeof(float) * (v2.vetor_len + 2));

	// Vector 0
	initialize_vector(v0, 0, index_end);
	calculate_positions_of_vector(v0, 0, index_end);

	// Populate vector 1
	for (int i = index_begin; i < (index_begin + index_end) / 2; i++) {
		v1.vetor_val[i] = v0.vetor_val[i];
	}

	// Populate vector 2
	for (int i = (index_begin + index_end) / 2; i < index_end; i++) {
		v2.vetor_val[i] = v0.vetor_val[i];
	}

	printf("[ ");
	for(int i = 0; i < v1.vetor_len; i++) {
		printf("%.1f ", v1.vetor_val[i]);
	}
	printf("]\n");

	printf("[ ");
	for(int i = 0; i < v2.vetor_len; i++) {
		printf("%.1f ", v2.vetor_val[i]);
	}
	printf("]\n");

	if(argc != 4) {
		fprintf(stderr, "Uso: %s hostname\n", argv[0]);
		exit(0);
	}
	
	clnt1 = clnt_create(argv[1], PROG, VERSION, "udp");
	clnt2 = clnt_create(argv[2], PROG, VERSION, "udp");

	if (clnt1 == (CLIENT *)NULL) {
		clnt_pcreateerror(argv[1]);
		exit(1);
	}
	if (clnt2 == (CLIENT *)NULL) {
		clnt_pcreateerror(argv[2]);
		exit(1);
	}

	result1 = analyse_vector(clnt1, &v1);
	result2 = analyse_vector(clnt2, &v2);

	if (result1.smallest < result2.smallest) {
		printf("O menor numero eh: %.1f\n", result1.smallest);
	} else {
		printf("O menor numero eh: %.1f\n", result2.smallest);
	}

	if (result1.biggest > result2.biggest) {
		printf("O maior numero eh: %.1f\n", result1.biggest);
	} else {
		printf("O maior numero eh: %.1f\n", result2.biggest);
	}

	return 0;
}