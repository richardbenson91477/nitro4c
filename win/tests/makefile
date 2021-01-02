BINS=notes color_test alpha_test
LDLIBS=../win.a ../../dlog/dlog.a ../../img/img.a ../../file/file.a ../../arc/arc.a ../../blaze/blaze.a ../../draw/draw.a -lSDL2 -lGL 

all: $(BINS)

test: all
	./notes

clean:
	rm -f $(BINS)

