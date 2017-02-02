CC = clang++
# CFLAGS = -g -pg -Weverything -Wno-c++98-compat -Wno-padded -Wno-float-equal -std=c++11 -O0 -I. -ferror-limit=200 -DMY_DEBUG
CFLAGS = -Weverything -Wno-c++98-compat -Wno-padded -Wno-float-equal -std=c++11 -O3 -I.
# FILES = test_adj_matrix.cpp
FILES = SAGRP4.cpp
TARGET = bin

$(TARGET):
	$(CC) $(CFLAGS) $(FILES) -o $(TARGET)

test: $(TARGET);
	./$(TARGET) < p4d0in.txt

mtest: $(TARGET);
	valgrind --tool=memcheck --leak-check=full ./$(TARGET) 100

debug: $(TARGET);
	lldb -- $(TARGET) 500
