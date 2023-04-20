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
  char ch;
  ch = ReadChar();
  PrintChar(ch);
  Halt();
  /* not reached */
}
