#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)
static char a[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};


int base_conversion(unsigned int num, int base) {
  int idx = 0;
  int len = 0;
  char buf[64];
  while (num != 0) {
    buf[idx++] = a[num % base];
    num /= base;
  }
  len = idx;
  for (idx = idx-1; idx >= 0; idx--) putch(buf[idx-1]);
  return len;
}



int printf(const char *fmt, ...) {
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
          for (int x = 0; x < len1; x++) {
            putch(*(str + x));
          }
          count += len1;
          break;
        case 'd':
          unsigned int num_d = va_arg(args, int);
          int len_d = base_conversion(num_d, 10);
          count += len_d;
          break;
        case 'c':
          int tmp_c = va_arg(args, int);
          putch((char)(tmp_c));
          count++;
          break;
        case 'x':
          unsigned int num_x = va_arg(args, int);
          int len_x = base_conversion(num_x, 16);
          count += len_x;
          break;
      } 
      i++;
    } else {
      putch(fmt[i++]);
      count++;
    }
  }
  putch('\0');
  va_end(args);
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
