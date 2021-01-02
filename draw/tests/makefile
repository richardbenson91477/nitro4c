BINS=draw_1
LDLIBS=../../draw/draw.a ../../img/img.a ../../file/file.a ../../arc/arc.a ../../blaze/blaze.a ../../dlog/dlog.a -lSDL2 -lGL -lm
IMGS=draw_1.rwh draw_1.ppm

all: $(BINS)

test: all 
	./draw_1 &&\
		../../img/tools/rwh_to_ppm draw_1.rwh draw_1.ppm &&\
		gio open draw_1.ppm

clean:
	rm -f $(BINS) $(IMGS)

