
CC?=gcc
CFLAGS=-O0 -g -MMD -Wall

# Add other sorting algorithms .o here
BASE_OBJS=main.o list.o
BASE_OBJS+=bubble_sort.o
BASE_OBJS+=selection_sort.o
BASE_OBJS+=insertion_sort.o
BASE_OBJS+=quick_sort.o
BASE_OBJS+=merge_sort.o

ALL_OBJS=main.o replay_sdl.o replay_console.o $(BASE_OBJS)

.PHONY: all clean

all: sort sdl_sort

-include $(ALL_OBJS:.o=.d)

# Console sort visualizer
# ======================
sort: replay_console.o $(BASE_OBJS)
	$(CC) -o $@ $(LDFLAGS) $^ $(LDLIBS)

# SDL sort visualizer
# ==================
sdl_sort: LDLIBS+=-lSDL2 -lSDL2_image
sdl_sort: main.o replay_sdl.o $(BASE_OBJS)
	$(CC) -o $@ $(LDFLAGS) $^ $(LDLIBS)

clean:
	rm -rf sort sdl_sort $(ALL_OBJS) $(ALL_OBJS:.o=.d)

# GIF generation
# ==============
clear_shots:
	rm -rf shots/*.png

# With Image Magick (convert is part of it)
#export PATH=/cygdrive/c/Tools/ImageMagick:$PATH
sort.gif:
	convert -delay 2 -loop 0 "shots/*.png" $@
