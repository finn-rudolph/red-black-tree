CC = clang
CFLAGS = -Wall -g

obj = red_black_tree.o main.o

all: $(obj)
	$(CC) $(CFLAGS) $(obj) -o rbtree

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $^

clean:
	rm *.o *.gch rbtree