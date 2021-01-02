BINS=sys.a sys.so
OBJS=sys.o
INCS=sys.h
ENUMS_MAPS=sys_enums_map.h

all: $(ENUMS_MAPS) $(BINS)
	+$(MAKE) -j1 -C tools/
	+$(MAKE) -C tests/

$(OBJS): $(INCS)

$(ENUMS_MAPS): $(INCS)
	../enums_map/enums_map_gen.py sys

sys.a: $(OBJS)
	ar rs $@ $^

sys.so: $(OBJS)
	$(CC) $(CFLAGS) -shared -o $@ $^

test: all
	+$(MAKE) -C tests/ test

clean:
	rm -f $(BINS) $(OBJS) $(ENUMS_MAPS)
	+$(MAKE) -C tools/ clean
	+$(MAKE) -C tests/ clean

