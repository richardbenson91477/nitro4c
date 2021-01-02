BINS=enums_map.a enums_map.so
OBJS=enums_map.o
INCS=enums_map.h

all: $(BINS)
	+$(MAKE) -C tests/

$(OBJS): $(INCS)

enums_map.a: $(OBJS)
	ar rs $@ $^

enums_map.so: $(OBJS)
	$(CC) $(CFLAGS) -shared -o $@ $^

test: all
	+$(MAKE) -C tests/ test

clean:
	rm -f $(BINS) $(OBJS)
	+$(MAKE) -C tests/ clean

