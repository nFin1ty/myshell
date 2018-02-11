CC=gcc

all: myshell

myshell: myshell.c
		$(CC) -o myshell myshell.c
		 
clean:
		rm myshell
