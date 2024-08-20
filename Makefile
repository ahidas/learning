CC=clang
TARGET=keylogger.exe 
CFLAGS = -Wall
SRC = keylog.c 
OBJS = keylog.o 

all: $(TARGET)

%.o: %.c 
	$(CC) $(CFLAGS) -c $<


server: reciever.o 
	$(CC) -o server reciever.o $(CFLAGS)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS)
 
format:
	clang-format -i -style=file *.[c.h]
clean:
	rm -f $(TARGET) $(OBJS)

