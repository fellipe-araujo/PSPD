#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Result {
  float smallest;
  float biggest;
}Result;

void initialize_vector(float *vector) {
  for (int i = 0; i < 10; i++) {
    vector[i] = pow(i - 10 / 2, 2);
  }

  printf("[ ");
  for (int i = 0; i < 10; i++) {
    printf("%1.f ", vector[i]);
  }
  printf("]\n");
}

void calculate_positions_of_vector(float *vector) {
  for (int i = 0; i < 10; i++) {
    vector[i] = sqrt(vector[i]);
  }

  printf("[ ");
  for (int i = 0; i < 10; i++) {
    printf("%1.f ", vector[i]);
  }
  printf("]\n");
}

Result analyse_vector(float *vector) {
  Result result;
  result.smallest = 1000;
  result.biggest = -1;

  for (int i = 0; i < 10; i++) {
    if (vector[i] < result.smallest) {
      result.smallest = vector[i];
    }
    if (vector[i] > result.biggest) {
      result.biggest = vector[i];
    }
  }

  return result;
}

int main() {
  Result result;
  float *vector = malloc(sizeof(float) * 20);

  initialize_vector(vector);
  calculate_positions_of_vector(vector);
  result = analyse_vector(vector);

  printf("O maior numero eh: %.1f | O menor numeo eh: %.1f\n", result.biggest, result.smallest);

  return 0;
}
