BINS=blaze.so blaze.a
OBJS=blaze.o 
INCS=blaze.h

all: $(BINS)
	+$(MAKE) -C tests/

$(OBJS): $(INCS)

blaze.a: $(OBJS)
	ar rs $@ $^

blaze.so: $(OBJS)
	$(CC) $(CFLAGS) -shared -o $@ $^

test: all
	+$(MAKE) -C tests/ test

clean:
	rm -f $(OBJS) $(BINS)
	+$(MAKE) -C tests/ clean

