//LED.h
//Module to control beaglebone LEDs
#ifndef LED_H
#define LED_H
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#define LED "/sys/class/leds/beaglebone:green:usr"

void LED_trigger(int LED_num,int status);

void LED_all_off();

void LED_all_return();

void LED_all_on();

#endif