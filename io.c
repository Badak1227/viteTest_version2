#define _CRT_SECURE_NO_WARNINGS
#define CTRL_S 19
#define CTRL_Q 17
#define ENTER 13
#define BACKSPACE 8

#define HOME 71
#define END 79
#define PG_UP 73
#define PG_DOWN 81
#define ESC 27
#define SPECIAL -32
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77

#include <stdio.h>
#include <conio.h>
#include "wordlist.h"
#include "ui.h"

void readf(char* path, chunk* curChunk, int* lines) {
	FILE* fp = fopen(path, "rt");

	chunk* tmpChunk = curChunk;
	char ch = fgetc(fp);
	int c_lines = 1;

	int now = 1;
	fseek(fp, 0, SEEK_END);
	int end = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	FILE* c_fp = fopen(tmpChunk->path, "wt");

	char buffer[1024] = { 0 };

	while (fgets(buffer, 1024, fp) != NULL) {
		now = ftell(fp);

		if (buffer[strlen(buffer) - 1] == '\n') {
			(*lines)++;

			c_lines++;
		}

		fputs(buffer, c_fp);

		if (c_lines > 46) {
			if (c_fp != NULL) {
				fclose(c_fp);
				c_fp = NULL;
			}

			tmpChunk->next = getChunk();
			tmpChunk->next->prev = tmpChunk;

			tmpChunk = tmpChunk->next;

			c_fp = fopen(tmpChunk->path, "wt");

			c_lines = 1;
		}

		fprintf(stdout, "\r Now Loading.. %d / %d", now, end);
	}

	fclose(fp);
	fclose(c_fp);
}

void writef(char* path, chunk* chunk_list, int lines) {
	chunk* tmpChunk = chunk_list;
	char buffer[1024] = { 0 };

	int now = 0;

	FILE* fp = fopen(path, "wt");

	gotoXY(99, 24);

	while (tmpChunk != NULL) {
		FILE* c_fp = fopen(tmpChunk->path, "rt");

		int readChars = fread(buffer, sizeof(char), 1024, c_fp);

		while (readChars > 0) {

			fwrite(buffer, 1, readChars, fp);

			readChars = fread(buffer, sizeof(char), 1024, c_fp);
		}

		fclose(c_fp);

		tmpChunk = tmpChunk->next;

		fprintf(stdout, "\rSaving... ", now, lines);
	}

	fclose(fp);
}

void keyInput(char* path, int srCol, int srRow, int* lines) {
	int saved = path == NULL ? 0 : 1; // 입력된 사항 저장 여부 확인. 0: 저장되지 않음 1: 저장됨

	char input = 0;

	//청크 리스트와 커서 생성
	chunk* chunk_list = getChunk();
	cursor* curCur = get_cursor(chunk_list);
	cd startCd = { 0,0 }, endCd = { 99, 22 };

	init_UI(srCol, srRow);

	if (path != NULL) readf(path, chunk_list, lines);

	chunkOn(chunk_list);
	if (chunk_list->next != NULL){
		chunkOn(chunk_list->next);
	}
	
	if (path != NULL) {
		print_UI(&startCd, curCur, &endCd, srCol, srRow, path, *lines);
	}

	do {
		input = _getch();

		if (input == CTRL_Q) {
			if (saved == 0) {
				gotoXY(srCol - 1, srRow - 1);
				fprintf(stdout, "\r FILE DOESN'T SAVED   EXIT = CTRL_Q | CANCEL = ANY KEY", path);

				input = _getch();

				if (input == CTRL_Q) input = 0;

			}
			else input = 0;
		}
		else if (input == CTRL_S) {
			if (path == NULL) {

				path = (char*)malloc(sizeof(char) * 100);

				path[0] = '*';	path[1] = '\0';

				int pathIdx = 0;

				while (1) {

					gotoXY(srCol - 1, srRow - 1);
					fprintf(stdout, "\r file name : %s  SAVE = ENTER | CANCEL = ESC", path);
					gotoXY(12 + pathIdx, srRow - 1);

					input = _getch();

					if (input == ENTER) {
						break;
					}
					else if (input == ESC) {
						free(path);
						path = NULL;
						break;
					}
					else if (input == BACKSPACE) {
						if (pathIdx > 0) pathIdx--;

						if (pathIdx == 0) {
							path[0] = '*';
							path[1] = '\0';
						}
						else path[pathIdx] = '\0';
					}
					else if (32 < input && input < 127) {
						path[pathIdx] = input;
						path[pathIdx + 1] = '\0';

						if (pathIdx < 98) pathIdx++;
					}
				}
			}

			if (saved == 0 && path != NULL) {
				if (curCur->curChunk->prev != NULL) chunkOff(curCur->curChunk->prev);
				chunkOff(curCur->curChunk);
				if (curCur->curChunk->next != NULL) chunkOff(curCur->curChunk->next);

				writef(path, chunk_list, *lines);

				if (curCur->curChunk->prev != NULL) chunkOn(curCur->curChunk->prev);
				chunkOn(curCur->curChunk);
				if (curCur->curChunk->next != NULL) chunkOn(curCur->curChunk->next);
			}
			
			saved = 1;
		}
		else if (input == BACKSPACE) {
			bsWord(curCur, lines);
			saved = 0;
		}
		else if (input == SPECIAL) {

			input = _getch();

			if (input == UP) {
				up(curCur);
			}
			else if (input == DOWN) {
				down(curCur);
			}
			else if (input == LEFT) {
				left(curCur);
			}
			else if (input == RIGHT) {
				right(curCur);
			}
			else if (input == HOME) {
				home(curCur);
			}
			else if (input == END) {
				end(curCur);
			}
			else if (input == PG_UP) {
				pgUp(&startCd, curCur);
			}
			else if (input == PG_DOWN) {
				pgDown(&startCd, curCur, &endCd, lines);
			}
		}
		else if (input == ENTER) {
			addWord('\n', curCur, lines);

			saved = 0;
		}
		else {
			addWord(input, curCur, lines);

			saved = 0;
		}

		check_cursor(&startCd, curCur, &endCd);

		print_UI(&startCd, curCur, &endCd, srCol, srRow, path, *lines);
	} while (input != 0);
	
	if (curCur->curChunk->prev != NULL) chunkOff(curCur->curChunk->prev);
	chunkOff(curCur->curChunk);
	if (curCur->curChunk->next != NULL) chunkOff(curCur->curChunk->next);
	
	if (path != NULL)free(path);
	
	free_chunk(&chunk_list);
}

/*
struct termios old, new;
		tcgetattr(STDIN_FILENO, &old); // 현재 터미널 설정 가져오기
		new = old;
		new.c_lflag &= ~(ICANON | ECHO); // 정규 모드와 에코 모드 비활성화
		tcsetattr(STDIN_FILENO, TCSANOW, &new);

		input = getchar();
*/