#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <errno.h>		// Errors
#include <string.h>
#include <sys/epoll.h>  // for epoll()
#include <fcntl.h>      // for open()
#include <unistd.h>     // for close()
#include <stdbool.h>
#include "timeFunctions.c"
#include "buttonState.c"
#include "LED.c"
#include "GPIO_mode.c"

#define GPIO_export "/sys/class/gpio/export"
#define GPIO "/sys/class/gpio/gpio"
#define RED "/sys/class/gpio/gpio70"
#define GREEN "/sys/class/gpio71/"
#define BLUE "/sys/class/gpio72/"
#define YELLOW "/sys/class/gpio73/"
#define WHITE "/sys/class/gpio74/"


int main(){

    char config_pin[30];
    for(int i=41; i<47; i++){     // importing necessary pins as GPIO, inputmode
        sprintf(config_pin,"config-pin p8.%i gpio",i);  
        runCommand(config_pin); // tell BBG to use pin8.41 ~ 46 as GPIO      
        GPIO_inputmode(i+29);   // make GPIO_70 ~ 75 to an input mode 
    }
    
    LED_all_off();
    printf("Wait for the LED_Matrix to show the sequence of letters and then \n");
    printf("press the corresponding buttons when the LEDs are all on\n");
    sleepForMs(3000);

    for(int i=72; i<75; i++){     // checking early press
        char color_button_path [50];
        sprintf(color_button_path,"%s%i/value",GPIO,i);   // open up the gpio/value file
        if (isUserButtonPressed(color_button_path) == true){
            printf("You pressed too soon!\n");
            exit(1);
        }
    }

    /////////////////////////////
    //  LED MATRIX LOGIC HERE //
    ////////////////////////////
    LED_all_on();
    
    /////////////////////////////////
    //  Comparing the Button = LED //
    /////////////////////////////////

    
    //////////////////Button test//////////////
    while(1){
        for(int i=72; i<75; i++){     // checking early press
        char color_button_path [50];
        sprintf(color_button_path,"%s%i/value",GPIO,i);   // open up the gpio/value file
        if (isUserButtonPressed(color_button_path) == true){
            printf("GPIO %i is pressed!\n",i);            
        }
    }
    }
    /////////////////////////////////////////////






    sleepForUs(100);
    getTimeinUs();
    for(int i=41; i<46; i++){     // make gpio back to readmode after done
        GPIO_readmode(i+29);   
    }
    LED_all_return();
}