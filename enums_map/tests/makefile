BINS=enums_map_1
INCS=enums_map_1.h
LDLIBS=../enums_map.a ../../dlog/dlog.a 
ENUMS_MAPS=enums_map_1_enums.h

all: $(ENUMS_MAPS) $(BINS)

$(ENUMS_MAPS): $(INCS)
	../enums_map_gen.py enums_map_1

test: all
	./enums_map_1

clean:
	rm -f $(BINS) $(ENUMS_MAPS)

