#include "display.h"

void setCursor(bool visible){
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = visible;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}

void clearScreen(){
    HANDLE hOut;
    COORD Position;
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    Position.X = 0;
    Position.Y = 0;
    SetConsoleCursorPosition(hOut, Position);
}

void printPOINT(POINT& p){
    std::cout << p.x << ", " << p.y << "\n";
}

void clearLastLine(void){
    HANDLE handle;
    COORD pos;
    CONSOLE_SCREEN_BUFFER_INFO current;
    handle = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(handle, &current);
    pos.X = 0;
    pos.Y = current.dwCursorPosition.Y - 1;
    SetConsoleCursorPosition(handle, pos);
    std::cout << std::string(current.dwSize.X, ' ') << "\r";
}