BINS=draw.a draw.so
OBJS=draw.o
INCS=draw.h

all: $(BINS)
	+$(MAKE) -C tests/

$(OBJS): $(INCS)

draw.a: $(OBJS)
	ar rs $@ $^

draw.so: $(OBJS)
	$(CC) $(CFLAGS) -shared -o $@ $^

test: all
	+$(MAKE) -c tests/ test

clean:
	rm -f $(OBJS) $(BINS)
	+$(MAKE) -C tests/ clean

