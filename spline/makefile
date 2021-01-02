BINS=spline.a spline.so
OBJS=spline.o
INCS=spline.h
ENUMS_MAPS=spline_enums_map.h

all: $(ENUMS_MAPS) $(BINS)
	+$(MAKE) -C tools/
	+$(MAKE) -C tests/

$(OBJS): $(INCS)

$(ENUMS_MAPS): $(INCS)
	../enums_map/enums_map_gen.py spline

spline.a: $(OBJS)
	ar rs $@ $^

spline.so: $(OBJS)
	$(CC) $(CFLAGS) -shared -o $@ $^

test: all
	+$(MAKE) -C tests/ test

clean:
	rm -f $(BINS) $(OBJS) $(ENUMS_MAPS)
	+$(MAKE) -C tools/ clean
	+$(MAKE) -C tests/ clean

