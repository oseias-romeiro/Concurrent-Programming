/* Processos */

#include "math.h"
#include "stdio.h"
#include "unistd.h"

double delta(double a, double b, double c) { return pow(b, 2) - 4 * a * c; }

int main() {
  int pid;
  double a = 1, b = 10, c = 24;

  printf("Bhaskara: a=%.0lf, b=%.0lf, c=%.0lf\n", a, b, c);

  double dt = delta(a, b, c);

  // cria um processo filho
  pid = fork();
  if (pid > 0) { // pai
    printf("Processo pai (%d) do %d\n", getpid(), pid);

    printf("x' = %.2lf\n", (-b + sqrt(dt)) / (2 * a));

  } else { // filho
    printf("Processo filho (%d)\n", getpid());

    printf("x'' = %.2lf\n", (-b - sqrt(dt)) / (2 * a));
  }

  return 0;
}
