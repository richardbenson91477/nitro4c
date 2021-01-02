BINS=vg_1
LDLIBS=../vgrab.a ../../img/img.a ../../dlog/dlog.a ../../file/file.a ../../arc/arc.a ../../blaze/blaze.a -lv4l2

all: $(BINS)

test: all
	./vg_1

clean:
	rm -f $(BINS) out*.rwh

