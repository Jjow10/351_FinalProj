#include "buttonState.h"

#define BUFFER_SIZE 2000

bool isButtonPressed(char* colourPath) {
    char buff[BUFFER_SIZE];

    FILE *pbuttonValueFile = fopen(colourPath, "r");
    if (pbuttonValueFile == NULL) {
        printf("ERROR OPENING %s", colourPath);
        exit(1);
    }

    fflush(pbuttonValueFile);
    fseek(pbuttonValueFile, 0, SEEK_SET);
    fgets(buff, 1024, pbuttonValueFile);
    rewind(pbuttonValueFile);
    fclose(pbuttonValueFile);

    return atoi(buff) == 0 ? true : false;
}