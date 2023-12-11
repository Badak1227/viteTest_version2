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

typedef struct sr {
    struct sr* prev;
    cursor* search_result;
    struct sr* next;
}sr;

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

sr* search(char* target, chunk* chunk_list, int* searchCount);

void check_cursor(cd* startCd, cursor* curCur, cd* endCd) {
    if (curCur->x < startCd->x) {

        startCd->x = curCur->x;
        endCd->x = startCd->x + 99;
    }
    else if (curCur->x > endCd->x - 2) {
        endCd->x = curCur->x + 2;
        startCd->x = endCd->x - 99;
    }

    if (curCur->y < startCd->y) {
        startCd->y = curCur->y;
        endCd->y = startCd->y + 22;
    }
    else if (curCur->y > endCd->y) {
        endCd->y = curCur->y;
        startCd->y = endCd->y - 22;
    }

}

cursor* get_cursor(chunk* chunk_list) {
    cursor* tmp = (cursor*)malloc(sizeof(cursor));

    tmp->x = 0;
    tmp->y = 0;

    tmp->curChunk = chunk_list;

    tmp->line = 0;

    return tmp;
}

char* getUniquePath() {
    char buffer[80] = "./Chunks/Chunk";
    int idx = 14;
    static int chunkNum = 1;
    int num = chunkNum;

    chunkNum++;

    while (num != 0) {
        buffer[idx++] = num % 10 + '0';

        num /= 10;
    }
    buffer[idx] = '\0';

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

    tmp->path = getUniquePath();

    fp = fopen(tmp->path, "wt");;
    fclose(fp);

    tmp->prev = NULL;
    tmp->next = NULL;

    tmp->line_list = NULL;

    tmp->listSize = 0;

    tmp->state = 0;

    return tmp;
}

void chunkSave(chunk* curChunk) {
    FILE* fp = fopen(curChunk->path, "wt");

    line* curLine = curChunk->line_list;

    while (curLine != NULL) {
        fwrite(curLine->text, 1, curLine->textSize, fp);

        curLine = curLine->down;
    }

    fclose(fp);
}

void chunkOn(chunk* curChunk) {
    //현재 청크 state가 비활성화 되있을 시 활성화
    if (curChunk->state == 0) {
        FILE* fp = fopen(curChunk->path, "rt");

        char input = fgetc(fp);
        line* tmpLine;

        curChunk->line_list = getLine();
        curChunk->listSize++;
        tmpLine = curChunk->line_list;

        while (input != EOF) {
            if (tmpLine->textSize > 0) {
                if (tmpLine->text[tmpLine->textSize - 1] == '\n') {
                    tmpLine->down = getLine();

                    tmpLine->down->up = tmpLine;
                    tmpLine = tmpLine->down;

                    curChunk->listSize++;
                }
            }

            if (tmpLine->textSize == tmpLine->arrSize) {
                tmpLine->text = (char*)realloc(tmpLine->text, sizeof(char) * (tmpLine->arrSize + 100));

                tmpLine->arrSize += 100;
            }

            tmpLine->text[tmpLine->textSize] = input;
            tmpLine->textSize++;

            input = fgetc(fp);
        }

        fclose(fp);
    }

    curChunk->state = 1;
}

void chunkOff(chunk* curChunk) {
    //현재 청크 state가 활성화 되있을 시 비활성화
    if (curChunk->state == 1) {
        curChunk->listSize = 0;

        chunkSave(curChunk);

        free_line(&(curChunk->line_list));
    }

    curChunk->state = 0;
}

void free_line(line** line_list) {
    line* curLine = *line_list;
    line* tmp;

    while (curLine != NULL) {
        tmp = curLine;

        curLine = curLine->down;

        free(tmp);
    }

    *line_list = NULL;
}

void free_chunk(chunk** chunk_list) {
    chunk* curChunk = *chunk_list;
    chunk* tmp;

    while (curChunk != NULL) {
        
        tmp = curChunk;

        free_line(&(curChunk->line_list));

        remove(tmp->path);
        
        curChunk = curChunk->next;

        free(tmp);
    }

    *chunk_list = NULL;
}

void divChunk(cursor* curCur) {
    chunk* curChunk = curCur->curChunk;

    chunk* newChunk = getChunk();
    line* next_list = curChunk->line_list;

    //divide 기준 라인
    int sepLine = curChunk->listSize / 2;

    for (int i = 1; i <= sepLine + 1; i++) {
        next_list = next_list->down;
    }

    //새 청크 활성화
    newChunk->state = 1;

    //현재 커서가 참조해야할 청크 판단
    int side = curCur->line < sepLine ? 0 : 1;


    //청크 연결
    newChunk->prev = curChunk;
    newChunk->next = curChunk->next;
    curChunk->next = newChunk;
    if (newChunk->next != NULL) newChunk->next->prev = newChunk;

    //라인리스트 청크에 할당
    newChunk->line_list = next_list;

    //청크간 라인리스트 연결 삭제
    next_list->up->down = NULL;
    next_list->down = NULL;

    newChunk->listSize = curChunk->listSize - sepLine;
    curChunk->listSize = sepLine;

    if (side == 0) {
        if (newChunk->next != NULL) chunkOff(newChunk->next);
    }
    else {
        if (curChunk->prev != NULL) chunkOff(curChunk->prev);

        curCur->curChunk = newChunk;
        curCur->line = curCur->line + 1 - sepLine;
    }
}

