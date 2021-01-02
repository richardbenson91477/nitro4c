BINS=net.a net.so
OBJS=net.o
INCS=net.h

all: $(BINS)
	+$(MAKE) -C tests/

$(OBJS): $(INCS)

net.a: $(OBJS)
	ar rs $@ $^

net.so: $(OBJS)
	$(CC) $(CFLAGS) -shared -o $@ $^

test: all
	+$(MAKE) -C tests/ test

clean:
	rm -f $(OBJS) $(BINS)
	+$(MAKE) -C tests/ clean

