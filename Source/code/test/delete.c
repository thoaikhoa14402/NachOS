#include "syscall.h"

int main()
{
  char fileName[50];
  int length, num;

    PrintString("Enter the length of file name: ");
    length = ReadNum();
   
    PrintString("Enter file's name: ");
    ReadString(fileName, length);

    do
    {
        PrintString("Do you want to remove this file? 1 - Yes, No - 0\n");
        num = ReadNum();
    }while(num != 1 && num != 0);

    if (num)
    {
         if (Remove(fileName))
            PrintString("This file was removed successfully!");
         else PrintString("Cant remove this file");
    }
    else PrintString("\nBye Bye");
  Halt();
  /* not reached */
}
