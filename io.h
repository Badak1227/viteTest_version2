#ifndef __IO_H__
#define __IO_H__
#include <stdio.h>
#include <conio.h>
#include "wordlist.h"
#include "ui.h"
void readf(char* path, chunk* curChunk, int* lines);

void writef(char* path, chunk* curChunk);

void keyInput(char* path, int srCol, int srRow, int* lines);
#endif