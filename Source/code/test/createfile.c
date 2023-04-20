#include "syscall.h"

int main() {
    char fileName[50];
    int length;

    PrintString("Enter the length of file name: ");
    length = ReadNum();
    PrintString("Enter file's name: ");
    ReadString(fileName, length);

    if (Create(fileName) == 0) {
        PrintString("File "); PrintString(fileName);
        PrintString(" was created successfully!\n");
    } else
        PrintString("Error create file.\n");
    Halt();
}