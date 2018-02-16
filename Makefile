
CC?=gcc
CFLAGS=-std=c90
CFLAGS+=-ansi
CFLAGS+=-Wall
CFLAGS+=-Wextra
CFLAGS+=-pedantic
CFLAGS+=-pedantic-errors
CFLAGS+=-O3
LDFLAGS=

INC_DIR=include
LIB_DIR=lib
BUILD_DIR=build
#TEST_BUILD_DIR=build/test
#EXAMPLE_BUILD_DIR=build/example

TEST_LIST=$(BUILD_DIR)/build_test
TEST_LIST+=$(BUILD_DIR)/empty_crawl_test
TEST_LIST+=$(BUILD_DIR)/no_option_test
TEST_LIST+=$(BUILD_DIR)/short_option_crawl_test
TEST_LIST+=$(BUILD_DIR)/short_option_with_parameter_crawl_test
TEST_LIST+=$(BUILD_DIR)/short_option_with_more_parameter_crawl_test
TEST_LIST+=$(BUILD_DIR)/long_option_crawl_test
TEST_LIST+=$(BUILD_DIR)/long_option_with_parameter_crawl_test
TEST_LIST+=$(BUILD_DIR)/long_option_with_more_parameter_crawl_test

CFLAGS+=-I$(INC_DIR)
LDFLAGS+=-L$(LIB_DIR)

$(LIB_DIR)/%.o: src/%.c
	@mkdir -p $(LIB_DIR)
	$(CC) -o $@ -c $< $(CFLAGS)

$(BUILD_DIR)/%: test/%.c
	# Always rebuild from source
	@mkdir -p $(BUILD_DIR)
	$(CC) `( cat $(<).defines 2>/dev/null )` -o $@ src/optread.c $< $(CFLAGS) $(LDFLAGS)

all: $(LIB_DIR)/optread.o $(TEST_LIST)
	echo "Done"

.PHONY: clean mrproper run_tests lib_folder build_folder

run_tests:
	for i in `find build -name "*_test*"`; do if [ -x "$$i" ]; then echo "Running $$i"; $$i; retval=$$?; if [ $$retval -ne 0 ]; then echo "	Return value error: $$retval" >&2; fi fi done

clean:
	rm -rf `find . -name "*.o"`

mrproper: clean
	rm -rf build/*

