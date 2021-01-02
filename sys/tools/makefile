BINS=sysa_to_sys smta_to_smt
LDLIBS=../sys.a ../../name_map/name_map.a ../../comp/comp.a ../../spline/spline.a ../../arc/arc.a ../../file/file.a ../../enums_map/enums_map.a ../../dlog/dlog.a ../../blaze/blaze.a -lm
SMTS=sys.smt

.SUFFIXES: .smta .smt
.smta.smt:
	./smta_to_smt $^ $@

all: $(BINS) $(SMTS)

clean:
	rm -f $(BINS) $(SMTS)

