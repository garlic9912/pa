#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  char out[256];

  va_list args;
  va_start(args, fmt);
  // i -> fmt
  // count -> out
  // pos -> ...
  int i = 0, count = 0;
  while (fmt[i] != '\0') {
    char c;
    if (fmt[i] == '%') {
      c = fmt[++i];
      switch(c) {
        case 's': 
          char *str = va_arg(args, char *);
          int len1 = strlen(str);
          strcpy(out+count, str);
          count += len1;
          break;
        case 'd':
          int idx = 0;
          char buf[32];
          int num = va_arg(args, int);
          while (num != 0) {
            buf[idx++] = '0' + (num % 10);
            num /= 10;
          }
          buf[idx] = '\0';
          // reversal
          int len2 = strlen(buf);
          for (int k = 0; k < len2 / 2; k++) {
              char temp = buf[k];
              buf[k] = buf[len2 - k - 1];
              buf[len2 - k - 1] = temp;
          }
          strcpy(out+count, buf);
          count += len2;
          break;
        case 'c':
          int tmp_c = va_arg(args, int);
          out[count++] = tmp_c;
      } 
      i++;
    } else {
      out[count++] = fmt[i++];
    }
  }
  
  for (int x = 0; x < count; x++) {
    putch(out[x]);
  }
  return count;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  panic("Not implemented");
}


int sprintf(char *out, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  // i -> fmt
  // count -> out
  int i = 0, count = 0;
  while (fmt[i] != '\0') {
    char c;
    if (fmt[i] == '%') {
      c = fmt[++i];
      switch(c) {
        case 's': 
          char *str = va_arg(args, char *);
          int len1 = strlen(str);
          strcpy(out+count, str);
          count += len1;
          break;
        case 'd':
          int idx = 0;
          char buf[32];
          int num = va_arg(args, int);
          while (num != 0) {
            buf[idx++] = '0' + (num % 10);
            num /= 10;
          }
          buf[idx] = '\0';
          // reversal
          int len2 = strlen(buf);
          for (int k = 0; k < len2 / 2; k++) {
              char temp = buf[k];
              buf[k] = buf[len2 - k - 1];
              buf[len2 - k - 1] = temp;
          }
          strcpy(out+count, buf);
          count += len2;
          break;
      } 
      i++;
    } else {
      out[count++] = fmt[i++];
    }
  }
  return 0;
}


int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
