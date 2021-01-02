BINS=name_map.a name_map.so
OBJS=name_map.o
INCS=name_map.h

all: $(BINS)
	+$(MAKE) -C tests/

$(OBJS): $(INCS)

name_map.a: $(OBJS)
	ar rs $@ $^

name_map.so: $(OBJS)
	$(CC) $(CFLAGS) -shared -o $@ $^

test: all
	+$(MAKE) -C tests/ test

clean:
	rm -f $(BINS) $(OBJS)
	+$(MAKE) -C tests/ clean