void mergeChunk(cursor* curCur) {
    chunk* curChunk = curCur->curChunk;
    line* curLine = curChunk->line_list;

    //다음 청크가 존재하는 경우
    if (curChunk->next != NULL) {
        chunk* targetChunk = curChunk->next;

        line* connect1 = curChunk->line_list;
        line* connect2 = targetChunk->line_list;

        while (connect1->down != NULL) {
            connect1 = connect1->down;
        }

        //두 청크간 line_list 연결
        connect1->down = connect2;
        connect2->up = connect1;

        //청크 재연결
        curChunk->next = targetChunk->next;
        if (targetChunk->next != NULL) {
            targetChunk->next->prev = curChunk;
        }

        curChunk->listSize += targetChunk->listSize;

        //대상 청크 지우기
        targetChunk->prev = NULL;
        targetChunk->next = NULL;
        targetChunk->line_list = NULL;

        remove(targetChunk->path);

        free(targetChunk->path);
        free(targetChunk);

        //다음 청크 활성화
        if (curChunk->next != NULL) chunkOn(curChunk->next);
    }

    //현 청크 사이즈 오버시 divChunk 실행
    if (curChunk->listSize > 46) divChunk(curCur);
}

void addWord(char input, cursor* curCur, int* lines) {
    chunk* curChunk = curCur->curChunk;
    line* curLine = curChunk->line_list;
    int curCol = curCur->x;

    curCur->x++;

    for (int i = 0; i < curCur->line; i++) {
        curLine = curLine->down;
    }
    
    //라인 배열 메모리 조절
    if (curLine->textSize == curLine->arrSize) {
        curLine->arrSize += +100;

        curLine->text = (char*)realloc(curLine->text, curLine->arrSize);
    }

    //배열 한칸씩 밀기
    for (int i = curLine->textSize; i >= curCol; i--) {
        curLine->text[i] = curLine->text[i - 1];
    }
    curLine->text[curCol] = input;
    curLine->textSize++;

    if (input == '\n') {
        line* tmpLine = getLine();
        (*lines)++;

        curCur->x = 0;
        curCur->y++;
        curCur->line++;

        tmpLine->down = curLine->down;
        curLine->down = tmpLine;
        tmpLine->up = curLine;
        if (tmpLine->down != NULL) tmpLine->down->up = tmpLine;

        
        tmpLine->textSize = curLine->textSize - curCol - 1;
        tmpLine->arrSize = (((tmpLine->textSize - 1) / 100) + 1) * 100;
        tmpLine->text = (char*)realloc(tmpLine->text, tmpLine->arrSize);

        for (int i = curCol, j = 0; i < curLine->textSize; i++, j++) {
            tmpLine->text[j] = curLine->text[i];
        }

        curLine->textSize = curCol;
        curLine->arrSize = (((curLine->textSize - 1) / 100) + 1) * 100;
        curLine->text = (char*)realloc(curLine->text, curLine->arrSize);

        curChunk->listSize++;
    }

    if (curChunk->listSize > 46) {
        divChunk(curCur);
    }
}

void bsWord(cursor* curCur, int* lines) {
    chunk* curChunk = curCur->curChunk;
    line* curLine = curChunk->line_list;
    int curCol = curCur->x;

    for (int i = 0; i < curCur->line; i++) {
        curLine = curLine->down;
    }

    //라인의 처음에서 백스페이스 시 동작
    if (curCol == 0) {
        //청크의 가장 첫라인이고 이전 청크가 존재할 경우 이전 청크에 현재 청크 병합
        if (curLine->up == NULL && curChunk->prev != NULL) {
            chunk* prevChunk = curChunk->prev;
            line* prevLine = prevChunk->line_list;
            
            while (prevLine->down != NULL) {
                prevLine = prevLine->down;
            }
            curCol = prevLine->textSize - 1;

            //커서 수정
            curCur->curChunk = prevChunk;
            curCur->line = prevChunk->listSize - 1;
            curCur->x = curCol;
            curCur->y--;
            (*lines)--;

            //이전 라인에 현재 라인 병합
            prevLine->down = curLine->down;
            if (prevLine->down != NULL) prevLine->down->up = prevLine;

            prevLine->textSize = curLine->textSize + curCol;
            prevLine->arrSize = (((prevLine->textSize - 1) / 100) + 1) * 100;
            prevLine->text = (char*)realloc(prevLine->text, prevLine->arrSize);

            for (int i = 0, j = curCol; i < curLine->textSize; i++, j++) {
                prevLine->text[j] = curLine->text[i];
            }
            
            //이전 청크에 현재 청크 병합
            prevChunk->next = curChunk->next;
            prevChunk->listSize += curChunk->listSize - 1;
            if (prevChunk->next != NULL) prevChunk->next->prev = prevChunk;

            if (prevChunk->prev != NULL) chunkOn(prevChunk->prev);

            //현재 청크 프리
            remove(curChunk->path);
            
            free(curLine);
            free(curChunk->path);
            free(curChunk);

            if (curCur->curChunk->listSize > 46) divChunk(curCur);
        }
        //청크의 첫 라인이 아닐 경우
        else if (curLine->up != NULL) {
            line* prevLine = curLine->up;
            curCol = prevLine->textSize - 1;
            (*lines)--;
            
            //커서 수정
            curCur->x = curCol;
            curCur->y--;
            curCur->line--;

            //이전 라인과 현재 라인 병합
            prevLine->down = curLine->down;
            if (prevLine->down != NULL) prevLine->down->up = prevLine;

            prevLine->textSize = curLine->textSize + curCol;
            prevLine->arrSize = (((prevLine->textSize - 1) / 100) + 1) * 100;
            prevLine->text = (char*)realloc(prevLine->text, prevLine->arrSize);

            for (int i = 0, j = curCol; i < curLine->textSize; i++, j++) {
                prevLine->text[j] = curLine->text[i];
            }


            curChunk->listSize--;

            free(curLine);

            if (curChunk->listSize < 23) mergeChunk(curCur);
        }
    }
    else {
        for (int i = curCol; i < curLine->textSize; i++) {
            curLine->text[i - 1] = curLine->text[i];
        }

        curLine->textSize--;
        curCur->x--;

        //텍스트 배열 크기 조절
        if (curLine->textSize % 100 == 0 && curLine->textSize > 0) {
            curLine->arrSize -= 100;
            curLine->text = (char*)realloc(curLine->text, curLine->arrSize);
        }
    }
}

