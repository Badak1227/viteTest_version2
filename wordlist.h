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
    //윗라인과 아래라인
    struct line* up;
    struct line* down;

    //문자열 저장 배열, 배열 크기, 텍스트 길이
    char* text;
    int arrSize;
    int textSize;
}line;

typedef struct chunk {
    //이전 청크와 다음 청크
    struct chunk* prev;
    struct chunk* next;

    //청크 고유 path
    char* path;

    //라인 리스트와 리스트 크기
    line* line_list;
    int listSize;

    //청크 활성화 상태
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

void check_cursor(cd* startCd, cursor* curCur, cd* endCd);

cursor* get_cursor(chunk* chunk_list);

char* getUniquePath();

line* getLine();

chunk* getChunk();

void chunkOn(chunk* curChunk);

void chunkOff(chunk* curChunk);

void free_line(line** line_list);

void free_chunk(chunk** chunk_list);


void divChunk(cursor* curCur);

void mergeChunk(cursor* curCur);

void addWord(char input, cursor* curCur, int* lines);

void bsWord(cursor* curCur, int* lines);

void up(cursor* curCur);

void down(cursor* curCur);

void left(cursor* curCur);

void right(cursor* curCur);

void home(cursor* curCur);

void end(cursor* curCur);

void pgUp(cd* startCd, cursor* curCur);

void pgDown(cd* startCd, cursor* curCur, cd* endCd, int* lines);

#endif