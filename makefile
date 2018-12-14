all:
	@gcc control.c 2> wait
	@rm wait
	./a.out

setup:
	@gcc -c control.c 2> wait
	@gcc control.o
	@rm wait
	./a.out $(args)
run:
	@gcc -c sem.c 2> test
	@gcc sem.o
	@rm test
	./a.out

clean:
	rm a.out *.o textfile
