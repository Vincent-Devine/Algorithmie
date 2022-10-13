
CC?=gcc
CFLAGS=-O0 -g
CPPFLAGS=-MMD
LDFLAGS=
LDLIBS=

OBJS=src/test_naive_search.o src/test_smart_search.o src/dataset.o
DEPS=$(OBJS:.o=.d)

all: test_naive_search test_smart_search

-include $(DEPS)

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

test_naive_search: src/test_naive_search.o src/dataset.o
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

test_smart_search: src/test_smart_search.o src/dataset.o
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

clean:
	rm -f test_naive_search test_smart_search $(OBJS) $(DEPS)