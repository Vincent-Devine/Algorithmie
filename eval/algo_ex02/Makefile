PROGRAM=snowflake

# Add your objs to generate in OBJS var
OBJS=src/main.o src/ui.o
OBJS+=src/demo_snowflake.o

CXX?=g++
 
CXXFLAGS=-O0 -g -Wall -Wextra -Wno-unused-parameter
CPPFLAGS=-Iinclude -Ithird_party/include -MMD
LDFLAGS=-Lthird_party/libs
LDLIBS=-lraylib -ldl -lpthread -lm

DEPS=$(OBJS:.o=.d)

.PHONY: all clean

all: $(PROGRAM)

-include $(DEPS)

%.o: %.c
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@

$(PROGRAM): $(OBJS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

clean:
	rm -f $(OBJS) $(DEPS) $(PROGRAM)