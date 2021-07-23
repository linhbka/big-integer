libbinteger.a: binteger.o
	ar rcs libbinteger.a binteger.o
binteger.o: binteger.c binteger.h
	gcc -c binteger.c
clean:
	rm binteger.o
libs:
	mv libbinteger.a /usr/local/lib/
