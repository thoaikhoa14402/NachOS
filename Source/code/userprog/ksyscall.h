/**************************************************************
 *
 * userprog/ksyscall.h
 *
 * Kernel interface for systemcalls 
 *
 * by Marcus Voelp  (c) Universitaet Karlsruhe
 *
 **************************************************************/

#ifndef __USERPROG_KSYSCALL_H__ 
#define __USERPROG_KSYSCALL_H__ 

#include "kernel.h"



// turn off 
void SysHalt()
{
  cerr << "\n";
  kernel->interrupt->Halt();
}


int SysAdd(int op1, int op2)
{
  return op1 + op2;
}

// convert character to number and return
int SysReadNum()
{
  char c;
  char buffer[255];
  int n = 0;
  do 
  {
    c = kernel->synchConsoleIn->GetChar();
    if (c == ' ' || c == '\n' || c == EOF || c == 7 || c == 8 || c == 9) break;
    buffer[n++] = c;    
  }while(1);
  if (strcmp(buffer, "2147483647") == 0 ) return 2147483647UL;
  if (n > 11) return 0;
  else if (n ==  10)
  {
    for (int i = 0; i < 10; i++)
    {
      if (buffer[i] > "2147483647"[i]) return 0;
    }
  }
  else if (n == 11)
  {
    if (buffer[0] == '-')
    {
      for (int i = 0; i < 10; i++)
      {
        if (buffer[i + 1] > "2147483648"[i]) return 0;
      }
    }
    else return 0;
  }
  int num = 0;
  for (int i = 0; i < n; i++)
  {
    if (i == 0 && buffer[0] == '-') continue;
    if ((buffer[i] < '0' || buffer[i] > '9'))
    {
      cerr << "Invalid. Try again\n";
    }
    else num = num * 10 + (buffer[i] - '0');
  }
  if (buffer[0] == '-') num *= -1;
  return num;
}

// convert number to character and print to console
void SysPrintNum(int number)
{
  int cnt = 0;
  char buffer[255];
  int tmp = number;
  if (number == 0) kernel->synchConsoleOut->PutChar('0');
  else 
  {
        while(number != 0)
        {
          buffer[cnt++] = abs(number % 10) + '0';
          number /= 10;
        }
        if (tmp < 0) buffer[cnt++] = '-';
        for (int i = cnt - 1; i >= 0; i--)
        {
          kernel->synchConsoleOut->PutChar(buffer[i]);
        }
  }
}
// read character from console
char SysReadChar()
{
  return kernel->synchConsoleIn->GetChar();
}

// print character to console
void SysPrintChar(char ch)
{
  kernel->synchConsoleOut->PutChar(ch);
}

// return random number
int SysRandomNum()
{
  srand(time(NULL));
  return rand();
}

// convert string from kernel space to user space
void System2User(char *str, int addr, int str_length = 0)
{
  int length;
  if (str_length == 0)
    length = strlen(str);
  else
    length = str_length;
  for (int i = 0; i < length; i++)
  {
    kernel->machine->WriteMem(addr + i, 1, str[i]);
  }
  kernel->machine->WriteMem(addr + length, 1, '\0');
}

// read user string from console
void SysReadString(char *buffer, int length)
{
   for (int i = 0; i <= length; i++)
   {
     buffer[i] = kernel->synchConsoleIn->GetChar();
   }
   buffer[length] = '\0';
}

// convert string from user space to kernel space
char* User2System(int virtAddr, int str_length = 0)
{
      int i;// index
      int Char;
      char* buffer = NULL;
      buffer = new char[str_length +1]; 
      if (buffer == NULL) return buffer;
      memset(buffer,0,str_length+1);
      for (i = 0 ; i < str_length ;i++)
      {
        kernel->machine->ReadMem(virtAddr+i,1,&Char);
        buffer[i] = (char)Char;
        if (Char == 0) break;
      }
      return buffer;
}

// print string to console
void SysPrintString(char* buffer, int str_length) {
    for (int i = 0; i < str_length; i++) {
        kernel->synchConsoleOut->PutChar(buffer[i]);
    }
}

// Check whether can create file or not
int Create(char *fileName) // Check if can create file or not
{
    int fileName_length = strlen(fileName);
    if (fileName_length == NULL)
    {
      DEBUG(dbgSys, "\nNot enough memory in system");
      return 0;
    } 
    else if (fileName_length == 0) {
        DEBUG(dbgSys, "\nFile name can not be empty");
        return 0;

    } 
    else {
        DEBUG(dbgSys,"\n Finish reading filename.");
        if (!kernel->fileSystem->Create(fileName)) {
            DEBUG(dbgSys, "\nError create file");
            return 0;
        } else {
            return 1;
        }
    }
}

#endif /* ! __USERPROG_KSYSCALL_H__ */
