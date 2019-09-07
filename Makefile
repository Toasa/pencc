run:
	gcc -o main main.c token.c parse.c gen_x86.c util.c
	./run.sh

clean:
	rm -f main tmp*
