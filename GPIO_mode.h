//GPIO_mode.h
//Module to reset GPIO mode
#ifndef GPIO_mode_H
#define GPIO_mode_H
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#define GPIO "/sys/class/gpio/gpio"


void runCommand(char* command);

int GPIO_inputmode(int GPIO_num);

int GPIO_readmode(int GPIO_num);


#endif

