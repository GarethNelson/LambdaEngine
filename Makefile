CC=gcc

CFLAGS=-g
LDFLAGS=
LIBS=-lphysfs

O=build

ENGINE_OBJS = $(O)/engine/vfs_pak.o  $(O)/engine/vfs_init.o

dist/bin/lambda: $(ENGINE_OBJS) $(O)/engine/l_main.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(ENGINE_OBJS) $(O)/engine/l_main.o -o $@ $(LIBS)

build/core01/libs/video.so:
	touch build/core01/libs/video.so

dist/data/core01.pak: build/core01/libs/video.so
	cd build/core01; zip -r ../../dist/data/core01.pak *

dist/data/core02.pak:
	cd build/core02; zip ../../dist/data/core02.pak *

enginebin: dist/bin/lambda

corepaks: dist/data/core01.pak dist/data/core02.pak

all: enginebin corepaks

$(O)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
