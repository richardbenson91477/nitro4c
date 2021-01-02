BINS=snd.a snd.so
OBJS=snd.o
INCS=snd.h

all: $(BINS)
	+$(MAKE) -C tests/

$(OBJS): $(INCS)

snd.a: $(OBJS)
	ar rs $@ $^

snd.so: $(OBJS)
	$(CC) $(CFLAGS) -shared -o $@ $^

test: all
	+$(MAKE) -C tests/ test

clean:
	rm -f $(BINS) $(OBJS)
	+$(MAKE) -C tests/ clean

