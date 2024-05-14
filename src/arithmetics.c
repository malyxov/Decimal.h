#include "s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int return_code = 0;
  s21_decimal res;
  s21_null_decimal(&res);
  bool sign1 = s21_get_sign(value_1);
  bool sign2 = s21_get_sign(value_2);
  if (sign1 == sign2) {
    return_code = s21_add_handler(value_1, value_2, &res);
  } else {
    if (sign1) {
      s21_negate(value_1, &value_1);
      return_code = s21_sub(value_2, value_1, &res);
    } else {
      s21_negate(value_2, &value_2);
      return_code = s21_sub(value_1, value_2, &res);
    }
  }
  *result = res;
  return return_code;
}

int s21_add_handler(s21_decimal value_1, s21_decimal value_2,
                    s21_decimal *result) {
  int return_code = 0;
  s21_big_decimal big_value_1 = s21_convert_to_big(value_1);
  s21_big_decimal big_value_2 = s21_convert_to_big(value_2);
  s21_big_decimal big_res;
  s21_leveling_big_decimal(&big_value_1, &big_value_2);
  return_code = s21_add_big_decimal(big_value_1, big_value_2, &big_res);
  *result = s21_convert_big_to_std(big_res);
  return return_code;
}

int s21_add_big_decimal(s21_big_decimal value_1, s21_big_decimal value_2,
                        s21_big_decimal *result) {
  int return_code = 0;
  bool carry_bit = 0;
  s21_null_big_decimal(result);
  for (int i = 0; i < 192; i++) {
    result->bits[i] = (value_1.bits[i] ^ value_2.bits[i]) ^ carry_bit;
    if ((value_1.bits[i] & value_2.bits[i]) || (value_1.bits[i] & carry_bit) ||
        (value_2.bits[i] & carry_bit))
      carry_bit = 1;
    else
      carry_bit = 0;
  }
  result->exp = value_1.exp;
  result->sign = value_1.sign;
  if (last_bit(*result) > 95) {
    if (result->exp <= 0) {
      return_code = 1;
    } else {
      s21_bank_rounding(result, div_by_ten(result));
    }
  }
  return return_code;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int return_code = 0;
  s21_decimal res;
  s21_null_decimal(&res);
  bool sign1 = s21_get_sign(value_1);
  bool sign2 = s21_get_sign(value_2);

  bool areSignsEqual = (sign1 == sign2);
  bool isValue1GreaterOrEqual = s21_is_greater_or_equal(value_1, value_2);
  bool isValue1LessOrEqual = s21_is_less_or_equal(value_1, value_2);

  if (areSignsEqual) {
    if ((isValue1GreaterOrEqual && !sign1) || (isValue1LessOrEqual && sign1)) {
      return_code = s21_sub_handler(value_1, value_2, &res);
    } else {
      sign1 = sign1 ^ 1;
      sign2 = sign2 ^ 1;
      return_code = s21_sub_handler(value_2, value_1, &res);
    }
    if (s21_get_sign(res) != sign1) s21_negate(res, &res);
  } else {
    s21_negate(value_2, &value_2);
    return_code = s21_add(value_1, value_2, &res);
  }
  *result = res;

  return return_code;
}

int s21_sub_handler(s21_decimal value_1, s21_decimal value_2,
                    s21_decimal *result) {
  int return_code = 0;
  s21_big_decimal big_value_1 = s21_convert_to_big(value_1);
  s21_big_decimal big_value_2 = s21_convert_to_big(value_2);
  s21_big_decimal big_res;
  s21_null_big_decimal(&big_res);
  s21_leveling_big_decimal(&big_value_1, &big_value_2);
  return_code = s21_sub_big_decimal(big_value_1, big_value_2, &big_res);
  big_res.exp = big_value_1.exp;
  *result = s21_convert_big_to_std(big_res);
  return return_code;
}

int s21_sub_big_decimal(s21_big_decimal value_1, s21_big_decimal value_2,
                        s21_big_decimal *result) {
  int return_code = 0;
  bool carry_bit = 0;

  for (int i = 0; i < 192; i++) {
    bool bit1 = value_1.bits[i];
    bool bit2 = value_2.bits[i];

    result->bits[i] = bit1 ^ bit2 ^ carry_bit;

    if (!bit1 && bit2)
      carry_bit = 1;
    else if (bit1 && !bit2)
      carry_bit = 0;
  }

  return return_code;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int return_code = 0;
  s21_big_decimal big_value_1 = s21_convert_to_big(value_1);
  s21_big_decimal big_value_2 = s21_convert_to_big(value_2);
  s21_big_decimal res;
  s21_null_big_decimal(&res);
  return_code = s21_mul_big_decimal(big_value_1, big_value_2, &res);
  *result = s21_convert_big_to_std(res);
  return return_code;
}

