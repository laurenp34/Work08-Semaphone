all: main.o write.o
	gcc -o control main.o
	gcc -o write write.o

main.o: main.c semaphore.c
	gcc -c main.c

write.o: write.c semaphore.c
	gcc -c write.c

clean:
	rm *.o
	rm *~
