#include "s21_decimal.h"

int s21_floor(s21_decimal value, s21_decimal *res) {
  int return_code = 0;
  if (res) {
    s21_big_decimal big_value = s21_convert_to_big(value);
    while (big_value.exp > 0) div_by_ten(&big_value);
    if (big_value.sign &&
        s21_is_not_equal(s21_convert_big_to_std(big_value), value)) {
      s21_big_decimal one;
      s21_null_big_decimal(&one);
      one.bits[0] = 1;
      one.sign = 1;
      s21_add_big_decimal(one, big_value, &big_value);
    }
    *res = s21_convert_big_to_std(big_value);
  } else {
    return_code = 1;
  }
  return return_code;
}

int s21_round(s21_decimal value, s21_decimal *res) {
  int return_code = 0;
  if (res) {
    s21_big_decimal big_value = s21_convert_to_big(value);
    int mod = 0;
    while (big_value.exp > 0) mod = div_by_ten(&big_value);
    if (mod >= 5) {
      s21_big_decimal one;
      s21_null_big_decimal(&one);
      one.bits[0] = 1;
      one.sign = big_value.sign;
      s21_add_big_decimal(one, big_value, &big_value);
    }
    *res = s21_convert_big_to_std(big_value);
  } else {
    return_code = 1;
  }
  return return_code;
}

int s21_truncate(s21_decimal value, s21_decimal *res) {
  int return_code = 0;
  if (res) {
    s21_big_decimal big_value = s21_convert_to_big(value);
    while (big_value.exp > 0) div_by_ten(&big_value);
    *res = s21_convert_big_to_std(big_value);
  } else {
    return_code = 1;
  }
  return return_code;
}

int s21_negate(s21_decimal value, s21_decimal *result) {
  int return_code;
  if (result) {
    return_code = 0;
    value.bits[3] ^= MINUS_SIGN;
    *result = value;
  } else {
    return_code = 1;
  }
  return return_code;
}
