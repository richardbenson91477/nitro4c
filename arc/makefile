BINS=arc.a arc.so
OBJS=arc.o
INCS=arc.h

all: $(BINS)
	+$(MAKE) -C tools/
	+$(MAKE) -C tests/

$(OBJS): $(INCS)

arc.a: $(OBJS)
	ar rs $@ $^

arc.so: $(OBJS)
	$(CC) $(CFLAGS) -shared -o $@ $^

test: all
	+$(MAKE) -C tests/ test

clean:
	rm -f $(BINS) $(OBJS)
	+$(MAKE) -C tools/ clean
	+$(MAKE) -C tests/ clean

