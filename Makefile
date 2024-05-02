CC = /u/sw/toolchains/gcc-glibc/11.2.0/prefix/bin/g++
CFLAGS = -I. -Ishared-folder/try -Wall -Werror

SRCS = main.cpp SparseMatrix.hpp Sparse_implementation.hpp chrono.hpp
OBJS = $(SRCS:.cpp=.o)

all: main

main: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) main