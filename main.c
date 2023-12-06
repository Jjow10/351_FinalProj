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
//#include "buttonState.h"

#define LED "/sys/class/leds/beaglebone:green:usr"
#define GPIO_export "/sys/class/gpio/export"
#define GPIO "/sys/class/gpio/gpio"
#define RED "/sys/class/gpio/gpio70"
#define GREEN "/sys/class/gpio71/"
#define BLUE "/sys/class/gpio72/"
#define YELLOW "/sys/class/gpio73/"
#define WHITE "/sys/class/gpio74/"
#define Init 0
#define Wait 1
#define Foul 2

static void runCommand(char* command){   // Execute the shell command (output into pipe)

    FILE *pipe = popen(command, "r");
    // Ignore output of the command; but consume it 
    // so we don't get an error when closing the pipe.
    char buffer[1024];
    while (!feof(pipe) && !ferror(pipe)) {
        if (fgets(buffer, sizeof(buffer), pipe) == NULL)
            break;
            // printf("--> %s", buffer); // Uncomment for debugging
    }
    // Get the exit code from the pipe; non-zero is an error:
    int exitCode = WEXITSTATUS(pclose(pipe));
    if (exitCode != 0) {
        perror("Unable to execute command:");
        printf(" command: %s\n", command);
        printf(" exit code: %d\n", exitCode);
    }
}

static int GPIO_inputmode(int GPIO_num){ // wait for the user to press GPIO_GPIO_num

    char address_gpio[50];
    sprintf(address_gpio,"%s%i/direction",GPIO,GPIO_num);   // open up the gpio/direction file
    FILE* pFile = fopen(address_gpio,"w");
    
    if(pFile ==NULL){
        printf("ERROR : gpio direction not created.\n");
        return -1;
    }
    else{
        fprintf(pFile,"in");                                    // change the direction to in / writing mode
        //printf("gpio%i is now in input mode\n",GPIO_num);     // uncomment for debugging
    }
    fclose(pFile);

    sleepForMs(300);  //wait 300ms before use
    return 0;
    
}

static int GPIO_readmode(int GPIO_num){  //make a gpio to read mode

    char address_gpio[50];
    sprintf(address_gpio,"%s%i/direction",GPIO,GPIO_num);  //open up the gpio/direction file
    FILE* pFile = fopen(address_gpio,"w");

    if(pFile ==NULL){
        printf("ERROR: gpio direction not created.\n");
        return -1;
    }
    else{
        fprintf(pFile,"out");                                   //change the direction to out / writing mode
        //printf("gpio%i is now in read mode\n",GPIO_num);
    }
    fclose(pFile);
    
    
    //wait 300ms before use
    sleepForMs(300);   
    return 0;
}

static bool isUserButtonPressed(char COLOR_BUTTON_PATH[]) {
    char button[5];

    FILE *pbuttonValueFile = fopen(COLOR_BUTTON_PATH, "r");
    if (pbuttonValueFile == NULL) {
        printf("ERROR OPENING %s.", COLOR_BUTTON_PATH);
        exit(1);
    }

    fflush(pbuttonValueFile);
    fseek(pbuttonValueFile, 0, SEEK_SET);
    fgets(button, sizeof(button), pbuttonValueFile);
    rewind(pbuttonValueFile);
    fclose(pbuttonValueFile);

    return atoi(button) == 0 ? true : false;
}

void LED_trigger(int LED_num,int status){ //revert triggers / brightness of LEDS
     
    char LEDtoRemove[50];   // trigger file location
    char LEDtoTurnOn[50];   // brightness file location
   
    sprintf(LEDtoRemove,"%s%i/trigger",LED,LED_num);
    sprintf(LEDtoTurnOn,"%s%i/brightness",LED,LED_num);

    FILE* pFile = fopen(LEDtoRemove,"w");  // trigger file
    FILE* wFile = fopen(LEDtoTurnOn,"w");  // brightness file 

   if(pFile == NULL || wFile == NULL){     //if any of the file is not opened
        printf("ERROR: LED trigger not opend / created.\n");
    }
    else{
        int charWritten;
        if (status == 0){                       // status 0 to turn everything off
            charWritten = fprintf(pFile,"none");// trigger turn off
            charWritten = fprintf(wFile,"0");   // brightness to 0
        }
        else if (status == 1){                  // if wants to turn on 
        charWritten = fprintf(wFile,"1");       // brightness to 1
        }
        else{                                   // if all done
        if(LED_num == 0)                        // return it to the original trigger mode
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

void LED_all_off(){
        LED_trigger(0,0);
        LED_trigger(1,0);
        LED_trigger(2,0);
        LED_trigger(3,0); 
}

void LED_all_return(){  //return all the LEDS to the original trigger mode
    LED_trigger(0,2); // LED0 = heartbeat
    LED_trigger(1,2); // LED1 = mmc0
    LED_trigger(2,2); // LED2 = cpu0
    LED_trigger(3,2); // LED3 = mmc1
}

void LED_all_on(){
    LED_trigger(0,1);
    LED_trigger(1,1);
    LED_trigger(2,1);
    LED_trigger(3,1);
}

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
    LED_all_on();
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
}
