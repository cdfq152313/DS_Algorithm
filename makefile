all: rbtree.c
	clang rbtree.c
	./a.out 
memory:
	clang -g rbtree.c
	valgrind --leak-check=yes ./a.out
debug:
	clang -g rbtree.c 
	lldb ./a.out
clean:
	rm -rf a.out a.out.dSYM/
