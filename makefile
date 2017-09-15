w4118_sh : shell.c shell.h list.o
			cc -o w4118_sh shell.c list.o -std=c11

list.o : list.c list.h shell.h
			cc -c list.c