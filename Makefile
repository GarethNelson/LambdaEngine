CC=gcc

CFLAGS=-g
LDFLAGS=
LIBS=-lphysfs

O=build

ENGINE_OBJS = $(O)/engine/vfs_pak.o  $(O)/engine/vfs_init.o

dist/bin/lambda: $(ENGINE_OBJS) $(O)/engine/l_main.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(ENGINE_OBJS) $(O)/engine/l_main.o -o $@ $(LIBS)

enginebin: dist/bin/lambda

all: enginebin

$(O)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
