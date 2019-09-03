run: run.sh main.c token.c gen_x86.c
	gcc -o main main.c token.c gen_x86.c
	./run.sh

clean:
	rm -f main tmp*