void up(cursor* curCur) {
    chunk* curChunk = curCur->curChunk;
    line* curLine = curCur->curChunk->line_list;

    //현재 청크의 첫라인이 아닌 경우
    if (curCur->line > 0) {
        curCur->y--;
        curCur->line--;
    }
    //현재 청크의 첫라인이며 이전 청크가 존재하는 경우
    else if (curCur->curChunk->prev != NULL) {
        curLine = curChunk->prev->line_list;

        curCur->curChunk = curCur->curChunk->prev;
        curCur->y--;
        curCur->line = curCur->curChunk->listSize - 1;

        if (curCur->curChunk->prev != NULL) chunkOn(curCur->curChunk->prev);
        if (curChunk->next != NULL) chunkOff(curChunk->next);
    }

    for (int i = 0; i < curCur->line; i++) {
        curLine = curLine->down;
    }

    if(curCur->x > curLine->textSize) curCur->x = curLine->textSize;

    if (curLine->text[curCur->x - 1] == '\n') curCur->x--;
}

void down(cursor* curCur) {
    chunk* curChunk = curCur->curChunk;
    line* curLine = curChunk->line_list;

    //아래 라인이 존재하는 경우
    if (curCur->line < curChunk->listSize -1) {
        curCur->y++;
        curCur->line++;
    }
    //아래 라인이 존재하지 않으며 다음 청크가 존재하는 경우
    else if (curChunk->next != NULL) {
        curLine = curChunk->next->line_list;

        curCur->curChunk = curChunk->next;
        curCur->y++;
        curCur->line = 0;

        if (curChunk->prev != NULL) chunkOff(curChunk->prev);
        if (curCur->curChunk->next != NULL) chunkOn(curCur->curChunk->next);
    }

    for (int i = 0; i < curCur->line; i++) {
        curLine = curLine->down;
    }

    if (curCur->x > curLine->textSize) curCur->x = curLine->textSize;

    if (curLine->text[curCur->x - 1] == '\n') curCur->x--;
}

void left(cursor* curCur) {
    if (curCur->x > 0) curCur->x--;
}

void right(cursor* curCur) {
    line* curLine = curCur->curChunk->line_list;

    for (int i = 0; i < curCur->line; i++) {
        curLine = curLine->down;
    }

    if (curCur->x < curLine->textSize && curLine->text[curCur->x] != '\n') curCur->x++;
}

void home(cursor* curCur) {
    curCur->x = 0;
}

void end(cursor* curCur) {
    chunk* curChunk = curCur->curChunk;
    line* curLine = curChunk->line_list;

    for (int i = 0; i < curCur->line; i++) {
        curLine = curLine->down;
    }

    curCur->x = curLine->textSize;

    if (curLine->text[curCur->x - 1] == '\n') curCur->x--;
}

void pgUp(cd* startCd, cursor* curCur) {
    if (startCd->y > 0) {
        int gap = (curCur->y - startCd->y) + 23;

        for (int i = 0; i < gap; i++) {
            up(curCur);
        }

        curCur->x = 0;
    }
}

void pgDown(cd* startCd, cursor* curCur, cd* endCd, int* lines) {
    if (endCd->y + 1 < *lines) {
        int gap = (endCd->y - curCur->y) + 1;

        for (int i = 0; i < gap; i++) {
            down(curCur);
        }

        curCur->x = 0;
        startCd->y = curCur->y;
        endCd->y = startCd->y + 22;
    }
}

sr* search(char* target, chunk* chunk_list, int* searchCount) {

}