#include "syscall.h"

int main() {
   char fileName[50];
    int length, id, pos, num;
   
    // Open file testing
    PrintString("Enter the length of file name: ");
    length = ReadNum();
    PrintString("Enter file's name: ");
    ReadString(fileName, length);
    id = Open(fileName);
    if (id != -1) {
    PrintString("File "); PrintString(fileName); PrintString(" was opened successfully!\n");
    PrintString("File's ID: "); PrintNum(id);
    PrintString("\n");
    } else PrintString("Failed to open file\n");
  

    // Seek file testing
    do
    {
        PrintString("Do you want to move file pointer to position? 1 - Yes, No - 0\n");
        num = ReadNum();
    }while(num != 1 && num != 0); 

    if (num)
    {
         PrintString("\nEnter the position you want to move file pointer: ");
         pos = ReadNum();
         if (Seek(pos,id) != -1)
         {
            PrintString("The file pointer was moved successfully!");
            PrintChar('('); PrintNum(Seek(pos,id)); PrintChar(')');
        
         }
         else PrintString("That position is invalid!");
    }
    // Close file testing    
    do
    {
        PrintString("\nDo you want to close this file? 1 - Yes, No - 0\n");
        num = ReadNum();
    }while(num != 1 && num != 0);    

    if (num)
    {
         if (Close(id) != -1)
            PrintString("This file was closed successfully!");
         else PrintString("Cant close this file");
    }
    else PrintString("\nBye Bye");
    Halt();
}