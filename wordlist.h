#ifndef __WORDLIST_H__
#define __WORDLIST_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct coordinate {
    int x;
    int y;
}cd;

typedef struct line {
    struct line* up;
    struct line* down;

    char* text;

    int arrSize;
    int textSize;
}line;

typedef struct chunk {
    FILE* fp;
    char* path;

    struct chunk* prev;
    struct chunk* next;

    line* line_list;

    int listSize;

    int state;
}chunk;

typedef struct cursor {
    //커서 위치값
    int x;
    int y;

    //현재 참조 청크
    chunk* curChunk;

    //현재 참조 라인
    int line;
}cursor;

char* getUniqueP();

void readf(char* path, chunk* curChunk, int* lines);

void writef(char* path, chunk* curChunk);

void chunkOn(cursor* curCur);

void chunkOff(cursor* curCur);

void free_line(line* line_list);

void free_chunk(chunk* chunk_list);

line* getLine();

void addWord(char input, cd* cur, chunk* curChunk, line* curRow);

void bsWord(cd* cur, chunk* curChunk, line* curRow);

chunk* getChunk();

void divChunk(chunk* curChunk, line* curRow);

void mergeChunk(chunk* curChunk, line* curRow);

void search();


#endif