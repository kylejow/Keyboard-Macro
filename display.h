#ifndef DISPLAY_H
#define DISPLAY_H

#include <windows.h>
#include <iostream>
#include <limits>

void setCursor(bool visible);
void clearScreen();
void printPOINT(POINT& p);
void clearLastLine(void);

#endif