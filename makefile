debug:
	clang -g rbtree.c 
	lldb a.out
clean:
	rm -rf a.out a.out.dSYM/
