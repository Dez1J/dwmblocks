#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int pscanf(const char *path, const char *fmt, ...) {
  FILE *fp;
  va_list ap;
  int n;

  if (!(fp = fopen(path, "r"))) {
    //		warn("fopen '%s':", path);
    return -1;
  }
  va_start(ap, fmt);
  n = vfscanf(fp, fmt, ap);
  va_end(ap);
  fclose(fp);

  return (n == EOF) ? -1 : n;
}
// const char *cpu_perc(void) {
int cpu_perc() {
  static long double a[7];
  long double b[7], sum;

  // memcpy(b, a, sizeof(b));
  /* cpu user nice system idle iowait irq softirq */
  if (a[0] == 0) {
    if (pscanf("/proc/stat", "%*s %Lf %Lf %Lf %Lf %Lf %Lf %Lf", &a[0], &a[1],
               &a[2], &a[3], &a[4], &a[5], &a[6]) != 7) {
      return 1;
    }
  }
  //  printf("\n");
  //  sleep(1);
  //  while (1) {
  if (pscanf("/proc/stat", "%*s %Lf %Lf %Lf %Lf %Lf %Lf %Lf", &b[0], &b[1],
             &b[2], &b[3], &b[4], &b[5], &b[6]) != 7) {
    return 3;
  }
  if (b[0] == 0) {
    return 4;
  }

  sum = (b[0] + b[1] + b[2] + b[3] + b[4] + b[5] + b[6]) -
        (a[0] + a[1] + a[2] + a[3] + a[4] + a[5] + a[6]);

  if (sum == 0) {
    return 5;
  }

  printf("%d%\n", (int)(100 *
                         ((b[0] + b[1] + b[2] + b[5] + b[6]) -
                          (a[0] + a[1] + a[2] + a[5] + a[6])) /
                         sum));
  memcpy(a, b, sizeof(b));
  //    sleep(1);
  //  }

  return 0;
}
int cpu_load() {
  double avgs[3];

  if (getloadavg(avgs, 3) < 0) {
    return -1;
  }

  printf("%.2f %.2f %.2f", avgs[0], avgs[1], avgs[2]);
  return 0;
}

int main() {
  while (1) {
    system("clear");
    cpu_load();
    printf("\t");
    cpu_perc();
//    printf("\n");
    sleep(1);
  }
  return 0;
}
