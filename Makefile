all:
	arm-linux-gnueabihf-gcc -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror -pthread main.c GPIO_mode.c LED.c timeFunctions.c buttonState.c displayLED.c -static -o main 
	cp main $(HOME)/cmpt433/public/myApps/

clean:
	rm -rf main
