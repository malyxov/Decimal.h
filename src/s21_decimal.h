#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_

#include <stdbool.h>
#include <stdio.h>

#define MINUS_SIGN 2147483648

#define S21_DEC_SIZE 4
#define S21_POSITIVE_RETURN 1
#define S21_ERROR_RETURN 0

#define S21_INT_MIN -2147483648
#define S21_INT_MAX 2147483647
#define S21_UNSIGNED_INT_MAX 4294967295
#define S21_DEC_MIN -79228162514264337593543950335
#define S21_DEC_MAX 79228162514264337593543950335

typedef struct s21_decimal {
  unsigned int bits[4];
} s21_decimal;

typedef struct s21_big_decimal {
  bool bits[192];
  bool sign;
  int exp;
} s21_big_decimal;

typedef enum {
  CONVERTATION_OK = 0,
  CONVERTATION_ERROR = 1
} convertation_result;

//===========ARITHMETICS===========

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_add_handler(s21_decimal value_1, s21_decimal value_2,
                    s21_decimal *result);
int s21_add_big_decimal(s21_big_decimal value_1, s21_big_decimal value_2,
                        s21_big_decimal *result);

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub_handler(s21_decimal value_1, s21_decimal value_2,
                    s21_decimal *result);
int s21_sub_big_decimal(s21_big_decimal value_1, s21_big_decimal value_2,
                        s21_big_decimal *result);

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul_big_decimal(s21_big_decimal value_1, s21_big_decimal value_2,
                        s21_big_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div_big_decimal(s21_big_decimal alt_value_1,
                        s21_big_decimal alt_value_2,
                        s21_big_decimal *alt_result);
int handle_status(int status, int *exp, s21_big_decimal *alt_result);
s21_big_decimal div_with_rem(s21_big_decimal alt_value_1,
                             s21_big_decimal alt_value_2,
                             s21_big_decimal *alt_result);

//===========COMPARISON===========

int s21_is_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_less(s21_decimal value_1, s21_decimal value_2);
int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2);

//===========CONVERTORS===========

s21_big_decimal s21_convert_to_big(s21_decimal dec);
s21_decimal s21_convert_big_to_std(s21_big_decimal big);

//===========HELPERS===========

bool s21_get_bit(unsigned int num, int pos);
void s21_null_decimal(s21_decimal *decimal);
void s21_null_big_decimal(s21_big_decimal *big);
bool is_null(s21_big_decimal big);
int last_bit(s21_big_decimal big);
bool compare_bits(s21_big_decimal big1, s21_big_decimal big2);
bool s21_get_sign(s21_decimal decimal);
int s21_decimal_get_power(s21_decimal decimal);
void s21_right_shift_big(s21_big_decimal *big);
void s21_left_shift_big(s21_big_decimal *big);
void s21_leveling_decimal(s21_decimal *value_1, s21_decimal *value_2);
void s21_leveling_big_decimal(s21_big_decimal *big_value_1,
                              s21_big_decimal *big_value_2);
void s21_leveling_alt_to_smaller(s21_big_decimal *alt_value_1,
                                 s21_big_decimal *alt_value_2);
void s21_normalize_big_decimal_exp(s21_big_decimal *big_value_1,
                                   s21_big_decimal *big_value_2);
int div_by_ten(s21_big_decimal *big);
void s21_bank_rounding(s21_big_decimal *alt, int mod);
void s21_left_shift(s21_decimal *value);
void align(s21_big_decimal *big_value_1, s21_big_decimal *big_value_2);

//===========CONVERSION===========

int s21_from_decimal_to_float(s21_decimal src, float *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_int_to_decimal(int src, s21_decimal *dst);

//===========OTHERS===========

int s21_floor(s21_decimal value, s21_decimal *res);
int s21_round(s21_decimal value, s21_decimal *res);
int s21_truncate(s21_decimal value, s21_decimal *res);
int s21_negate(s21_decimal value, s21_decimal *result);

#endif  // SRC_S21_DECIMAL_H_