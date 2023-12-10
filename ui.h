#include "wordlist.h"

#ifdef _WIN32

#ifndef __UI_WINDOWS_H__ 
#define __UI_WINDOWS_H__

#endif

#elif defined __linux__

#ifndef __UI_LINUX_H__ 
#define __UI_LINUX_H__

#endif

#elif defined __APPLE__ && defined __MACH__

#ifndef __UI_MAC_H__ 
#define __UI_MAC_H__

#endif

#endif

void gotoXY(int x, int y);

void init_UI(int srCol, int srRow);

void print_UI(cd* startCur, cursor* curCur, cd* endCur, int srCol, int srRow, char* path, int lines);