#include "syscall.h"

int main() {
    PrintString("20CLC08 - About us:\n");
    PrintString("20127039-Tran Dam Gia Huy\n");
    PrintString("20127043-Nguyen Thoai Dang Khoa\n");
    PrintString("20127666-Huynh Tan Vinh\n\n");
    PrintString("ASCII program: at folder \"test\", type  ../build.linux/nachos -x ascii, to print the ASCII table.\n");
    PrintString("\t-Program will print all readable characters in ascii table (from 32 to 126).\n");
    PrintString("\nSORT program: at folder \"test\", type  ../build.linux/nachos -x bubblesort, to start the sort array.\n");
    PrintString("\t-Enter the number of elements (maximum: 100)\n");
    PrintString("\t-Two available options:\n");
    PrintString("\t\t0: increasing, 1: decreasing\n");
    PrintString("\t-After sorting program will print out the sorted array.\n");
    PrintString("\nCREATE FILE program: at folder \"test\", type ../build.linux/nachos -x createfile, to create new file.\n");
    PrintString("\t-User will input file name from console read string; if the return value is 0, new file will be created.\n");
    PrintString("\nCAT program: at folder \"test\", type  ../build.linux/nachos -x cat, to display the content of this file.\n");
    PrintString("\t-User will input file name from console read string, the program will read each character in file and print.\n");
    PrintString("\nCOPY program: at folder \"test\", type  ../build.linux/nachos -x copy, to copy content from source file to destination file.\n");
    PrintString("\t-User will input source file name and destination file name, the program will copy content from source to des file.\n");
    PrintString("\nDELETE program: at folder \"test\", type  ../build.linux/nachos -x delete, to remove file out of directory.\n");
    PrintString("\t-User will input file name from console read string, the program will delete that file if it is available.\n");
    Halt();
}