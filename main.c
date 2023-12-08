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
#include "LED.h"
#include "buttonState.h"
#include "displayLED.h"
#include "timeFunctions.h"

/**
 * BUTTON WIRING:
 * RED -> P_8.41
 * YELLOW -> P_8.42
 * GREEN -> P_8.43
 * BLUE -> P_8.44
 */
#define GPIO_export "/sys/class/gpio/export"
#define GPIO "/sys/class/gpio/gpio"
#define RED_BUTTON_VALUE "/sys/class/gpio/gpio74/value"
#define YELLOW_BUTTON_VALUE "/sys/class/gpio/gpio75/value"
#define GREEN_BUTTON_VALUE "/sys/class/gpio/gpio72/value"
#define BLUE_BUTTON_VALUE "/sys/class/gpio/gpio73/value"

#define BUFFER_SIZE 2000
#define ROW_FOR_X 4

enum Colours { RED, YELLOW, GREEN, BLUE };

int main() {
    initDisplay();
    clearMatrixDisplay();
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

    /////////////////////////////
    //  LED MATRIX LOGIC HERE //
    ////////////////////////////config-p
    LED_all_on();

    /////////////////////////////////
    //  Comparing the Button = LED //
    /////////////////////////////////
    bool gameOver = false;
    int patternSequence[BUFFER_SIZE];
    int sequenceSize = 0;
    int record = -1;
    int LED_Display_Delay = 1500;
    int LED_OFF_Delay = 500;
    srand(time(NULL));
    while (!gameOver) {
        sequenceSize++;
        record++;
        if(sequenceSize % 2 == 0 && LED_Display_Delay >= 500){
            LED_Display_Delay -= 250;
            LED_OFF_Delay -= 50;
        }
        // Generate random integer between 0 and 3
        patternSequence[sequenceSize - 1] = rand() % 4;
        for (int i = 0; i < sequenceSize; ++i) {
            LED_all_off();
            displayLetters(patternSequence[i]);
            sleepForMs(LED_Display_Delay);
            clearMatrixDisplay();
            sleepForMs(LED_OFF_Delay);
        }
        printf("\nRound %i!\n", record + 1);
        printf("Press now!\n");
        // Get user button input
        int userSequenceIndex = -1;

        LED_all_on();
        while ((userSequenceIndex < sequenceSize - 1) && !gameOver) {
            if (isButtonPressed(RED_BUTTON_VALUE)) {
                userSequenceIndex++;
                printf("RED is pressed!\n");
                sleepForMs(500);
                if (patternSequence[userSequenceIndex] == RED) {
                    continue;
                } else {
                    displayLetters(ROW_FOR_X);
                    sleepForMs(1500);
                    printf("Your record is %i!\n", record);
                    gameOver = true;
                }
            } else if (isButtonPressed(YELLOW_BUTTON_VALUE)) {
                userSequenceIndex++;
                printf("YELLOW is pressed!\n");
                sleepForMs(500);
                if (patternSequence[userSequenceIndex] == YELLOW) {
                    continue;
                } else {
                    displayLetters(ROW_FOR_X);
                    sleepForMs(1500);
                    printf("Your record is %i!\n", record);
                    gameOver = true;
                }
            } else if (isButtonPressed(GREEN_BUTTON_VALUE)) {
                userSequenceIndex++;
                printf("GREEN is pressed!\n");
                sleepForMs(500);
                if (patternSequence[userSequenceIndex] == GREEN) {
                    continue;
                } else {
                    displayLetters(ROW_FOR_X);
                    sleepForMs(1500);
                    printf("Your record is %i!\n", record);
                    gameOver = true;
                }
            } else if (isButtonPressed(BLUE_BUTTON_VALUE)) {
                userSequenceIndex++;
                printf("BLUE is pressed!\n");
                sleepForMs(500);
                if (patternSequence[userSequenceIndex] == BLUE) {
                    continue;
                } else {
                    displayLetters(ROW_FOR_X);
                    sleepForMs(1500);
                    printf("Your record is %i!\n", record);
                    gameOver = true;
                }
            }
        }

        // Uncomment if button testing needed
        /*
        //////////////////Button test//////////////
        while (1) {
            if (isButtonPressed(RED)) {
                printf("RED is pressed!\n");
            } else if (isButtonPressed(GREEN_BUTTON_VALUE)) {
                printf("GREEN_BUTTON_VALUE is pressed!\n");
            } else if (isButtonPressed(BLUE)) {
                printf("BLUE is pressed!\n");
            } else if (isButtonPressed(YELLOW_BUTTON_VALUE)) {
                printf("YELLOW is pressed!\n");
            }
        }
        /////////////////////////////////////////////
        */
    }

    sleepForUs(100);
    getTimeinUs();
    for (int i = 41; i < 45; i++) {  // make gpio back to readmode after done
        GPIO_readmode(i + 31);
    }
    LED_all_off();
    LED_all_return();
}