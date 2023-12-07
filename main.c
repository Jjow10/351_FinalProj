#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <errno.h>		// Errors
#include <string.h>
#include <sys/epoll.h>  // for epoll()
#include <fcntl.h>      // for open()
#include <unistd.h>     // for close()
#include <stdbool.h>
#include "timeFunctions.h"
#include "buttonState.h"
#include "LED.h"
#include "GPIO_mode.h"
#include "displayLED.h"
/**
 * BUTTON WIRING:
 * RED -> P_8.41
 * YELLOW -> P_8.42
 * GREEN -> P_8.43
 * BLUE -> P_8.44
*/
#define GPIO_export "/sys/class/gpio/export"
#define GPIO "/sys/class/gpio/gpio"
#define RED "/sys/class/gpio/gpio74/value"
#define YELLOW "/sys/class/gpio/gpio75/value"
#define GREEN "/sys/class/gpio/gpio72/value"
#define BLUE "/sys/class/gpio/gpio73/value"

int main(){

    char config_pin[50];
    for(int i=41; i<45; i++) {     // importing necessary pins as GPIO, inputmode
        sprintf(config_pin,"config-pin p8.%i gpio",i);
        runCommand(config_pin); // Set pins 8.41 - 8.44 as GPIO
        GPIO_inputmode(i+31);   // Set GPIO_72 - 75 to input mode
    }
    
    LED_all_off();
    printf("Wait for the LED_Matrix to show the sequence of letters and then \n");
    printf("press the corresponding buttons when the LEDs are all on\n");
    sleepForMs(3000);

    for(int i=72; i<75; i++){     // checking early press
        char color_button_path [50];
        sprintf(color_button_path,"%s%i/value",GPIO,i);   // open up the gpio/value file
        if (isButtonPressed(color_button_path) == true){
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
    ;

    while(1) {
        if (isButtonPressed(RED)) {
            printf("RED is pressed!\n");
        }
        else if (isButtonPressed(GREEN)) {
            printf("GREEN is pressed!\n");
        }
        else if (isButtonPressed(BLUE)) {
            printf("BLUE is pressed!\n");
        }
        else if (isButtonPressed(YELLOW)) {
            printf("YELLOW is pressed!\n");
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