CC = gcc -std=c11
FLAGS = -Wall -Wextra -Werror
LIBS = --coverage -lcheck -lm -lpthread -lsubunit 
NAME = s21_decimal

all: test

clean:
	rm -rf ${NAME} *.o *.a *.gcno *.html *.gcda
	rm -rf ./report/
	rm -rf report.info

clang_test:
	clang-format -n -style=Google *.c *.h

s21_string.a:
	${CC} ${FLAGS} ${NAME}/*.c
	ar rc ${NAME}.a *.o
	ranlib ${NAME}.a

test:
	${CC} ${FLAGS} -o ${NAME} *.c tests/*.c ${LIBS}
	 ./${NAME}

gcov_report: clean test
	gcov -o report.info -c -d .
	lcov -t "${NAME}" -o report.info -c -q -d .
	genhtml -o report report.info
	open ./report/index.html
