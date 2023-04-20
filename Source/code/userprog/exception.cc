// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "main.h"
#include"synchconsole.h"
#include "syscall.h"
#include "ksyscall.h"
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// If you are handling a system call, don't forget to increment the pc
// before returning. (Or else you'll loop making the same system call forever!)
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	is in machine.h.
//----------------------------------------------------------------------

void increasePC()
{
/* Modify return point */
	{
	  /* set previous programm counter (debugging only)*/
	  kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

	  /* set programm counter to next instruction (all Instructions are 4 byte wide)*/
	  kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
	  
	  /* set next programm counter for brach execution */
	  kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
	} return;
	ASSERTNOTREACHED();
}


void Add()
{
		DEBUG(dbgSys, "Add " << kernel->machine->ReadRegister(4) << " + " << kernel->machine->ReadRegister(5) << "\n");
					/* Process SysAdd Systemcall*/
					int result;
					result = SysAdd(/* int op1 */(int)kernel->machine->ReadRegister(4),
							/* int op2 */(int)kernel->machine->ReadRegister(5));
					DEBUG(dbgSys, "Add returning with " << result << "\n");
					/* Prepare Result */
					kernel->machine->WriteRegister(2, (int)result);
}
// handle for SC_ReadNum
void ReadNumHandler() // read number from user's input
{
	int result;
	result = SysReadNum();
	kernel->machine->WriteRegister(2, (int)result);
}

// handle for SC_PrintNum
void PrintNumHandler()
{
	SysPrintNum(kernel->machine->ReadRegister(4));
}

// hamdle for SC_ReadChar
void ReadCharHandler()
{
	char ch;
	ch = SysReadChar();
	kernel->machine->WriteRegister(2, (char)ch);
}

// handle for SC_PrintChar
void PrintCharHandler()
{
	SysPrintChar(kernel->machine->ReadRegister(4));
}


// handle for SC_RandomNum
void RandomNumHandler()
{
	kernel->machine->WriteRegister(2,SysRandomNum());
}

// handle for SC_ReadString
void ReadStringHandler()
{
  int virtAddr = kernel->machine->ReadRegister(4); // from user
  int str_length = kernel->machine->ReadRegister(5); // from user
  if (str_length > 255) {  
        DEBUG(dbgSys, "String length is invalid" << 255);
        SysHalt();
  }
  char *buffer = new char[str_length + 1];
  SysReadString(buffer, str_length); // read charater in kernel space
  System2User(buffer, virtAddr); // copy from kernel to user
  delete [] buffer;
}

// handle for SC_PrintString
void PrintStringHandler()
{
	 int virtAddr = kernel->machine->ReadRegister(4); // from user
	 char* buffer = User2System(virtAddr,255);
     SysPrintString(buffer, strlen(buffer));
     delete[] buffer;
}


// hamdle for SC_CreateFile
void CreateFileHandler()
{
    int virtAddr;
    char* file_name;
	virtAddr = kernel->machine->ReadRegister(4);
	file_name = User2System(virtAddr, 32); // 32 is max file length
	if (Create(file_name))
	{
 		kernel->machine->WriteRegister(2, 0);
	}
    else
	{
 		kernel->machine->WriteRegister(2, -1);
	}
    delete[] file_name;
}

// handle for SC_OpenFile
void OpenFileHandler()
{
	char *file_name; // file name
	int id; // id of file
	file_name = User2System(kernel->machine->ReadRegister(4), 32); // convert from user's buffer to kernel's buffer
	id = (int)kernel->fileSystem->openFile(file_name); // get id of that file
	kernel->machine->WriteRegister(2, id);
	delete[] file_name;
}

// handle for SC_CloseFile
void CloseFileHandler()
{
	int id;
	id = kernel->machine->ReadRegister(4); // read id of file
	kernel->machine->WriteRegister(2, kernel->fileSystem->Close(id));	
}

// handle for SC_RemoveFile
void RemoveFileHandler()
{
	char *file_name;
	int virtAddr = kernel->machine->ReadRegister(4);
	file_name = User2System(virtAddr, 32);
	kernel->machine->WriteRegister(2, (int)kernel->fileSystem->Remove(file_name));
	delete[] file_name;
}

// handle for SC_ReadFile
void ReadFileHandler()
{
	int virtAddr = kernel->machine->ReadRegister(4);
	int numBytes = kernel->machine->ReadRegister(5);
	int id = kernel->machine->ReadRegister(6);
	char *buffer = new char[numBytes];
	if (id == 0) // if using console input
	{
		kernel->machine->WriteRegister(2, (int)kernel->synchConsoleIn->GetString(buffer, numBytes));
		System2User(buffer,virtAddr,numBytes);
	}
	else if (id < 0 || id >= MAX_FILE || id == 1) // if id is invalid, return -1
	{
		kernel->machine->WriteRegister(2, -1); 
	}
	else // if read data from file
	{
		kernel->machine->WriteRegister(2,(int)kernel->fileSystem->Read(buffer,numBytes, id));
		System2User(buffer,virtAddr,numBytes);
	}
	delete []buffer;
	buffer = NULL;
}

