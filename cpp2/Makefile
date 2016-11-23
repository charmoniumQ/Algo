CC = clang++
CFLAGS = -g -Weverything -Wno-c++98-compat -Wno-padded -std=c++11 -O0 -I.
#FILES = test_adj_matrix.cpp
FILES = test_array.cpp
TARGET = bin

$(TARGET):
	$(CC) $(CFLAGS) $(FILES) -o $(TARGET)

test: $(TARGET);
	./$(TARGET) 500

mtest: $(TARGET);
	valgrind --tool=memcheck --leak-check=full ./$(TARGET) 100

debug: $(TARGET);
	lldb -- $(TARGET) 500
