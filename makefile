CC     = cc 
OPT    = -O3
WARN   = -Wall -Wextra -Werror
TARGET = main 
FILE   = cli.c

compile:
	$(CC) -o $(TARGET) $(FILE) $(OPT) $(WARN)


run:
	$(CC) -o $(TARGET) $(FILE) $(OPT) $(WARN) && ./$(TARGET)


clean:
	rm -rf main
