printfile : printfile.o printline.o
	gcc -o printfile printfile.o printline.o
printfile.o : printfile.c
	gcc -c printfile.c
printline.o : printline.c
	gcc -c printline.c
clean:
	rm -f *.o