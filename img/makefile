BINS=img.so img.a
OBJS=img.o
INCS=img.h

all: $(BINS)
	+$(MAKE) -C tools/
	+$(MAKE) -C tests/

$(OBJS): $(INCS)

img.a: $(OBJS)
	ar rs $@ $^

img.so: $(OBJS)
	$(CC) $(CFLAGS) -shared -o $@ $^

test: all
	+$(MAKE) -C tests/ test

clean:
	rm -f $(BINS) $(OBJS)
	+$(MAKE) -C tools/ clean
	+$(MAKE) -C tests/ clean

