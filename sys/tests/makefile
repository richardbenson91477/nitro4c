BINS=sys_1
LDLIBS=../sys.a ../../name_map/name_map.a ../../comp/comp.a ../../spline/spline.a ../../arc/arc.a ../../file/file.a ../../timer/timer.a ../../dlog/dlog.a ../../blaze/blaze.a -lm
SYSS=sys_1.sys

.SUFFIXES: .sysa .sys
.sysa.sys:
	../tools/sysa_to_sys $^ $@

all: $(SYSS) $(BINS)

test: all
	./sys_1

clean:
	rm -f $(BINS) $(SYSS)

