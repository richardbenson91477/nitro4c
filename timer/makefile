BINS=timer.a timer.so
OBJS=timer.o
INCS=timer.h

all: $(BINS)
	+$(MAKE) -C tests/

$(OBJS): $(INCS)

timer.a: $(OBJS)
	ar rs $@ $^

timer.so: $(OBJS)
	$(CC) $(CFLAGS) -shared -o $@ $^

test: all
	+$(MAKE) -C tests/ test

clean:
	rm -f $(BINS) $(OBJS)
	+$(MAKE) -C tests/ clean

