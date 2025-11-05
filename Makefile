BIN = bin

ifeq ($(OS),Windows_NT)
TARGET = bin/inet6.dll
OS_FLAGS = -I . -lws2_32
else
TARGET = bin/inet6.so
OS_FLAGS =
endif


$(TARGET): $(BIN)
	gcc -Wall -g -fPIC -shared init.c -o $(TARGET) $(OS_FLAGS)

$(BIN):
	mkdir $(BIN)

clean:
	rm -rf $(BIN)/*

test: $(TARGET)
	./testsuite.sh
