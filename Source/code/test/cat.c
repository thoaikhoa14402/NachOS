#include "syscall.h"
int main(){
    char fileName[50], character;
    int nameSize=0, fileSize, openID, read, setZero=0, i;

    //Input file name size
    while(nameSize<=0){
        PrintString("Enter size of file name: ");
        nameSize=ReadNum();
    }
    //Input file name
    PrintString("Enter file name: ");
    ReadString(fileName, nameSize);

    openID=Open(fileName);//Open file

    if (openID == -1) PrintString("\nFile name is invalid\n");
    
    fileSize=Seek(-1,openID);// Get file size
    setZero=Seek(0,openID); // Seek pointer at 0 to read 

    PrintString("Output:\n");

    //Read each character in file and print
    for(i=0; i<fileSize; i++) {
        read=Read(&character,1,openID);
        PrintChar(character);
    }
    Halt();
    return 0;
}