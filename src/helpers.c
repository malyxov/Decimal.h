#include "s21_decimal.h"

bool s21_get_bit(unsigned int num, int pos) { return (num >> pos) & 1; }

void s21_null_decimal(s21_decimal *decimal) {
  decimal->bits[0] = 0;
  decimal->bits[1] = 0;
  decimal->bits[2] = 0;
  decimal->bits[3] = 0;
}

void s21_null_big_decimal(s21_big_decimal *big) {
  big->sign = 0;
  big->exp = 0;
  for (int i = 0; i < 192; i++) big->bits[i] = 0;
}

bool is_null(s21_big_decimal big) {
  bool result = 0;
  for (int i = 0; i < 192; i++) result += big.bits[i];
  return !result;
}

int last_bit(s21_big_decimal big) {
  int i = -1;
  if (!is_null(big)) {
    for (i = 191; i >= 0; i--)
      if (big.bits[i] == 1) break;
  }
  return i;
}

bool compare_bits(s21_big_decimal big1, s21_big_decimal big2) {
  int i = 191;
  while (i >= 0 && big1.bits[i] == big2.bits[i]) {
    i--;
  }
  return i == -1 ? 1 : big1.bits[i];
}

bool s21_get_sign(s21_decimal decimal) {
  return s21_get_bit(decimal.bits[3], 31);
}

int s21_decimal_get_power(s21_decimal decimal) {
  return (decimal.bits[3] >> 16) & 0xFF;
}

void s21_left_shift(s21_decimal *value) {
  s21_big_decimal big;
  s21_null_big_decimal(&big);
  big = s21_convert_to_big(*value);
  s21_left_shift_big(&big);
  *value = s21_convert_big_to_std(big);
}

void s21_right_shift_big(s21_big_decimal *big) {
  for (int i = 0; i < 191; i++) big->bits[i] = big->bits[i + 1];
  big->bits[191] = 0;
}

void s21_left_shift_big(s21_big_decimal *big) {
  for (int i = 191; i > 0; i--) big->bits[i] = big->bits[i - 1];
  big->bits[0] = 0;
}

void s21_leveling_decimal(s21_decimal *value_1, s21_decimal *value_2) {
  s21_big_decimal big_value_1 = s21_convert_to_big(*value_1);
  s21_big_decimal big_value_2 = s21_convert_to_big(*value_2);
  s21_leveling_big_decimal(&big_value_1, &big_value_2);
  *value_1 = s21_convert_big_to_std(big_value_1);
  *value_2 = s21_convert_big_to_std(big_value_2);
}

void s21_leveling_big_decimal(s21_big_decimal *big_value_1,
                              s21_big_decimal *big_value_2) {
  s21_big_decimal ten_power;
  int exp_dif = big_value_1->exp - big_value_2->exp;
  s21_null_big_decimal(&ten_power);
  ten_power.bits[1] = 1;
  ten_power.bits[3] = 1;
  if (big_value_1->exp > big_value_2->exp) {
    for (int i = 0; i < exp_dif; i++)
      s21_mul_big_decimal(*big_value_2, ten_power, big_value_2);
    if (last_bit(*big_value_2) > 95) {
      s21_leveling_alt_to_smaller(big_value_1, big_value_2);
    }
    big_value_2->exp = big_value_1->exp;
  } else if (big_value_2->exp > big_value_1->exp) {
    s21_leveling_big_decimal(big_value_2, big_value_1);
  }
}

void s21_leveling_alt_to_smaller(s21_big_decimal *value_1,
                                 s21_big_decimal *value_2) {
  int mod1 = 0, mod2 = 0;
  while (last_bit(*value_2) > 95) {
    mod1 = div_by_ten(value_1);
    mod2 = div_by_ten(value_2);
  }
  s21_bank_rounding(value_1, mod1);
  s21_bank_rounding(value_2, mod2);
  if (last_bit(*value_2) > 95) {
    mod1 = div_by_ten(value_1);
    mod2 = div_by_ten(value_2);
    s21_bank_rounding(value_1, mod1);
    s21_bank_rounding(value_2, mod2);
  }
}

void s21_normalize_big_decimal_exp(s21_big_decimal *big_value_1,
                                   s21_big_decimal *big_value_2) {
  s21_big_decimal ten = {.bits = {0, 1, 0, 1}};

  s21_big_decimal *larger_exp_value, *smaller_exp_value;

  if (big_value_1->exp > big_value_2->exp) {
    larger_exp_value = big_value_1;
    smaller_exp_value = big_value_2;
  } else {
    larger_exp_value = big_value_2;
    smaller_exp_value = big_value_1;
  }

  larger_exp_value->exp -= smaller_exp_value->exp;
  smaller_exp_value->exp = 0;

  while (larger_exp_value->exp != 0) {
    larger_exp_value->exp--;
    s21_mul_big_decimal(*smaller_exp_value, ten, smaller_exp_value);
  }
}

int div_by_ten(s21_big_decimal *big) {
  int remainder = 0;
  for (int i = 191; i >= 0; --i) {
    int cur = big->bits[i] + (remainder << 1);
    big->bits[i] = cur / 10;
    remainder = cur % 10;
  }
  big->exp--;
  return remainder;
}

void s21_bank_rounding(s21_big_decimal *alt, int mod) {
  if ((mod == 5 && alt->bits[0]) || mod > 5) {
    s21_big_decimal one;
    s21_null_big_decimal(&one);
    one.bits[0] = 1;
    one.exp = alt->exp;
    one.sign = alt->sign;
    s21_add_big_decimal(*alt, one, alt);
    if (last_bit(*alt) > 95) {
      s21_sub_big_decimal(*alt, one, alt);
      mod = div_by_ten(alt);
      s21_bank_rounding(alt, mod);
    }
  }
}

void align(s21_big_decimal *big_value_1, s21_big_decimal *big_value_2) {
  if (!is_null(*big_value_1) && !is_null(*big_value_2)) {
    int l1 = last_bit(*big_value_1);
    int l2 = last_bit(*big_value_2);
    while (last_bit(*big_value_1) != last_bit(*big_value_2))
      if (compare_bits(*big_value_1, *big_value_2))
        s21_left_shift_big(big_value_2);
      else
        s21_left_shift_big(big_value_1);
    if (!compare_bits(*big_value_1, *big_value_2))
      s21_left_shift_big(big_value_1);
    if (big_value_1->bits[0] == 0 && big_value_2->bits[0] == 0 &&
        last_bit(*big_value_1) > l1 && last_bit(*big_value_2) > l2) {
      s21_right_shift_big(big_value_1);
      s21_right_shift_big(big_value_2);
    }
  }
}
