/* add.c
 *	Simple program to test whether the systemcall interface works.
 *	
 *	Just do a add syscall that adds two values and returns the result.
 *
 */

#include "syscall.h"

int
main()
{
  char buffer[255];
  int n;
  PrintString("Enter the number of character: ");
  n = ReadNum();
  if (n > 255 || n <= 0) PrintString("\nInvalid!");
  else {
    ReadString(buffer,n);
    PrintString("String input: "); PrintString(buffer);
  }
  Halt();
  /* not reached */
}
