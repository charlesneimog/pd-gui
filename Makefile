# FOR WINDOWS 11
### pacman -S mingw-w64-x86_64-gtk4 mingw-w64-x86_64-libadwaita-1
### I am supposing that you have make, gcc and pkg-config installed

# FOR LINUX
### sudo apt install libgtk-4-dev libadwaita-1-dev

# FOR MACOS
### brew install libadwaita


uname := $(shell uname -s)

ifeq (MINGW,$(findstring MINGW,$(uname)))
	CFLAGS = -I C:/msys64/mingw64/include/libadwaita-1
	CFLAGS += `pkg-config --cflags gtk4`
	LDFLAGS = `pkg-config --libs gtk4` -ladwaita-1
else ifeq (Linux,$(findstring Linux,$(uname)))
	CC = /usr/bin/gcc
	CFLAGS = -I/usr/include/gtk-4.0 -I/usr/include/libadwaita-1/ -I/usr/include/cairo -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/pango-1.0 -I/usr/include/harfbuzz -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/graphene-1.0 -I/usr/lib/graphene-1.0/include
	LDFLAGS = -lcairo -ladwaita-1 -lgtk-4 -lglib-2.0
else ifeq (Darwin,$(findstring Darwin,$(uname)))

else
  $(error "Unknown system type: $(uname)")
  $(shell exit 1)
endif

SRC = main.c
OBJ = pdgtk

all: $(OBJ)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(OBJ)

.PHONY: all clean

