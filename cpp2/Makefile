CC = clang++
CFLAGS = -g -Weverything -Wno-c++98-compat -Wno-padded -std=c++11 -O0 -I.
FILES = test_list.cpp
TARGET = bin

$(TARGET):
	$(CC) $(CFLAGS) $(FILES) -o $(TARGET)

test: $(TARGET);
	./$(TARGET) 20000

mtest: $(TARGET);
	valgrind --tool=memcheck --leak-check=full ./$(TARGET) 1000

debug: $(TARGET);
	lldb -- $(TARGET) 1000
