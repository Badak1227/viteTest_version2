#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include "wordlist.h"

void gotoXY(int x, int y) {
	HANDLE  hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO Con;
	GetConsoleScreenBufferInfo(hCon, &Con);


	COORD pos = { x, y };
	SetConsoleCursorPosition(hCon, pos);
}

void gotoRC(cd* startCd, cursor* curCur, cd* endCd, HANDLE hCon) {

	COORD pos = { curCur->x - startCd->x, curCur->y - startCd->y };
	SetConsoleCursorPosition(hCon, pos);
}

void revColor(HANDLE hCon, CONSOLE_SCREEN_BUFFER_INFO Con) {

	WORD color = Con.wAttributes;

	SetConsoleTextAttribute(hCon, ((color >> 4) & 0x000F) | ((color & 0x000F) << 4));
}

void text_print(cd* startCd, cursor* curCur, cd* endCd, char buffer[23][101]) {
	for (int i = 0; i < 23; i++) {
		buffer[i][0] = '~';
		buffer[i][1] = '\0';

		for (int j = 2; j < 100; j++) {
			buffer[i][j] = ' ';
		}

		buffer[i][100] = '\0';
	}

	chunk* curChunk = curCur->curChunk;
	line* curLine = curChunk->line_list;

	if (curCur->line < curCur->y - startCd->y) {
		int gap = (curCur->y - startCd->y) - curCur->line;

		curChunk = curChunk->prev;
		curLine = curChunk->line_list;

		for (int i = 0; i < curChunk->listSize - gap; i++) {
			curLine = curLine->down;
		}
	}
	else {
		for (int i = 0; i < curCur->line - (curCur->y - startCd->y); i++) {
			curLine = curLine->down;
		}
	}

	for (int i = 0; i < 23; i++) {
		for (int j = startCd->x, k = 0; j <= 100 + startCd->x; j++, k++) {
			//현재 라인이 편집화면에 표시되지 않을 경우
			if (curLine->textSize <= j) {
				buffer[i][k] = '\0';
				break;
			}
			//현재라인의 끝에 도달했거나 버퍼의 끝에 도착햇을 경우
			if (j == 100 + startCd->x || j == curLine->textSize) break;

			//개행문자를 만날 경우
			if (curLine->text[j] == '\n') {
				buffer[i][k] = '\0';
				break;
			}

			buffer[i][k] = curLine->text[j];
		}

		if (curLine->down != NULL) curLine = curLine->down;
		else {
			if (curChunk->next == NULL)break;
			else curChunk = curChunk->next;

			curLine = curChunk->line_list;
		}
	}
}

void init_UI(int srCol, int srRow) {

	HANDLE  hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO Con;
	GetConsoleScreenBufferInfo(hCon, &Con);

	Con.dwSize.Y = srRow;
	Con.dwSize.X = srCol;
	Con.srWindow.Bottom = srRow - 1;
	Con.srWindow.Right = srCol - 1;

	SetConsoleScreenBufferSize(hCon, Con.dwSize);
	SetConsoleWindowInfo(hCon, TRUE, &Con.srWindow);

	for (int i = 0; i < srRow - 2; i++) {
		fprintf(stdout, "~");
		if (i == 8) {
			for (int j = 1; j < srCol / 2 - 18; j++) {
				fprintf(stdout, " ");
			}
			fprintf(stdout, "Visual Text editor -- version 0.0.1");
		}
		fprintf(stdout, "\n");
	}

	revColor(hCon, Con);
	fprintf(stdout, " [NO Name] - 1 lines");

	GetConsoleScreenBufferInfo(hCon, &Con);
	for (int j = Con.dwCursorPosition.X; j < srCol - 9; j++) {
		fprintf(stdout, " ");
	}
	fprintf(stdout, "  1 /   1\n");
	revColor(hCon, Con);

	printf(" HELP: Ctrl-S = save | Ctrl-Q = quit | Ctrl-F = find");

	COORD pos = { 0, 0 };
	SetConsoleCursorPosition(hCon, pos);
}

void print_UI(cd* startCd, cursor* curCur, cd* endCd, int srCol, int srRow, char* path, int lines) {
	HANDLE  hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO Con;
	GetConsoleScreenBufferInfo(hCon, &Con);
	
	char buffer[23][101];
	text_print(startCd, curCur, endCd, buffer);

	system("cls");

	for (int i = 0; i < 23; i++) {
		fprintf(stdout, "%s\n", buffer[i]);
		continue;
	}

	revColor(hCon, Con);
	if (path == NULL) {
		fprintf(stdout, " [NO Name] - %d lines", lines);
	}
	else {
		fprintf(stdout, " [%s] - %d lines", path, lines);
	}

	GetConsoleScreenBufferInfo(hCon, &Con);
	for (int j = Con.dwCursorPosition.X; j < srCol - 9; j++) {
		fprintf(stdout, " ");
	}
	fprintf(stdout, "%3d / %3d\n", curCur->y + 1, curCur->x + 1);
	revColor(hCon, Con);

	printf(" HELP: Ctrl-S = save | Ctrl-Q = quit | Ctrl-F = find   currentChunk: %s", curCur->curChunk->path);

	gotoRC(startCd, curCur, endCd, hCon);
}