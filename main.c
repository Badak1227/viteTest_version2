#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include "io.h"

int main(char argc,char* argv[]) {
    int lines = 1, srCol = 100, srRow = 25;
    char* path = NULL;

    if (argc > 2) {
        printf("ERROR: TOO MANY PARAMETER");
        return 0;
    }
    else if (argc == 2) {
        path = (char*)malloc(sizeof(char) * (strlen(argv[1]) + 1));
        strcpy(path, argv[1]);
        
        path[strlen(argv[1])] = '\0';

        printf("%s", path);
    }
    
    keyInput(path, srCol, srRow, &lines);
}