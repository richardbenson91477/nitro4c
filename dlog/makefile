BINS=dlog.a dlog.so
OBJS=dlog.o
INCS=dlog.h

all: $(BINS)
	+$(MAKE) -C tests/

$(OBJS): $(INCS)

dlog.a: $(OBJS)
	ar rs $@ $^

dlog.so: $(OBJS)
	$(CC) $(CFLAGS) -shared -o $@ $^

test: all
	+$(MAKE) -C tests/ test

clean:
	rm -f $(BINS) $(OBJS)
	+$(MAKE) -C tests/ clean

