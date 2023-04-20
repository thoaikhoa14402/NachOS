#include "syscall.h"

int main() {
    int i; // index
    for (i = 32; i <= 126; i++) {
        PrintNum(i);
        PrintString(": ");
        PrintChar(i);
        PrintChar('\n');
    }
    Halt();
}