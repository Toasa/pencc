run: main run.sh
	gcc -o main main.c
	./run.sh

clean:
	rm -f main tmp*
