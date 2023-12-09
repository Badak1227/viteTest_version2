#define _CRT_SECURE_NO_WARNINGS
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
    //청크 고유 path
    char* path;

    //이전 청크와 다음 청크
    struct chunk* prev;
    struct chunk* next;

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



char* getUniqueP() {
    char buffer[80] = "Chunk";
    int idx = 5;
    static char chunkNum = 1;
    char num = chunkNum;

    chunkNum++;

    while (num != 0) {
        buffer[idx++] = num % 10 + '0';

        num /= 10;
    }

    char* path = (char*)malloc(strlen(buffer) + 1);
    strcpy(path, buffer);

    return path;
}

line* getLine() {
    line* tmp = (line*)malloc(sizeof(line));

    tmp->up = NULL;
    tmp->down = NULL;

    tmp->text = (char*)malloc(sizeof(char) * 100);

    tmp->arrSize = 100;
    tmp->textSize = 0;

    return tmp;
}

chunk* getChunk() {
    chunk* tmp = (chunk*)malloc(sizeof(chunk));
    FILE* fp;

    tmp->path = getUniqueP();

    fp = fopen(tmp->path, "wt");;
    fclose(fp);

    tmp->prev = NULL;
    tmp->next = NULL;

    tmp->line_list = NULL;

    tmp->listSize = 0;

    return tmp;
}

void readf(char* path,chunk* curChunk, int* lines) {
    FILE* fp = fopen(path, "rt");

    chunk* tmpChunk = curChunk;
    char ch = fgetc(fp);
    int c_lines = 1;

    FILE* c_fp = fopen(tmpChunk->path, "wt");

    while (ch != EOF) {
        fputc(ch, c_fp);

        if (ch == '\n') {
            *lines++;
            c_lines++;
        }

        if (c_lines > 40) {
            fclose(c_fp);

            tmpChunk->next = getChunk();
            tmpChunk->next->prev = tmpChunk;

            tmpChunk = tmpChunk->next;

            c_fp = fopen(tmpChunk->path, "wt");

            c_lines = 1;
        }

        ch = fgetc(fp);
    }
    
    fclose(fp);
    fclose(c_fp);
}

void writef(char* path, chunk* curChunk) {
    chunk* tmpChunk = curChunk;
    char buffer[100] = {0};

    FILE* fp = fopen(path, "wt");

    while (tmpChunk != NULL) {
        FILE* c_fp = fopen(tmpChunk->path, "rt");

        int readChars = fread(buffer, 1, 100, c_fp);

        while (readChars > 0) {
            printf("%s\n", tmpChunk->path);

            fwrite(buffer, 1, 100, fp);

            readChars = fread(buffer, 1, 100, c_fp);
        }

        fclose(c_fp);

        tmpChunk = tmpChunk->next;
    }
    
    fclose(fp);
}

void chunkOn(cursor* curCur) {
    chunk* curChunk = curCur->curChunk;

    //현재 청크 state가 비활성화 되있을 시 line_list 생성
    if (curChunk->state == 0) {
        FILE* fp = fopen(curChunk->path, "rt");

        char input = fgetc(fp);
        line* tmpLine;

        curChunk->line_list = getLine();
        curChunk->listSize++;
        tmpLine = curChunk->line_list;

        while (input != EOF) {

            if (tmpLine->textSize == tmpLine->arrSize) {
                tmpLine->text = (char*)realloc(tmpLine->text, sizeof(char) * (tmpLine->arrSize + 100));

                tmpLine->arrSize += 100;
            }

            tmpLine->text[tmpLine->textSize] = input;
            tmpLine->textSize++;

            if (input == '\n') {
                tmpLine->down = getLine();

                tmpLine->down->up = tmpLine;
                tmpLine = tmpLine->down;

                curChunk->listSize++;

            }
        }
    }

    curChunk->state = 1;
}

