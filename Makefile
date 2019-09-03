run: run.sh main.c token.c
	gcc -o main main.c token.c
	./run.sh

clean:
	rm -f main tmp*
