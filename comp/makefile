BINS=comp.a comp.so
OBJS=comp.o
INCS=comp.h
ENUMS_MAPS=comp_enums_map.h

all: $(ENUMS_MAPS) $(BINS)
	+$(MAKE) -C tools/
	+$(MAKE) -C tests/

$(OBJS): $(INCS)

$(ENUMS_MAPS): $(INCS)
	../enums_map/enums_map_gen.py comp

comp.a: $(OBJS)
	ar rs $@ $^

comp.so: $(OBJS)
	$(CC) $(CFLAGS) -shared -o $@ $^

test: all
	+$(MAKE) -C tests/ test

clean:
	rm -f $(BINS) $(OBJS) $(ENUMS_MAPS)
	+$(MAKE) -C tools/ clean
	+$(MAKE) -C tests/ clean

