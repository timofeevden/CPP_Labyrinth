CXX := g++
CXXFLAGS :=-Wall -Werror -Wextra -std=c++17
LDFLAGS := -lgtest

VIEW_DIR := ./View
CONTROLLER_DIR := ./Controller
MODEL_DIR := ./Model
TESTS_DIR := ./Tests
BUILD_DIR := ./Build
INSTALL_DIR := $(HOME)/Maze_Application
SRC := ./*.cc
TESTS := $(wildcard $(TESTS_DIR)/*.cc)

UNAME :=$(shell uname -s)

VERSION = `date +"%Y.%m.%d"`
ARCHIVE = MAZE-$(VERSION)

ifeq ($(UNAME),Linux)
	LDFLAGS += -lsubunit
	OPEN_CM=xdg-open
endif
ifeq ($(UNAME),Darwin)
	OPEN_CM=open
endif


.PHONY: all clean
all: clean build

build:
	mkdir -p $(BUILD_DIR)
	cmake -S . -B $(BUILD_DIR)
	make -C $(BUILD_DIR)

install:
	mkdir -p $(INSTALL_DIR)
	cmake -S . -B $(INSTALL_DIR)
	make -C $(INSTALL_DIR)

uninstall:
	rm -rf $(INSTALL_DIR)

dvi: 
	$(OPEN_CM) documentation.md

dist:
	tar -cf $(ARCHIVE).tar Controller Model View Tests CMakeLists.txt Makefile documentation.md s21_main.cc

tests: clean
	$(CXX) $(CXXFLAGS) -g3 --coverage -c $(TESTS) ./Model/s21_*.cc
	$(CXX) $(CXXFLAGS) *.o $(LDFLAGS) -g3 --coverage -o ./Tests/s21_run_tests 
	./Tests/s21_run_tests 

gcov_report: tests
	./Tests/s21_run_tests
	lcov -t "test" -o test.info --no-external -c -d .
	genhtml -o report test.info
	open ./report/index.html

valgrind: tests
	valgrind --tool=memcheck --leak-check=yes --leak-check=full -s Tests/s21_run_tests 

style:
	cp ../materials/linters/.clang-format .
	clang-format -n $(SRC) $(HEADER) $(TESTS)
	rm -rf .clang-format

edit_style:
	cp ../materials/linters/.clang-format .
	clang-format -i $(SRC) $(HEADER) $(TESTS)
	rm -rf .clang-format

cpp_check:
	cppcheck --language=c++ --suppress=missingIncludeSystem ./Model/*.cc ./Model/*.h ./Controller/*.cc ./Controller/*.h ./View/*.cc ./View/*.h ./Tests/*.cc ./Tests/*.h

clean:
	rm -rf *.a *.o *.out *.gch *.gcno *.gcna *.gcda *.info *.tgz *.user report ./Tests/s21_run_tests dist_maze/

realclean: clean
	rm -rf $(BUILD_DIR) *.tar