int s21_mul_big_decimal(s21_big_decimal value_1, s21_big_decimal value_2,
                        s21_big_decimal *result) {
  int return_code = 0;
  s21_null_big_decimal(result);
  if (!is_null(value_1) && !(is_null(value_2))) {
    for (int i = 0; i < 192; i++) {
      if (value_2.bits[i] == 1) s21_add_big_decimal(*result, value_1, result);
      s21_left_shift_big(&value_1);
    }
    result->exp = value_1.exp + value_2.exp;
    result->sign = value_1.sign ^ value_2.sign;
    if (last_bit(*result) > 95 || result->exp > 28) {
      int mod = 0;
      while (result->exp > 0 && (last_bit(*result) > 95 || result->exp > 28)) {
        if (result->exp == 0) break;
        mod = div_by_ten(result);
      }
      if (result->exp > 0)
        s21_bank_rounding(result, mod);
      else
        return_code = 1;
    }
    if (is_null(*result)) return_code = 2;
  }
  return return_code;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int return_code = 0;
  if (value_2.bits[0] == 0 && value_2.bits[1] == 0 && value_2.bits[2] == 0) {
    return_code = 3;
  } else if (value_1.bits[0] == 0 && value_1.bits[1] == 0 &&
             value_1.bits[2] == 0) {
    s21_null_decimal(result);
  } else {
    s21_big_decimal big_value_1 = s21_convert_to_big(value_1);
    s21_big_decimal big_value_2 = s21_convert_to_big(value_2);
    s21_big_decimal res;
    s21_null_big_decimal(&res);
    while (big_value_1.bits[0] == 0 && big_value_2.bits[0] == 0) {
      s21_right_shift_big(&big_value_1);
      s21_right_shift_big(&big_value_2);
    }
    return_code = s21_div_big_decimal(big_value_1, big_value_2, &res);
    *result = s21_convert_big_to_std(res);
  }
  return return_code;
}

int s21_div_big_decimal(s21_big_decimal big_value_1,
                        s21_big_decimal big_value_2, s21_big_decimal *result) {
  int return_code = 0;
  int exp = 0;
  int status = 0;
  s21_big_decimal ten = {.bits = {0, 1, 0, 1}};
  s21_big_decimal modulo;
  s21_null_big_decimal(&modulo);
  s21_null_big_decimal(result);
  bool sign = big_value_1.sign ^ big_value_2.sign;
  big_value_1.sign = 0;
  big_value_2.sign = 0;

  s21_normalize_big_decimal_exp(&big_value_1, &big_value_2);
  modulo = div_with_rem(big_value_1, big_value_2, &big_value_1);
  while (!is_null(big_value_1) || !is_null(modulo)) {
    exp++;
    s21_mul_big_decimal(modulo, ten, &modulo);
    s21_mul_big_decimal(*result, ten, result);
    status = s21_add_big_decimal(*result, big_value_1, result);
    modulo = div_with_rem(modulo, big_value_2, &big_value_1);
    if (status == 1) break;
  }

  return_code = handle_status(status, &exp, result);
  result->sign = sign;
  return return_code;
}

int handle_status(int status, int *exp, s21_big_decimal *result) {
  int mod;
  int return_code = 0;
  if (status == 1) {
    mod = div_by_ten(result);
    while (last_bit(*result) > 95 && *exp > 0) {
      mod = div_by_ten(result);
      (*exp)--;
    }
    s21_bank_rounding(result, mod);
    (*exp)--;
  }
  result->exp = *exp - 1;
  mod = 10;
  while (result->exp > 28) mod = div_by_ten(result);
  if (is_null(*result))
    return_code = 2;
  else if (mod != 10)
    s21_bank_rounding(result, mod);
  if (last_bit(*result) > 95) return_code = 1;
  return return_code;
}

s21_big_decimal div_with_rem(s21_big_decimal big_value_1,
                             s21_big_decimal big_value_2,
                             s21_big_decimal *result) {
  int denominator_left_bit = last_bit(big_value_2);
  s21_big_decimal modulo;
  s21_null_big_decimal(&modulo);
  s21_null_big_decimal(result);
  if (compare_bits(big_value_1, big_value_2)) {
    align(&big_value_1, &big_value_2);
  }
  for (int i = 0; i < 191; i++) {
    if (compare_bits(big_value_1, big_value_2)) {
      s21_sub_big_decimal(big_value_1, big_value_2, &big_value_1);
      result->bits[0] = 1;
    }
    if (last_bit(big_value_2) <= denominator_left_bit) break;
    if (last_bit(big_value_2) > denominator_left_bit)
      s21_right_shift_big(&big_value_2);
    s21_left_shift_big(result);
  }
  return big_value_1;
}
