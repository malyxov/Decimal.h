#include <math.h>

#include "s21_decimal.h"

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  if (!dst) {
    return S21_ERROR_RETURN;
  }

  int buf = src.bits[0];
  int sign = src.bits[3] >> 31;
  int exp = (src.bits[3] << 7) >> 23;

  if (exp < 0 || exp >= 29) {
    return S21_ERROR_RETURN;
  }

  float del = powf(10, exp);
  *dst = (float)buf / del;
  if (sign == 1) {
    *dst *= -1;
  }

  return S21_POSITIVE_RETURN;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int res = S21_POSITIVE_RETURN;
  if (dst) {
    int exp = (src.bits[3] << 7) >> 23;
    if (exp >= 0 && exp < 29) {
      long long del = pow(10, exp);
      double rez = src.bits[0] / del;
      *dst = rez;
      if (src.bits[3] >> 31 == 1) {
        *dst *= (-1);
      }
    } else {
      res = S21_ERROR_RETURN;
    }
  } else {
    res = S21_ERROR_RETURN;
  }

  return res;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  if (!dst) {
    return S21_ERROR_RETURN;
  }

  int sign = 0;
  if (src < 0) {
    sign = 1;
    src = -src;
  }

  long int new = (long int)src;
  long int exp = 0;
  while (exp < 29 && src - ((float)new / (long int)(pow(10, exp))) != 0) {
    exp++;
    new = src *(long int)(pow(10, exp));
  }

  for (int i = 0; i < S21_DEC_SIZE; i++) {
    dst->bits[i] = 0;
  }

  dst->bits[0] = new;
  dst->bits[3] = (sign << 31) | (exp << 16);

  return S21_POSITIVE_RETURN;
}

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int res = S21_POSITIVE_RETURN;
  if (dst) {
    for (int i = 0; i < S21_DEC_SIZE; i++) {
      dst->bits[i] = 0;
    }
    if (src < 0) {
      dst->bits[3] = 1 << 31;
      src *= (-1);
    }
    dst->bits[0] = src;
  } else {
    res = S21_ERROR_RETURN;
  }
  return res;
}