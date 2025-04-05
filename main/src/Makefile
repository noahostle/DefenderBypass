CC = x86_64-w64-mingw32-gcc

#SRC
STAGER1 = s1.c webreq.c
STAGER2 = s2.c
testing = msg.c

#output
DIR     = ../shellcode/executables/
TARGET1 = s1.exe
TARGET2 = s2.exe
TARGETt = msg.exe

# Build the targets
$(TARGET1): $(STAGER1)
	$(CC) $(STAGER1) -o ../$(TARGET1) -lwinhttp

$(TARGET2): $(STAGER2)
	$(CC) $(STAGER2) -o $(DIR)$(TARGET2)

$(TARGETt): $(testing)
	$(CC) $(testing) -o $(DIR)$(TARGETt)

# Clean up the generated files
clean:
	rm -f $(DIR)* ../$(TARGET1)

# All target (build all executables)
all: $(TARGET1) $(TARGET2)
test: $(TARGET1) $(TARGETt)
