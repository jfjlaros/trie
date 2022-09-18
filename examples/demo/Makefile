EXEC := test
MAIN := test.cc
LIBS :=

CC := g++
CC_ARGS := -std=c++20 -Wall -Wextra -pedantic -fcoroutines -O2


OBJS := $(addsuffix .o, $(LIBS))

.PHONY: clean distclean


all: $(EXEC)

$(EXEC): $(MAIN) $(OBJS)
	$(CC) $(CC_ARGS) -o $@ $^

%.o: %.c
	$(CC) $(CC_ARGS) -o $@ -c $<

%.o: %.cc
	$(CC) $(CC_ARGS) -o $@ -c $<

clean:
	rm -f $(OBJS)

distclean: clean
	rm -f $(EXEC)
