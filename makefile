all:
	arm-linux-gnueabihf-gcc -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror -pthread main.c -static -o main 
	cp main $(HOME)/cmpt433/public/myApps/


