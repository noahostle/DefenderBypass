# Define compiler and flags
CC = x86_64-w64-mingw32-gcc
CFLAGS = -Wall
LDFLAGS = -lwinhttp

# Output binary names
TARGET1 = s1.exe
TARGET2 = s2.exe
TARGET3 = winhttp_request.exe

# Source files
SRCS1 = s1.c
SRCS2 = s2.c
SRCS3 = webreq.c

# Build the targets
$(TARGET1): $(SRCS1)
	$(CC) $(CFLAGS) $(SRCS1) -o $(TARGET1)

$(TARGET2): $(SRCS2)
	$(CC) $(CFLAGS) $(SRCS2) -o $(TARGET2)

$(TARGET3): $(SRCS3)
	$(CC) $(CFLAGS) $(SRCS3) -o $(TARGET3) $(LDFLAGS)

# Clean up the generated files
clean:
	rm -f $(TARGET1) $(TARGET2) $(TARGET3)

# All target (build all executables)
all: $(TARGET1) $(TARGET2) $(TARGET3)

