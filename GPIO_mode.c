#include "GPIO_mode.h"

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
