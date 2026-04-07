CC     = cc 
OPT    = -O3
WARN   = -Wall -Wextra -Werror
TARGET = blob 
FILE   = cli.c

compile:
	$(CC) -o $(TARGET) $(FILE) $(OPT) $(WARN)

install:
	$(CC) -o $(TARGET) $(FILE) $(OPT) $(WARN) && sudo cp ./$(TARGET) /usr/local/bin && rm -rf ./$(TARGET)

run:
	$(CC) -o $(TARGET) $(FILE) $(OPT) $(WARN) && ./$(TARGET)


clean:
	rm -rf main
