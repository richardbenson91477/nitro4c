BINS=comp_1 comp_2
LDLIBS=../comp.a ../../spline/spline.a ../../arc/arc.a ../../file/file.a ../../dlog/dlog.a ../../blaze/blaze.a -lm
SPLINES=lin01.spline scale10.spline
COMPS=test1.comp test2.comp

.SUFFIXES: .splinea .spline .compa .comp
.splinea.spline:
	../../spline/tools/splinea_to_spline $^ $@
.compa.comp:
	../tools/compa_to_comp $^ $@

all: $(SPLINES) $(COMPS) $(BINS)

test: all
	./comp_1 | plot /dev/stdin 101 2 0
	./comp_2 | plot /dev/stdin 629 2 0

clean:
	rm -f $(BINS) $(SPLINES) $(COMPS)

