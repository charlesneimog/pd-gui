CC = /usr/bin/gcc
CFLAGS = -I/usr/include/libadwaita-1/

# add gtk4 flags and includes
CFLAGS += `pkg-config --cflags gtk4`
LDFLAGS = `pkg-config --libs gtk4` -ladwaita-1

SRC = main.c
OBJ = app

all: $(OBJ)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(OBJ)

.PHONY: all clean