// handle for SC_WriteFile
void WriteFileHandler()
{
	int virtAddr = kernel->machine->ReadRegister(4);
	int numBytes = kernel->machine->ReadRegister(5);
	int id = kernel->machine->ReadRegister(6);
	char *buffer = User2System(virtAddr,255);
	if (id == 1) // if using console output
	{
		kernel->machine->WriteRegister(2,(int)kernel->synchConsoleOut->PutString(buffer, numBytes));
    }
	else if (id < 0 || id >= MAX_FILE || id == 0) 
	{
		kernel->machine->WriteRegister(2,-1);
	}
	// if write data to file
	else kernel->machine->WriteRegister(2,(int)kernel->fileSystem->Write(buffer,numBytes, id));
	delete []buffer;
	buffer = NULL;
}

// handle for SC_SeekFile
void SeekFileHandler()
{
	int position = kernel->machine->ReadRegister(4);
	int id = kernel->machine->ReadRegister(5);
	if (id <= 1) // if id file is 0 or 1 (console IO), return -1
	{
		DEBUG(dbgSys, "\nCan't seek in console");
		kernel->machine->WriteRegister(2, -1);
		return;
	}
	kernel->machine->WriteRegister(2, (int)kernel->fileSystem->Seek(position,id)); // return the position that file pointer is standing
}


void ExceptionHandler(ExceptionType which)
{
    int type = kernel->machine->ReadRegister(2);

    DEBUG(dbgSys, "Received Exception " << which << " type: " << type << "\n");

    switch (which) {
		case NoException:
				kernel->interrupt->setStatus(SystemMode);
				DEBUG(dbgSys, "Switched to system mode.\n");
				break;

		case PageFaultException:    // No valid translation found
		case ReadOnlyException:     // Write attempted to page marked 
		case BusErrorException:    // Translation resulted in an 
		case AddressErrorException: // Unaligned reference or one that
		case OverflowException:     // Integer overflow in add or sub.
		case IllegalInstrException: // Unimplemented or reserved instr.
		case NumExceptionTypes:					
				DEBUG(dbgSys, which << " exception occurs");
				cerr << "Error " << which << " occurs\n";
				SysHalt();
				ASSERTNOTREACHED();
				break;
    	case SyscallException:
      	switch(type) {
			  	// Input: none, Output: Turn off nachOS
      			case SC_Halt:
      				DEBUG(dbgSys, "Shutdown, initiated by user program.\n");
					SysHalt();
					ASSERTNOTREACHED();
					break;
    	  		case SC_Add:	
				    Add();
					increasePC();
					return;
					break;
				// Input: character, Output: none
				case SC_ReadNum:
				    ReadNumHandler();
					increasePC();
					return;
					break;
				// Input: integer number, Output: return the character after converting from number	
				case SC_PrintNum:
				    PrintNumHandler();
					increasePC();
					return;
					break;
				// Input: get character from kernel space, Output: return that character to user
				case SC_ReadChar:
				  	ReadCharHandler();
					increasePC();
					return;
					break;
				// Input: character from user, Output: print character to console	
				case SC_PrintChar:
				 	PrintCharHandler();
					increasePC();
					return;
					break;
				// Input: none, Output: return the random number	
				case SC_RandomNum:
					RandomNumHandler();
					increasePC();
					return;
					break;
				// Input: Buffer (char*), Output: none
				case SC_ReadString:
				 	ReadStringHandler();
					increasePC();
					return;
					break;
				// Input: Buffer (char*), Output: print buffer to console	
				case SC_PrintString:
				    PrintStringHandler();
				 	increasePC();
					return;
					break;
				// Input: file name & size, Output: file is created
				case SC_CreateFile:
				  CreateFileHandler();
					increasePC();
					return;
					break;
				// Input: file name, Output: file is opened	
				case SC_OpenFile:
					OpenFileHandler();
					increasePC();
					return;
					break;
				// Input: file's ID, Output: file is closed	
				case SC_CloseFile:
					CloseFileHandler();
					increasePC();
					return;
					break;	
				// Input: buffer, number of characters to read, file's ID
				// Output: number of characters was read	
				case SC_ReadFile:
					ReadFileHandler();
					increasePC();
					return;
					break;
				// Input: buffer, number of characters to write, file's ID
				// Output: number of characters was written	
				case SC_WriteFile:
					WriteFileHandler();
					increasePC();
					return;
					break;
				// Input: position, file's ID, Output: -1: last position in file, else return the exact position
				case SC_SeekFile:
					SeekFileHandler();
					increasePC();
					return;
					break;
				// Input: file's name (char*), Output: file was removed	
				case SC_RemoveFile:
				 	RemoveFileHandler();
					increasePC();
					return;
					break;
				
   		default:
				cerr << "Unexpected system call " << type << "\n";
				return;
				break;
    }
    break;
    default:
      cerr << "Unexpected user mode exception" << (int)which << "\n";
			return;
      break;
    }
    ASSERTNOTREACHED();
}
