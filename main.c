#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "wordlist.h"

int main(char argc,char* argv[]) {
    int lines = 0;

    chunk* chunk_list = getChunk();

    readf(argv[1], chunk_list, &lines);

    system("pause");

    writef(argv[1], chunk_list);

    system("pause");

    free_chunk(chunk_list);
}