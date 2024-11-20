FLAGS := -Wall -Werror -Wextra --std=gnu11 -lncurses
FLAGS_TESTS := -Wall -Werror -Wextra --std=gnu11
GCOV_FLAGS := -fprofile-arcs -ftest-coverage
CLANG_FLAGS := --style=Google --verbose
TEST_FLAGS := -lcheck
CFILES := tetris/main.c tetris/tetris.c gui/graphics.c tests/tests.c
HFILES := tetris/tetris.h

all: install

install: uninstall
	mkdir BrickGame
	gcc tetris/main.c tetris/tetris.c gui/graphics.c  -o BrickGame/tetris.out $(FLAGS)

uninstall:
	rm -rf BrickGame

dvi:
	$(MAKE) --directory=doxygen
	open dvi/html/index.html

test: clean tetris.a_tests
	gcc tests/tests.c tetris.a -o test.out $(FLAGS) $(TEST_FLAGS)
	./test.out

gcov_report: clean
	gcc tests/tests.c tetris/tetris.c -o gcov_report.out $(FLAGS) $(GCOV_FLAGS) $(TEST_FLAGS)
	./gcov_report.out
	lcov -t "brickgame" -o brickgame.info -c -d . -q
	genhtml -o report/html brickgame.info -q
	open report/html/index.html

tetris.a:
	gcc -c -o tetris.o tetris/tetris.c $(FLAGS)
	ar rcs tetris.a tetris.o

tetris.a_tests:
	gcc -c -o tetris.o tetris/tetris.c $(FLAGS_TESTS)
	ar rcs tetris.a tetris.o

clean:
	rm -f *.a *.o *.info *.gcda *.gcno gcov_report.out test.outm *.tar
	rm -rf report dvi

rebuild: clean test

clang_check:
	clang-format -n $(CLANG_FLAGS) $(CFILES) $(HFILES)
clang:
	clang-format -i $(CLANG_FLAGS) $(CFILES) $(HFILES)

valgrind: clean
	gcc tests/tests.c tetris/tetris.c -o test.out $(FLAGS) $(LIBCHECK)
	valgrind --tool=memcheck --leak-check=yes ./test.out
