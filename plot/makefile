BINS=plot
LDLIBS=../win/win.a ../dlog/dlog.a ../img/img.a ../file/file.a ../arc/arc.a ../blaze/blaze.a ../draw/draw.a -lSDL2 -lGL -lm

all: $(BINS)
	+$(MAKE) -C tests/

test: all
	+$(MAKE) -C tests/ test

clean:
	rm -f $(BINS)
	+$(MAKE) -C tests/ clean

