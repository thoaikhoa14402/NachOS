#include "syscall.h"

#define console_in 0
#define console_out 1

int main() {
    int i;
    int file_id;
    char buffer[255];
    char buffer_io[255];
    int read,write;
    int len = 0;
    int num = 0; 

    // Read testing
    PrintString("---READ FILE TESTING---\n");
    file_id = Open("abc"); 
    if (file_id == -1) PrintString("File ID is invalid. PLease try again!\n");
    else
    {
        PrintString("Enter the number of characters for reading: "); num = ReadNum();
        read = Read(buffer,num,file_id);
        while (buffer[len] != '\0') ++len;
        PrintString("Read ");
        PrintNum(len);
        PrintString(" characters (from file abc): ");
        if (len != 0) PrintString(buffer);
        PrintString("\n");
        Close(file_id);
    }
    
    //Write testing
    PrintString("\n---WRITE FILE TESTING---\n");
    file_id = Open("createfile.txt");
    if (file_id == -1) PrintString("File ID is invalid. Please try again!\n");
    else 
    {
        write = Write(buffer, len, file_id);
        PrintString("Write ");
        PrintNum(write);
        PrintString(" characters (into file createfile.txt): ");
        if (len != 0) PrintString(buffer);
        PrintString("\n");
        Close(file_id);
    }

    // CONSOLE IO
    //Console Input testing
    PrintString("Enter the number of characters for Console Input: "); num = ReadNum();
    PrintString("Console input: ");
    read = Read(buffer_io,num,console_in);
    len=0;  
    while (buffer_io[len] != '\0') ++len;
    PrintString("(Read ");
    PrintNum(len);
    PrintString(" characters into buffer)\n");

    // Console Output testing
    PrintString("\nConsole output: ");
    write = Write(buffer_io, len, console_out);
    PrintString("\n(Wrote ");
    PrintNum(write);
    PrintString(" characters from buffer)");

    Halt();
 }