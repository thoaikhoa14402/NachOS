#include "syscall.h"
int main(){
    char fileName_1[50], fileName_2[50], character;
    int nameSize_1=0, nameSize_2=0, fileSize, openID_1, openID_2, read, write, setZero=0, i;

    //Input file name size 1
    while(nameSize_1<=0){
        PrintString("Enter size of file name 1: ");
        nameSize_1=ReadNum();
    }
    //Input file name 1
    PrintString("Enter file name 1: ");
    ReadString(fileName_1, nameSize_1);

    //Input file name size 2
    while(nameSize_2<=0){
        PrintString("Enter size of file name 2: ");
        nameSize_2=ReadNum();
    }
    //Input file name 2
    PrintString("Enter file name 1: ");
    ReadString(fileName_2, nameSize_2);

    openID_1=Open(fileName_1);//Open file 1
    if (openID_1 == -1) PrintString("File name 1 is invalid\n");
    openID_2=Open(fileName_2);//Open file 2
    if (openID_2 == -1) PrintString("File name 2 is invalid\n");

    if (openID_1 != -1 && openID_2 != -1)
    {
        fileSize=Seek(-1,openID_1);// Get file size
        setZero=Seek(0,openID_1); // Seek pointer at 0 to read 

        //Read each character in file and print
        for(i=0; i<fileSize; i++) {
            read=Read(&character,1,openID_1);
            write=Write(&character,1,openID_2);
        }
        PrintString("Copied successfully!");
    }
   
    Halt();
    return 0;
}