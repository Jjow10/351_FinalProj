#include <errno.h>  // Errors
#include <fcntl.h>  // for open()
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>  // for epoll()
#include <time.h>
#include <unistd.h>  // for close()

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

#define BUFFER_SIZE 2000

int main() {
    char config_pin[50];

    for (int i = 41; i < 45; i++) {  // importing necessary pins as GPIO, inputmode
        sprintf(config_pin, "config-pin p8.%i gpio", i);
        runCommand(config_pin);  // Set pins 8.41 - 8.44 as GPIO
        GPIO_inputmode(i + 31);  // Set GPIO_72 - 75 to input mode
    }

    LED_all_off();
    printf("Wait for the LED_Matrix to show the sequence of letters and then \n");
    printf("press the corresponding buttons when the LEDs are all on\n");
    sleepForMs(3000);

    for (int i = 72; i < 75; i++) {  // checking early press
        char color_button_path[50];
        sprintf(color_button_path, "%s%i/value", GPIO, i);  // open up the gpio/value file
        if (isButtonPressed(color_button_path) == true) {
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
    bool gameIsNotOver = true;
    int patternSequence[BUFFER_SIZE];
    int freePosition = 0;
    while (gameIsNotOver) {
        srand(time(NULL));
        // Generate random integer between 0 and 3
        int random_colour = rand() % 3;
        // Append generated colour to patternSequence
        patternSequence[freePosition] = random_colour;
        // printf("%d", patternSequence[freePosition]);
        freePosition++;

        // TODO: Display the patternSequence array using LED

        // Get user button input
        bool stateUserInput = true;
        int lastButtonPressed = -1;
        int userSequenceIndex = -1;

        while (stateUserInput && userSequenceIndex < freePosition - 1) {
            if (isButtonPressed(RED) && lastButtonPressed != 0) {
                lastButtonPressed = 0;
                userSequenceIndex++;
                printf("RED is pressed!\n");
                if (patternSequence[userSequenceIndex] == 0) {
                    continue;
                } else {
                    // TODO: Display X
                    exit(0);
                }
            } else if (isButtonPressed(GREEN) && lastButtonPressed != 1) {
                lastButtonPressed = 1;
                userSequenceIndex++;
                printf("GREEN is pressed!\n");
                if (patternSequence[userSequenceIndex] == 1) {
                    continue;
                } else {
                    // TODO: Display X
                    exit(0);
                }
            } else if (isButtonPressed(BLUE) && lastButtonPressed != 2) {
                lastButtonPressed = 2;
                printf("BLUE is pressed!\n");
                stateUserInput = false;
            } else if (isButtonPressed(YELLOW) && lastButtonPressed != 3) {
                lastButtonPressed = 3;
                printf("YELLOW is pressed!\n");
                stateUserInput = false;
            }
        }

        // Uncomment if button testing needed
        /*
        //////////////////Button test//////////////
        while (1) {
            if (isButtonPressed(RED)) {
                printf("RED is pressed!\n");
            } else if (isButtonPressed(GREEN)) {
                printf("GREEN is pressed!\n");
            } else if (isButtonPressed(BLUE)) {
                printf("BLUE is pressed!\n");
            } else if (isButtonPressed(YELLOW)) {
                printf("YELLOW is pressed!\n");
            }
        }
        /////////////////////////////////////////////
        */

        sleepForUs(100);
        getTimeinUs();
        for (int i = 41; i < 45; i++) {  // make gpio back to readmode after done
            GPIO_readmode(i + 31);
        }
        LED_all_return();
    }
}