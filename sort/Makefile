
CC=gcc
CFLAGS=-O0 -g -MMD -Wall

ALL_OBJS=main.o bubble_sort.o selection_sort.o insertion_sort.o
EXES=bubble_sort selection_sort insertion_sort

.PHONY: all clean

all: $(EXES)

-include $(ALL_OBJS:.o=.d)

bubble_sort: main.o bubble_sort.o
	$(CC) -o $@ $(LDFLAGS) $^ $(LDLIBS)

selection_sort: main.o selection_sort.o
	$(CC) -o $@ $(LDFLAGS) $^ $(LDLIBS)

insertion_sort: main.o insertion_sort.o
	$(CC) -o $@ $(LDFLAGS) $^ $(LDLIBS)

clean:
	rm -rf $(EXES) $(ALL_OBJS) $(ALL_OBJS:.o=.d)
