CC     = cc 
OPT    = -O3
WARN   = -Wall -Wextra -Werror
TARGET = main 

compile:
	$(CC) -o $(TARGET) main.c $(OPT) $(WARN)


run:
	$(CC) -o $(TARGET) main.c $(OPT) $(WARN) && ./$(TARGET)


clean:
	rm -rf main
