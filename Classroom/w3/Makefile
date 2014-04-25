ASSIGNMENT=integer_file

SRC_FILES=$(ASSIGNMENT).c \
          $(ASSIGNMENT)_main.c
          
TEST_FILES=../../Unity/unity.c \
           $(ASSIGNMENT).c \
           $(ASSIGNMENT)_test.c


C_COMPILER=gcc

ifeq ($(OS),Windows_NT)
	TARGET_EXTENSION=.exe
	CLEANUP=del /F /Q $(TARGET) && del /F /Q $(TEST)
    #do not define executable prefix: on Windows, . is in path
else
    #do not define target extension: on Linux it remains empty
	CLEANUP=rm -f $(TARGET) $(TEST)
    EXECUTABLE_PREFIX=./
endif

TARGET = $(ASSIGNMENT)$(TARGET_EXTENSION)
TEST_EXTENSION=_test
TEST = $(ASSIGNMENT)$(TEST_EXTENSION)$(TARGET_EXTENSION)

INC_DIRS=
TEST_INC_DIRS=$(INC_DIRS) -I../../Unity

SYMBOLS=-Wall -Werror -std=c99
TEST_SYMBOLS=$(SYMBOLS) -DTEST


all: default

test: Makefile $(TEST_FILES) *.h
	$(C_COMPILER) $(TEST_INC_DIRS) $(TEST_SYMBOLS) $(TEST_FILES) -o $(TEST)
	$(EXECUTABLE_PREFIX)$(TEST)

default: Makefile $(SRC_FILES) *.h
	$(C_COMPILER) $(INC_DIRS) $(SYMBOLS) $(SRC_FILES) -o $(TARGET)

clean:
	$(CLEANUP)
	
