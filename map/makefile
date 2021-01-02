BINS=map.a map.so
OBJS=map.o map_ent.o
INCS=map.h map_ent.h
ENUMS_MAPS=map_enums_map.h
SMTS=map_ent.smt

.SUFFIXES: .smta .smt
.smta.smt:
	../sys/tools/smta_to_smt $^ $@

all: $(ENUMS_MAPS) $(BINS) $(SMTS)
	+$(MAKE) -C tools/

$(OBJS): $(INCS)

$(ENUMS_MAPS): $(INCS)
	../enums_map/enums_map_gen.py map

map.a: $(OBJS)
	ar rs $@ $^

map.so: $(OBJS)
	$(CC) $(CFLAGS) -shared -o $@ $^

clean:
	rm -f $(BINS) $(OBJS) $(ENUMS_MAPS) $(SMTS)
	+$(MAKE) -C tools/ clean

