/* #include "utils.h" */
/* #include "utils.h" */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

/* smprintf may be unsafe, or use 'asprintf' */
char *smprintf(char *fmt, ...) {
  va_list fmtargs;
  char *ret;
  int len;

  va_start(fmtargs, fmt);
  /* first vsnprintf: detection of str */
  len = vsnprintf(NULL, 0, fmt, fmtargs);
  va_end(fmtargs);

  ret = malloc(++len);
  if (ret == NULL) {
    perror("malloc");
    exit(1);
  }

  va_start(fmtargs, fmt);
  /* second vsnprintf: assign str to *ret actually */
  vsnprintf(ret, len, fmt, fmtargs);
  va_end(fmtargs);

  return ret;
}
