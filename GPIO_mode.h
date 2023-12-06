//GPIO_mode.h
//Module to reset GPIO mode
#ifndef GPIO_mode_H
#define GPIO_mode_H
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#define GPIO_export "/sys/class/gpio/export"
#define GPIO "/sys/class/gpio/gpio"
#define RED "/sys/class/gpio/gpio70"
#define GREEN "/sys/class/gpio71/"
#define BLUE "/sys/class/gpio72/"
#define YELLOW "/sys/class/gpio73/"
#define WHITE "/sys/class/gpio74/"

static void runCommand(char* command);

static int GPIO_inputmode(int GPIO_num);

static int GPIO_readmode(int GPIO_num);


#endif

