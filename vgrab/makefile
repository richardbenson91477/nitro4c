BINS=vgrab.a vgrab.so
OBJS=vgrab.o
INCS=vgrab.h

all: $(BINS)
	+$(MAKE) -C tests/

$(OBJS): $(INCS)

vgrab.a: $(OBJS)
	ar rs $@ $^

vgrab.so: $(OBJS)
	$(CC) $(CFLAGS) -shared -o $@ $^

test: all
	+$(MAKE) -C tests/ test

clean:
	rm -f $(BINS) $(OBJS)
	+$(MAKE) -C tests/ clean

