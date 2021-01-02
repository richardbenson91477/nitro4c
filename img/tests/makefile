BINS=rwh_to_rwh raw_to_raw
LDLIBS=../img.a ../../dlog/dlog.a ../../file/file.a ../../arc/arc.a ../../blaze/blaze.a

all: $(BINS)

test: all
	../tools/png_to_bgra colors.png colors.bgra
	diff -s colors.bgra colors.correct.bgra
	../tools/png_to_rwh colors.png colors.rwh
	diff -s colors.rwh colors.correct.rwh
	./raw_to_raw colors.correct.bgra colors.bgra
	diff -s colors.bgra colors.correct.bgra
	./rwh_to_rwh colors.correct.rwh colors.rwh
	diff -s colors.rwh colors.correct.rwh

clean:
	rm -f $(BINS) colors.bgra colors.rwh

