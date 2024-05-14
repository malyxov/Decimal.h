#ifndef SRC_TESTS_S21_DECIMAL_TEST_H_
#define SRC_TESTS_S21_DECIMAL_TEST_H_

#include <check.h>

#include "../s21_decimal.h"

Suite* comparsion_suite(void);
Suite* conversion_suite(void);
Suite* add_suite(void);
Suite* sub_suite(void);
Suite* mul_suite(void);
Suite* div_suite(void);
Suite* negate_suite(void);
Suite* truncate_suite(void);
Suite* round_suite(void);
Suite* floor_suite(void);
#endif  // SRC_TESTS_S21_DECIMAL_TEST_H_
