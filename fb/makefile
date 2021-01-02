BINS=fb.so fb.a
OBJS=fb.o
INCS=fb.h

all: $(BINS)
	+$(MAKE) -C tests/

$(OBJS): $(INCS)

fb.a: $(OBJS)
	ar rs $@ $^

fb.so: $(OBJS)
	$(CC) $(CFLAGS) -shared -o $@ $^

test: all
	+$(MAKE) -C tests/ test

clean:
	rm -f $(BINS) $(OBJS)
	+$(MAKE) -C tests/ clean

