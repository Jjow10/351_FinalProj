#include "LED.h"

void LED_trigger(int LED_num, int status) {  // revert triggers / brightness of LEDS

    char LEDtoRemove[50];  // trigger file location
    char LEDtoTurnOn[50];  // brightness file location

    sprintf(LEDtoRemove, "%s%i/trigger", LED, LED_num);
    sprintf(LEDtoTurnOn, "%s%i/brightness", LED, LED_num);

    FILE* pFile = fopen(LEDtoRemove, "w");  // trigger file
    FILE* wFile = fopen(LEDtoTurnOn, "w");  // brightness file

    if (pFile == NULL || wFile == NULL) {  // if any of the file is not opened
        printf("ERROR: LED trigger not opend / created.\n");
    } else {
        int charWritten;
        if (status == 0) {                         // status 0 to turn everything off
            charWritten = fprintf(pFile, "none");  // trigger turn off
            charWritten = fprintf(wFile, "0");     // brightness to 0
        } else if (status == 1) {                  // if wants to turn on
            charWritten = fprintf(wFile, "1");     // brightness to 1
        } else {                                   // if all done
            if (LED_num == 0)                      // return it to the original trigger mode
                charWritten = fprintf(pFile, "heartbeat");
            else if (LED_num == 1)
                charWritten = fprintf(pFile, "mmc0");
            else if (LED_num == 2)
                charWritten = fprintf(pFile, "cpu0");
            else if (LED_num == 3)
                charWritten = fprintf(pFile, "mmc1");
        }
        if (charWritten <= 0) {
            printf("ERROR: Wrong trigger check error.\n");
            exit(1);
        }
    }
    fclose(pFile);
    fclose(wFile);
}

void LED_all_off() {
    LED_trigger(0, 0);
    LED_trigger(1, 0);
    LED_trigger(2, 0);
    LED_trigger(3, 0);
}

void LED_all_return() {  // return all the LEDS to the original trigger mode
    LED_trigger(0, 2);   // LED0 = heartbeat
    LED_trigger(1, 2);   // LED1 = mmc0
    LED_trigger(2, 2);   // LED2 = cpu0
    LED_trigger(3, 2);   // LED3 = mmc1
}

void LED_all_on() {
    LED_trigger(0, 1);
    LED_trigger(1, 1);
    LED_trigger(2, 1);
    LED_trigger(3, 1);
}