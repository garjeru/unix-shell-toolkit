#Makefile

CC=gcc

mytoolkit: mytoolkit.c
	$(CC) -Wall -ansi -pedantic -o mytoolkit mytoolkit.c