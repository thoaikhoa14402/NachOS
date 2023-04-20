#include "syscall.h"
int main()
{
  int n, arr[100], choice;
  int i,j,temp; // used for sorting
  PrintString("\n---SORTING PROGRAM---\n");
  // Input
   do {
        PrintString("Enter the number of elements (maximum: 100): ");
        n = ReadNum();
        if (n <= 0 || n > 100) PrintString("Invalid! Try again \n");
    } while (n <= 0 || n > 100);

    for (i = 0; i < n; i++) {
        PrintString("a[");
        PrintNum(i);
        PrintString("]: ");
        arr[i] = ReadNum();
    }
    // option to sort array
    do {
        PrintString("Enter your choice (0: increasing order, 1: decreasing order): ");
        choice = ReadNum();
        if (choice != 0 && choice != 1) PrintString("Invalid syntax, please try again\n");
    } while (choice != 0 && choice != 1);
    // bubble sort algorithm
    for (i = 0; i < n; i++) {
        for (j = 0; j < n - 1; j++) {
            if (choice == 0) {
                if (arr[j] > arr[j + 1]) {
                    temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
            } else if (choice == 1) {
                if (arr[j] < arr[j + 1]) {
                    temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
            }
        }
    }
    // print the result
    PrintString("Array after sorting: \n");
    for (i = 0; i < n; i++) {
        PrintNum(arr[i]);
        PrintChar(' ');
    }
  Halt();
  /* not reached */
}
 