exe: build_obj build_lib build_all

build_obj:
	gcc -c loglib.c

build_lib:
	ar rcs libloglib.a loglib.o

build_all:
	gcc -o main main.c -L. -lloglib