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
    //�����ΰ� �Ʒ�����
    struct line* up;
    struct line* down;

    //���ڿ� ���� �迭, �迭 ũ��, �ؽ�Ʈ ����
    char* text;
    int arrSize;
    int textSize;
}line;

typedef struct chunk {
    //���� ûũ�� ���� ûũ
    struct chunk* prev;
    struct chunk* next;

    //ûũ ���� path
    char* path;

    //���� ����Ʈ�� ����Ʈ ũ��
    line* line_list;
    int listSize;

    //ûũ Ȱ��ȭ ����
    int state;
}chunk;

typedef struct cursor {
    //Ŀ�� ��ġ��
    int x;
    int y;

    //���� ���� ûũ
    chunk* curChunk;

    //���� ���� ����
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