void chunkOff(cursor* curCur) {
    chunk* curChunk = curCur->curChunk;

    curChunk->state = 0;

    //현재 청크 state가 비활성화로 전환 되었을 시 line_list 생성
    if (curChunk->state == 0) {
        FILE* fp = fopen(curChunk->path, "wt");

        line* tmpLine = curChunk->line_list;

        while (tmpLine != NULL) {
            for (int i = 0; i < tmpLine->textSize; i++) {
                fputc(tmpLine->text[i], fp);
            }

            tmpLine = tmpLine->down;
        }

        curChunk->listSize = 0;

        free_line(curChunk->line_list);

        fclose(fp);
    }
}

void free_line(line* line_list) {
    line* tmp;

    while (line_list != NULL) {
        tmp = line_list;

        line_list = line_list->down;

        free(tmp);
    }
}

void free_chunk(chunk* chunk_list) {
    chunk* tmp;

    while (chunk_list != NULL) {
        tmp = chunk_list;

        free_line(chunk_list->line_list);

        remove(chunk_list->path);

        chunk_list = chunk_list->next;

        free(tmp);
    }
}

void addWord(char input, cd* cur, chunk* curChunk, line* curRow) {
    if (input == '\n') {
        line* tmp = getLine();

        curChunk->listSize++;

        while(curRow->textSize - cur->x < tmp->arrSize){
            tmp->text = (char*)realloc(tmp->text, sizeof(char) * (tmp->arrSize + 100));

            tmp->arrSize += 100;
        }

        for (int i = cur->x; i < curRow->textSize; i++) {
            tmp->text[i - cur->x] = curRow->text[i];
        }

        curRow->text[cur->x] = input;

        curRow->textSize = cur->x + 1;

        while (curRow->textSize < curRow->arrSize) {
            if (curRow->arrSize == 100) break;

            curRow->text = (char*)realloc(curRow->text, sizeof(char) * (curRow->arrSize - 100));
            
            curRow->arrSize -= 100;
        }

        curRow->down->up = tmp;
        tmp->down = curRow->down;

        curRow->down = tmp;
        tmp->up = curRow;
        
        curRow = tmp;

        curChunk->listSize++;

        cur->x = 0;
        cur->y++;
    }
    else {
        char tmp;

        curRow->textSize++;
        
        if (curRow->arrSize == curRow->textSize) {
            curRow->text = (char*)realloc(curRow->text, sizeof(char) * (curRow->arrSize + 100));

            curRow->arrSize += + 100;
        }

        for (int i = curRow->textSize - 1; i >= cur->x; i--) {
            curRow->text[i] = curRow->text[i - 1];
        }
        curRow->text[cur->x] = input;

        cur->x++;
    }

    if (curChunk->listSize > 46) {
        //divChunk();
    }
}

void bsWord(cd* cur, chunk* curChunk, line* curRow) {

}





void divChunk(chunk* curChunk, line* curRow) {
    int side = 1;

    line* next_list = curChunk->line_list;
    
    chunk* newChunk = getChunk();

    newChunk->path = getUniqueP();


    for (int i = 0; i <= 23; i++) {

        if (next_list == curRow) side = 0;
        
        getChunk();
    }
}

void mergeChunk(chunk* curChunk, line* curRow) {
    if (curChunk->next != NULL) {
        chunk* targetChunk = curChunk->next;

        line* connect1 = curChunk->line_list;

        line* connect2 = targetChunk->line_list;

        //두 청크간 line_list 연결
        while (connect1->down != NULL) {
            connect1 = connect1->down;
        }

        connect1->down = connect2->down;
        connect2->down->up = connect1->up;
        
        curChunk->listSize += targetChunk->listSize;
        
        //청크 재연결
        curChunk->next = targetChunk->next;
        if (targetChunk->next != NULL) {
            targetChunk->next->prev = curChunk;
        }

        //대상 청크 비우기&지우기
        targetChunk->prev = NULL;
        targetChunk->next = NULL;
        targetChunk->line_list = NULL;

        if (targetChunk->state > 0) {
            fclose(targetChunk->path);
        }

        remove(targetChunk->path);

        free(targetChunk->path);
        free(targetChunk);

        //현 청크 사이즈 오버시 divChunk 실행
        if (curChunk->listSize > 46) divChunk(curChunk, curRow);
    }

}

void search(cd start, cd cur, cd end, char* target, chunk* curChunk) {

}
