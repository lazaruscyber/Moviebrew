CC=clang
CFLAGS=-Wall -O2 -I/opt/homebrew/opt/cjson/include
LDFLAGS=-L/opt/homebrew/opt/cjson/lib -lcjson
SRCS=src/main.c src/config.c src/commands.c
TARGET=moviebrew

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) $(LDFLAGS)

clean:
	rm -f $(TARGET)
