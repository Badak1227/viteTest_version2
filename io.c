#define _CRT_SECURE_NO_WARNINGS
#define CTRL_S 19
#define CTRL_Q 17
#define ENTER 13
#define BACKSPACE 8

#define HOME 71
#define END 79
#define PG_UP 73
#define PG_DOWN 81
#define SPECIAL -32
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77

#include <stdio.h>
#include "wordlist.h"

/*
#include <conio.h>

#include <stdio.h>
#include <conio.h>
#include "wordlist.h"
#include "ui.h"

void readf(FILE* fp, ln* lineList, int* lines) {
	cursor readCur;

	readCur.col = 1;
	readCur.row = 1;
	readCur.colp = lineList->lnstart;
	readCur.rowp = lineList;

	char input = 0;

	fscanf(fp, "%c", &input);
	while (input != EOF) {
		addWord(input, &readCur, lines);
	}
}

void writef(ln* lineList, FILE* fp, char* PATH) {

	wd* writeCur = lineList->lnstart;

	while (writeCur->word != -2) {
		char tmp = writeCur->word;

		if (tmp != -1) {
			fprintf(fp, "%c", tmp);
		}

		writeCur = writeCur->next;
	}

	fclose(fp);
	fp = fopen(PATH, "r+t");
}

void keyInput(ln* lineList, int* lines) {
	int saved = 1; // 입력된 사항 저장 여부 확인. 0: 저장되지 않음 1: 저장됨

	char input = 0;

	cursor curCur;

	//화면 시작 커서, 위치 커서, 끝 커서 선언 및 초기화
	curCur.colp = lineList->lnstart;
	curCur.rowp = lineList;
	curCur.col = 1;
	curCur.row = 1;

	do {

		input = _getch();

		if (input == CTRL_Q) {
			if (saved == 0) {
				input = _getch();

				if (input == 17) input = 0;

			}
			else input = 0;
		}
		else if (input == CTRL_S) {

			saved = 1;
		}
		else if (input == BACKSPACE) {
			bsWord(&curCur, lines);
			checkCursor(&startCur, &curCur, &endCur);
			saved = 0;
		}
		else if (input == SPECIAL) {

			input = _getch();

			if (input == UP) {
				upCursor(&curCur);
			}
			else if (input == DOWN) {
				downCursor(&curCur);
			}
			else if (input == LEFT) {
				leftCursor(&curCur);
			}
			else if (input == RIGHT) {
				rightCursor(&curCur);
			}
			else if (input == HOME) {
				home(&curCur);
			}
			else if (input == END) {
				end(&curCur);
			}
			else if (input == PG_UP) {
				pgUp(&startCur, &curCur, &endCur);
			}
			else if (input == PG_DOWN) {
				pgDown(&startCur, &curCur, &endCur);
			}

			checkCursor(&startCur, &curCur, &endCur);
		}
		else if (input == ENTER) {
			addWord('\n', &curCur, lines);

			checkCursor(&startCur, &curCur, &endCur);
			saved = 0;
		}
		else {
			addWord(input, &curCur, lines);

			checkCursor(&startCur, &curCur, &endCur);
			saved = 0;
		}

		print_UI(&curCur, &endCur, *lines);
	} while (input != 0);
}

/*
struct termios old, new;
		tcgetattr(STDIN_FILENO, &old); // 현재 터미널 설정 가져오기
		new = old;
		new.c_lflag &= ~(ICANON | ECHO); // 정규 모드와 에코 모드 비활성화
		tcsetattr(STDIN_FILENO, TCSANOW, &new);

		input = getchar();
*/