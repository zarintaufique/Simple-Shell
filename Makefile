output:	main.o functionality.o 
	gcc main.o functionality.o -o output

main.o:	main.c
	gcc -c main.c

function.o: functionality.c header.h
	gcc -c functionality.c

clean: rm *.o output


