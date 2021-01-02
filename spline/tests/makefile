BINS=spline_1
LDLIBS=../spline.a ../../arc/arc.a ../../file/file.a ../../dlog/dlog.a ../../blaze/blaze.a -lm
SPLINES=blend_1.spline

.SUFFIXES: .splinea .spline
.splinea.spline:
	../tools/splinea_to_spline $^ $@

all: $(SPLINES) $(BINS)

test: all
	./spline_1 | plot /dev/stdin 51 2 0
	../tools/spline_lerp blend_1.spline | plot /dev/stdin 201 2 0

clean:
	rm -f $(BINS) $(SPLINES)

