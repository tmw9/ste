project: main.o ebuffer.o editor_config.o
	cc main.o ebuffer.o editor_config.o -o project -lncurses
main.o: main.c ebuffer.h editor_config.h
	cc -c main.c
ebuffer.o: ebuffer.c ebuffer.h
	cc -c ebuffer.c
editor_config.o: editor_config.c editor_config.h
	cc -c editor_config.c
