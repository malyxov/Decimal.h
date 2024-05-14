#include "s21_decimal_test.h"

START_TEST(simple_round) {
  // -39.104096094745301845239149102
  // 01111110 01011010 00100110 01110000
  // 11110011 01000010 11100010 00010000
  // 00001010 10100100 00110010 00101110
  // 10000000 00011011 00000000 00000000
  s21_decimal dec;
  s21_null_decimal(&dec);
  dec.bits[0] = 178532910;
  dec.bits[1] = 4081246736;
  dec.bits[2] = 2119837296;
  dec.bits[3] = 2149253120;

  s21_decimal ans;
  s21_null_decimal(&ans);

  // -39
  // 00000000 00000000 00000000 00000000
  // 00000000 00000000 00000000 00000000
  // 00000000 00000000 00000000 00100111
  // 10000000 00000000 00000000 00000000
  s21_decimal true_ans;
  s21_null_decimal(&true_ans);
  true_ans.bits[0] = 39;
  true_ans.bits[3] = MINUS_SIGN;

  int status = s21_round(dec, &ans);
  int true_status = 0;

  ck_assert_int_eq(1, s21_is_equal(ans, true_ans));
  ck_assert_int_eq(status, true_status);
}
END_TEST

START_TEST(simple_round_2) {
  // 0.91275848761238746318274123
  // 00000000 01001011 10000000 01101001
  // 10101101 00010110 00101101 10111100
  // 00100011 10110000 01111110 01001011
  // 00000000 00011010 00000000 00000000
  s21_decimal dec;
  s21_null_decimal(&dec);
  dec.bits[0] = 598769227;
  dec.bits[1] = 2903911868;
  dec.bits[2] = 4948073;
  dec.bits[3] = 1703936;

  s21_decimal ans;
  s21_null_decimal(&ans);

  s21_decimal true_ans;
  s21_null_decimal(&true_ans);
  true_ans.bits[0] = 1;
  int status = s21_round(dec, &ans);
  int true_status = 0;

  ck_assert_int_eq(1, s21_is_equal(ans, true_ans));
  ck_assert_int_eq(status, true_status);
}
END_TEST

START_TEST(simple_round_3) {
  // -0.91275848761238746318274123
  // 00000000 01001011 10000000 01101001
  // 10101101 00010110 00101101 10111100
  // 00100011 10110000 01111110 01001011
  // 10000000 00011010 00000000 00000000
  s21_decimal dec;
  s21_null_decimal(&dec);
  dec.bits[0] = 598769227;
  dec.bits[1] = 2903911868;
  dec.bits[2] = 4948073;
  dec.bits[3] = 1703936 + MINUS_SIGN;

  s21_decimal ans;
  s21_null_decimal(&ans);

  s21_decimal true_ans;
  s21_null_decimal(&true_ans);
  true_ans.bits[0] = 1;
  true_ans.bits[3] = MINUS_SIGN;
  int status = s21_round(dec, &ans);
  int true_status = 0;

  ck_assert_int_eq(1, s21_is_equal(ans, true_ans));
  ck_assert_int_eq(status, true_status);
}
END_TEST

START_TEST(simple_round_4) {
  // -263814.1627387128
  // 00000000 00001001 01011111 01100000
  // 00110110 10010001 01001100 11111000
  // 10000000 00001010 00000000 00000000
  s21_decimal dec;
  s21_null_decimal(&dec);
  dec.bits[0] = 915492088;
  dec.bits[1] = 614240;
  dec.bits[3] = 655360 + MINUS_SIGN;

  s21_decimal ans;
  s21_null_decimal(&ans);

  s21_decimal true_ans;
  s21_null_decimal(&true_ans);
  true_ans.bits[0] = 263814;
  true_ans.bits[3] = MINUS_SIGN;
  int status = s21_round(dec, &ans);
  int true_status = 0;

  ck_assert_int_eq(1, s21_is_equal(ans, true_ans));
  ck_assert_int_eq(status, true_status);
}
END_TEST

START_TEST(math_round) {
  // 0.49999999999999999
  // 00000000 10110001 10100010 10111100
  // 00101110 11000100 11111111 11111111
  // 00000000 00010001 00000000 00000000
  s21_decimal dec;
  s21_null_decimal(&dec);
  dec.bits[0] = 784662527;
  dec.bits[1] = 11641532;
  dec.bits[3] = 1114112;

  s21_decimal ans;
  s21_null_decimal(&ans);

  s21_decimal true_ans;
  s21_null_decimal(&true_ans);
  true_ans.bits[0] = 0;
  int status = s21_round(dec, &ans);
  int true_status = 0;

  ck_assert_int_eq(1, s21_is_equal(ans, true_ans));
  ck_assert_int_eq(status, true_status);
}
END_TEST

Suite* round_suite(void) {
  Suite* s;
  TCase* tc_core;

  s = suite_create("round");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, simple_round);
  tcase_add_test(tc_core, simple_round_2);
  tcase_add_test(tc_core, simple_round_3);
  tcase_add_test(tc_core, simple_round_4);
  tcase_add_test(tc_core, math_round);
  suite_add_tcase(s, tc_core);

  return s;
}
